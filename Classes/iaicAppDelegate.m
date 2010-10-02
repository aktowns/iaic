//
//  iaicAppDelegate.m
//  iaic
//
//  Created by Ashley Towns on 11/07/10.
//

#import "iaicAppDelegate.h"
#import "ChanWindowController.h"

@implementation iaicAppDelegate
- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    LOG(@"Finished loading..");
    ChanWindowController* cwc = [[ChanWindowController alloc] init];
    [cwc showWindow:self];
}
@end
