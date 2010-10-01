//
//  MiscRubyAdditions.h
//  iaic
//
//  Created by Ashley Towns on 12/07/10.
//

#import <Cocoa/Cocoa.h>
#import <Ruby/Ruby.h>
#define rbStr(str) rb_str_new2(str)
@interface NSString (MiscRubyAdditions) 


-(VALUE)toRubyString;
+ (NSString*) fromRubyString:(VALUE)rubystr;
@end

@interface NSDictionary (MiscRubyAdditions) 
-(VALUE)toRubyHash;
@end