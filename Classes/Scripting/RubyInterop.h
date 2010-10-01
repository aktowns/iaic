//
//  RubyInterop.h
//  iaic
//
//  Created by Ashley Towns on 12/07/10.
//

#import <Cocoa/Cocoa.h>
#import <Ruby/Ruby.h>
#import "MiscRubyAdditions.h"
#import "lazy.h"

#define kRbTest @"_kkRbTest"
#define kRbCallbackManager_cbCenter @"_kRbCallbackManager_cbCenter"

#define kCBTypeRegister @"_kCBTypeRegister"
#define kCBTypeUnregister @"_kCBTypeUnregister"
#define kCBTypeFire @"_kCBTypeFire"

#define kCBFileName @"_kCBFileName"
#define kCBCallbackType @"_kCBCallbackType"
#define kCBMethodDef @"_kCBMethodDef"
#define kCBKlassDef @"_kCBKlassDef"
#define kCBType @"_kCbType"

#define kRBonLoad @"_kRBonLoad"
#define kRBonJoin @"_kRBonJoin"
#define kRBonPart @"_kRBonPart"
#define kRBonKick @"_kRBonKick"
#define kRBonNick @"_kRBonNick"
#define kRBonQuit @"_kRBonQuit"
#define kRBonUnload @"_kRBonUnload"
#define kRBonChanMsg @"_kRBonChanMsg"

#define kCBScriptCommand @"_kCBScriptCommand"
#define kCBScriptCommandType @"_kCBScriptCommandType"
#define kCBScriptCommandWriteLine @"kCBScriptCommandWriteLine"
#define kCBScriptCommandPayloadArgs0 @"kCBScriptCommandPayloadArgs0"
#define kCBScriptCommandPayloadArgs1 @"kCBScriptCommandPayloadArgs1"
#define kCBScriptCommandPayloadArgs2 @"kCBScriptCommandPayloadArgs2"
#define kCBScriptCommandPayloadArgs3 @"kCBScriptCommandPayloadArgs3"


@interface RubyInterop : NSObject {
    NSMutableDictionary *registeredCallbacks;
    NSString *filename;
}

PROP NSMutableDictionary *registeredCallbacks;

-(id)initWithScript:(NSString*)script;
+ (void)ruby_initialize;
-(void)fireCallback:(NSString*)callbackType content:(NSDictionary*)content;
@end
