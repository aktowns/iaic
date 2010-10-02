//
//  iaicAppDelegate.m
//  iaic
//
//  Created by Ashley Towns on 11/07/10.
//

#import "iaicAppDelegate.h"
#import "ChanWindowController.h"

@implementation iaicAppDelegate

//@synthesize window;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    NSLog(@"Finished loading..");
    	// Insert code here to initialize your application 
        //ChanWindowController* cwc = [[ChanWindowController alloc] initWithWindowNibName:@"ChanWindow"];
        //    [cwc showWindow:self];
    ChanWindowController* cwc = [[ChanWindowController alloc] init];
    [cwc showWindow:self];
    //[window setIsVisible:NO];
}


@end
