//
//  NSString+ByteConversion.h
//  iaic
//
//  Created by Ashley Towns on 11/07/10.
//  Copyright 2010 Mogeneration. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <ChatCore/ChatCore.h>
#import "NSTextStorageAdditions.h"

@interface NSString (ByteConversion)
+(NSString*)stringOrNilFromData:(NSData*)data;
+(NSString*)stringAsStatus:(NSString*)content;
-(NSString*)asChannel;
-(NSString*)asUser;
-(NSString*)asTimestamp;
@end

@interface NSString (SearchingAdditions)
- (BOOL) containsString:(NSString *)aString;
- (BOOL) containsString:(NSString *)aString ignoringCase:(BOOL)flag;
@end