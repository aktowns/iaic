//
//  NSString+Colour.m
//  iaic
//
//  Created by Ashley Towns on 13/07/10.
//

#import "NSString+Colour.h"

// I shall call this the micolous colour alogorithm
/*
  micolous: hash the nickname
  micolous: take two bytes from it
  micolous: byte 1 == hue
  micolous: byte 2 == saturation
  micolous: value = 50%
  micolous: have fun
*/
// Usage: [someString toHashedColour];
@implementation NSString (Colour)
-(NSString*)FNVhash:(NSString*)str {
	// A Fowler-Noll-vo hash! (FNV-1a)
	// http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-1a
	const char* strptr=[[str uppercaseString] UTF8String];
	int32_t y=2166136261U,x=0,p=16777619;
	while (x != [str length]) {
		y^=strptr[x];
		y*=p;
		x++;
	}
	return [NSString stringWithFormat:@"%lld",y];
}
-(NSString*)toHashedColour {
	NSString* hash = [[self FNVhash:self] stringByReplacingOccurrencesOfString:@"0" withString:@""];
	CGFloat h=([[hash substringToIndex:1]floatValue]/9.0);
	CGFloat s=([[hash substringWithRange:NSMakeRange(2,1)]floatValue]/9.0);
	NSColor* colour = [NSColor colorWithCalibratedHue:h saturation:s brightness:0.5 alpha:1.0];
	// Ultimately i should set safe zones to stay in, compared to the background css but thats fucking effort
    return [NSString stringWithFormat:@"<span style='color: rgb(%.0f,%.0f,%.0f);'>%@</span>",
			([colour redComponent]*255.9999f),([colour greenComponent]*255.9999f),([colour blueComponent]*255.9999f),self];
}
@end
