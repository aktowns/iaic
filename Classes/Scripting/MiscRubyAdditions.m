//
//  MiscRubyAdditions.m
//  iaic
//
//  Created by Ashley Towns on 12/07/10.
//

#import "MiscRubyAdditions.h"

@implementation NSString (MiscRubyAdditions)
- (VALUE) toRubyString {
    return rbStr([self UTF8String]);   
}
+ (NSString*) fromRubyString:(VALUE)rubystr {
    return [NSString stringWithUTF8String:STR2CSTR(rubystr)];
}
@end

@implementation NSDictionary (MiscRubyAdditions)
- (VALUE) toRubyHash {
    VALUE hash = rb_hash_new();
    for (NSString* key in self) {
        rb_hash_aset(hash, ID2SYM(rb_intern([key UTF8String])), rb_str_new2([[self objectForKey:key] UTF8String]));
    }
    return hash;
}

/* cbf atm
VALUE i_each(VALUE elem, VALUE *obj)
{
    printf("%d\n", FIX2INT(elem));
    return Qnil;
}

+ (NSDictionary*) fromRubyHash:(VALUE)rubyhash {
    NSMutableDictionary* dict = [NSMutableDictionary dictionaryWithCapacity:0];


    rb_iterate(rb_each, rubyhash, i_each, (VALUE) obj);
}*/
@end

