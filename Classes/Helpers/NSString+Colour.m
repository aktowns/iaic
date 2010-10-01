//
//  NSString+Colour.m
//  iaic
//
//  Created by Ashley Towns on 13/07/10.
//  Copyright 2010 Mogeneration. All rights reserved.
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
// Usage: [someString toColour];
@implementation NSString_Colour
-(NSString*)toColour {
    // Ultimately i should set safe zones to stay in, compared to the background css but thats fucking effort
    
}
@end
