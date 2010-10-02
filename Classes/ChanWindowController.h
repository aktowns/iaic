//
//  ChanWindowController.h
//  iaic
//
//  Created by Ashley Towns on 11/07/10.
//
#import <Cocoa/Cocoa.h>
#import <PSMTabBarControl.h>
#import <ChatCore/ChatCore.h>
#import "tabModel.h"
#import "lazy.h"
#import "ChanTabViewController.h"
#import "RubyInterop.h"
#import "HomeViewController.h"
#import "ScriptViewController.h"

@interface ChanWindowController : NSWindowController <NSTabViewDelegate> {
    IBOutlet PSMTabBarControl* tabBar;
    IBOutlet NSTabView* tabView;
    MVChatConnection* ircSession;
    NSMutableArray* channels;
}

PROP MVChatConnection* ircSession;
UB PSMTabBarControl* tabBar;
UB NSTabView* tabView;
//@property (nonatomic,retain) IBOutlet NSWindow *window;
-(void)setTitle:(NSString *)title;
-(void)createTabForChannel:(NSString *)channel;
-(void)closeTabForChannel:(NSString *)channel withMessage:(NSString *)message;
-(BOOL)isCurrentTab:(NSString *)channel;
-(NSTabViewItem*)tabViewForTabViewLabeled:(NSString *)label;
-(void)tabViewIncrementObjectforTabLabeled:(NSString *)label;
@end
