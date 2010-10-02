//
//  RubyInterop.m
//  iaic
//
//  Created by Ashley Towns on 12/07/10.
//

#import "RubyInterop.h"

// Ruby => Obj-c vice versa interop
// Helper methods including NSDictionary => ruby symbol hash 
//                          NSString => ruby string
// Event driven firing notifications 
//
// Basically in ruby register for a callback
// a = IAIC.new
// a.callbackManager("onLoad",ONLOAD)
// and define a method onLoad.
//
// Obj-c side
// [[NSNotificationCenter defaultCenter] postNotificationName:kRBonLoad object:[NSDictionary dictionaryWithObject:@"foo" andKey:@"bar"]];
// OR
// [RubyInterop fireEvent:kRBonLoad content:[NSDictionary dictionaryWithObject:@"wee" andKey:@"waa"]];
//
// 
@implementation RubyInterop
@synthesize registeredCallbacks;
const char* rbfile;
VALUE IAIC_klass;
#pragma mark -
#pragma mark RUBY METHODS
/* Static ruby embedded because macruby requires gc compiles which fuck everything up royally */
static VALUE t_init(VALUE self)
{
    //LOG(@"%s",STR2CSTR(klass));
    //VALUE arr;
    //arr = rb_ary_new();
    //rb_iv_set(self, "@arr", arr);
    return self;
}

static VALUE _rbCallbackManager(VALUE self, VALUE aKlass, VALUE anObject, VALUE callback) {
    LOG(@"Ruby code requesting a callback: %@", [NSString fromRubyString:anObject]);
    // Lets create the class instance here.... (prepend? iaic_)
    //
    NSString* instance_name =  [NSString stringWithFormat:@"iaic_%@",[NSString fromRubyString:aKlass]];
    VALUE ourklass;
    if (rb_gv_get([instance_name UTF8String]) == Qnil) {
        LOG(@"Creating a new class.");
        ourklass = rb_class_new_instance(0, 0, rb_const_get(rb_cObject, rb_intern([[NSString fromRubyString:aKlass] UTF8String])));
    } else {
        LOG(@"Class already exists.");
        ourklass = rb_gv_get([instance_name UTF8String]);
    }
    rb_gv_set([instance_name UTF8String], ourklass);
    //
    NSDictionary* cbObj = [NSDictionary dictionaryWithObjectsAndKeys:kCBTypeRegister,kCBType,
                           [NSString fromRubyString:anObject],kCBMethodDef,
                           instance_name,kCBKlassDef,
                           [NSString fromRubyString:callback],kCBCallbackType,
                           [NSString stringWithUTF8String:rbfile],kCBFileName,
                          nil];
    [[NSNotificationCenter defaultCenter] postNotificationName:kRbCallbackManager_cbCenter object:cbObj];
    return Qnil;
}

static VALUE _rbEchoTo (VALUE _self, VALUE to, VALUE content){
    NSDictionary* payload = [NSDictionary dictionaryWithObjectsAndKeys:kCBScriptCommandWriteLine,kCBScriptCommandType,
                             [NSString fromRubyString:content],kCBScriptCommandPayloadArgs0,
                             [NSString fromRubyString:to],kCBScriptCommandPayloadArgs1,
                            nil];
    [[NSNotificationCenter defaultCenter] postNotificationName:kCBScriptCommand object:payload];
    return Qnil;
}

static VALUE _rbTest (VALUE _self){
    [[NSNotificationCenter defaultCenter] postNotificationName:kRbTest object:nil]; 
    return [@"Everything is A-OK" toRubyString]; //rbStr("Everything is A-OK");
}

#pragma mark -
#pragma mark OBJECTIVE-C Methods

-(void)dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:self forKeyPath:kRbCallbackManager_cbCenter];
    [[NSNotificationCenter defaultCenter] removeObserver:self forKeyPath:kRBonLoad];
    [[NSNotificationCenter defaultCenter] removeObserver:self forKeyPath:kRBonJoin];
    [[NSNotificationCenter defaultCenter] removeObserver:self forKeyPath:kRBonPart];
    [[NSNotificationCenter defaultCenter] removeObserver:self forKeyPath:kRBonKick];
    [[NSNotificationCenter defaultCenter] removeObserver:self forKeyPath:kRBonQuit];
    [[NSNotificationCenter defaultCenter] removeObserver:self forKeyPath:kRBonUnload];
    [[NSNotificationCenter defaultCenter] removeObserver:self forKeyPath:kRBonChanMsg];
    //
    ruby_finalize();
    registeredCallbacks = nil;
    [super dealloc];
}
+ (void)ruby_initialize {
    ruby_init();
    ruby_init_loadpath();
    rb_define_global_const("ONLOAD", [kRBonLoad toRubyString]);
    rb_define_global_const("ONJOIN", [kRBonJoin toRubyString]);
    rb_define_global_const("ONPART", [kRBonPart toRubyString]);
    rb_define_global_const("ONKICK", [kRBonKick toRubyString]);
    rb_define_global_const("ONNICK", [kRBonNick toRubyString]);
    rb_define_global_const("ONQUIT", [kRBonQuit toRubyString]);
    rb_define_global_const("ONUNLOAD", [kRBonUnload toRubyString]);
    rb_define_global_const("ONCHANMSG", [kRBonChanMsg toRubyString]);
    
    IAIC_klass = rb_define_class("IAIC", rb_cObject);
    rb_define_method(IAIC_klass, "initialize", t_init, 0);
    rb_define_method(IAIC_klass, "callbackManager", _rbCallbackManager, 3);
    rb_define_method(IAIC_klass, "echoto", _rbEchoTo, 2);
}

-(id)initWithScript:(NSString*)script {
    if (self = [super init]) {
        filename = script;
        registeredCallbacks = [[[NSMutableDictionary alloc] initWithCapacity:0] retain];
        // Main
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(CallbackCenter:) name:kRbCallbackManager_cbCenter object:nil];
        // Events
        //[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(callbackHandler:) name:kRBonLoad object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(callbackHandler:) name:kRBonJoin object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(callbackHandler:) name:kRBonPart object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(callbackHandler:) name:kRBonKick object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(callbackHandler:) name:kRBonQuit object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(callbackHandler:) name:kRBonUnload object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(callbackHandler:) name:kRBonChanMsg object:nil];
        // Ruby init
        //[self ruby_initialize];
        // Unsafe!
        rbfile = [script UTF8String];
        rb_load_file([script UTF8String]);
    
        //ruby_script([script UTF8String]);
        ruby_exec();
        // Safe
        //int status; 
        //ruby_script("embedded"); 
        //rb_load_protect(rb_str_new2([script UTF8String]), 0, &status);
        //if (status == 0) { 
        // ruby_exec();
        //}
        NSDictionary* testSend = [NSDictionary dictionaryWithObject:filename forKey:@"script"];
        //[[NSNotificationCenter defaultCenter] postNotificationName:kRBonLoad object:testSend];
        [self fireCallback:kRBonLoad content:testSend];
    } 
    return self;
}

-(void)unloadScript {
    [self release];
}


// Callback related
-(void)CallbackCenter:(NSNotification*)notification {
    NSDictionary* cObj = notification.object;
    NSAssert([cObj valueForKey:kCBType], @"Invalid type specified");
    if ([[cObj valueForKey:kCBType] isEqualTo:kCBTypeRegister] && [[cObj valueForKey:kCBFileName] isEqualTo:filename]) {
        LOG(@"Registering a new callback! (%@.%@[%@] <%@>)", [cObj valueForKey:kCBKlassDef], [cObj valueForKey:kCBMethodDef], [cObj valueForKey:kCBCallbackType], [NSString stringWithUTF8String:rbfile]);
        //NSDictionary* klassmeth = [NSDictionary dictionaryWithObjectsAndKeys:[cObj valueForKey:kCBMethodDef] forKey:kCBMethodDef]
        //[registeredCallbacks setObject:[cObj valueForKey:kCBMethodDef] forKey:[cObj valueForKey:kCBCallbackType]];
        [registeredCallbacks setObject:cObj forKey:[cObj valueForKey:kCBCallbackType]];
    }
}

-(void)fireCallback:(NSString*)callbackType content:(NSDictionary*)content {
    for (NSString* key in registeredCallbacks) {
        if ([key isEqualToString:callbackType] && [[[registeredCallbacks valueForKey:key] valueForKey:kCBFileName] isEqualToString:filename]) {
            LOG(@"Firing (on file %@) '%@'",filename, [[registeredCallbacks valueForKey:key] valueForKey:kCBMethodDef]);
            ID funct = rb_intern([[[registeredCallbacks valueForKey:key] valueForKey:kCBMethodDef] UTF8String]);
            VALUE class = rb_gv_get([[[registeredCallbacks valueForKey:key] valueForKey:kCBKlassDef] UTF8String]);
            rb_funcall(class, funct, 1, [content toRubyHash]);
            
            if ([[[registeredCallbacks valueForKey:key] valueForKey:kCBCallbackType] isEqualToString:kRBonLoad])
            {
                LOG(@"Removing %@.%@(%@) callback for onLoad as its onehit", [[registeredCallbacks valueForKey:key] valueForKey:kCBKlassDef], [[registeredCallbacks valueForKey:key] valueForKey:kCBMethodDef],filename);
                // loop+mutating = bad
                //[registeredCallbacks removeObjectForKey:kRBonLoad];
                //[[NSNotificationCenter defaultCenter] removeObserver:self forKeyPath:kRBonLoad];
            }
            // Should probably handle the above and error check, meh.
        }
    }
}

-(void)callbackHandler:(NSNotification*)notification {
    [self fireCallback:notification.name content:notification.object];
}

#pragma mark -
#pragma mark OBJECTIVE-C Static methods
// Basically fire notifications from here and receive them in the object above
+(void)fireEvent:(NSString*)event content:(NSDictionary*)content {
    [[NSNotificationCenter defaultCenter] postNotificationName:event object:content];
}

@end
