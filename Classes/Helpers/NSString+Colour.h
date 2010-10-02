//
//  NSString+Colour.h
//  iaic
//
//  Created by Ashley Towns on 13/07/10.
//

#import <Cocoa/Cocoa.h>
#include <openssl/sha.h>

// I shall call this the micolous colour alogorithm
@interface NSString (Colour)

-(NSString*)toHashedColour;

@end
