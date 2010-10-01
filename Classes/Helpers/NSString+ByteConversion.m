//
//  NSString+ByteConversion.m
//  iaic
//
//  Created by Ashley Towns on 11/07/10.
//

#import "NSString+ByteConversion.h"

@implementation NSString (ByteConversion) // Need to change the name to reflect other changes

- (NSString*)asTimestamp {
    return [NSString stringWithFormat:@"<span style='background-color: #e0e0e0;'><span style=\"color:#4c4c4c\">[</span>%@<span style='color:#4c4c4c'>]</span></span>", self];
}

+ (NSString*)stringAsStatus:(NSString*)content {//width: 100%;  line-height:130%%; #d0d0d0 / c
    return [NSString stringWithFormat:@"<span style='background-color: #e0e0e0; display: inline-block;'>&nbsp;%@&nbsp;</span> <br />", content];
}

- (NSString*)asUser {
    return [NSString stringWithFormat:@"<span style='color: #666688'>%@</span>", self];
}

- (NSString*)asChannel {
    return [NSString stringWithFormat:@"<span style='color: #668866'>%@</span>", self];
}

+ (NSString*)stringOrNilFromData:(NSData*)data {
    if (data  == nil) 
        return nil;
    else {
        NSFont *baseFont = [[NSFontManager sharedFontManager] fontWithFamily:@"Menlo" traits:( NSUnboldFontMask | NSUnitalicFontMask ) weight:5 size:12.0f];
        if( ! baseFont ) baseFont = [NSFont userFontOfSize:12.];
        NSMutableDictionary *options = [NSMutableDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithUnsignedLong:NSUTF8StringEncoding], @"StringEncoding", [NSNumber numberWithBool:[[NSUserDefaults standardUserDefaults] boolForKey:@"JVChatStripMessageColors"]], @"IgnoreFontColors", [NSNumber numberWithBool:[[NSUserDefaults standardUserDefaults] boolForKey:@"JVChatStripMessageFormatting"]], @"IgnoreFontTraits", baseFont, @"BaseFont", nil];
        NSTextStorage *messageString = [NSTextStorage attributedStringWithChatFormat:data options:options];
        
        if (!messageString) {
            [options setObject:[NSNumber numberWithUnsignedLong:NSISOLatin1StringEncoding] forKey:@"StringEncoding"];
            messageString = [NSMutableAttributedString attributedStringWithChatFormat:data options:options];
            
            NSMutableDictionary *attributes = [NSMutableDictionary dictionaryWithObjectsAndKeys:baseFont, NSFontAttributeName, nil];
            NSMutableAttributedString *error = [[[NSMutableAttributedString alloc] initWithString:[@" " stringByAppendingString:NSLocalizedString( @"incompatible encoding", "encoding of the message different than your current encoding" )] attributes:attributes] autorelease];
            [error addAttribute:@"CSSClasses" value:[NSSet setWithObjects:@"error", @"encoding", nil] range:NSMakeRange( 1, ( [error length] - 1 ) )];
            [messageString appendAttributedString:error];
        }
        
        // The message needs to be able to convert to UTF8, otherwise we can't display it.
        if( ! [messageString length] || ! [[messageString string] UTF8String] ) {
            NSMutableDictionary *attributes = [NSMutableDictionary dictionaryWithObjectsAndKeys:baseFont, NSFontAttributeName, [NSSet setWithObjects:@"error", @"encoding", nil], @"CSSClasses", nil];
            messageString = [[[NSTextStorage alloc] initWithString:NSLocalizedString( @"incompatible encoding", "encoding of the message different than your current encoding" ) attributes:attributes] autorelease];
        }
        return [messageString string];
    }
    return nil;
}
@end
@implementation NSString (SearchingAdditions)

- (BOOL)containsString:(NSString *)aString {
    return [self containsString:aString ignoringCase:NO];
}

- (BOOL)containsString:(NSString *)aString ignoringCase:(BOOL)flag {
    unsigned mask = (flag ? NSCaseInsensitiveSearch : 0);
    return [self rangeOfString:aString options:mask].length > 0;
}

@end
