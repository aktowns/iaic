//
//  ChanTabView.h
//  iaic
//
//  Created by Ashley Towns on 11/07/10.
//  Copyright 2010 Mogeneration. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <WebKit/WebKit.h>
#import <WebKit/WebFrameLoadDelegate.h>
#import <ChatCore/ChatCore.h>
#import "lazy.h"
#import "ChanWindowController.h"
#import "NSString+XMLEntities.h"
#import "NSString+ByteConversion.h"
#import "MiscRubyAdditions.h"

@interface ChanTabViewController : NSViewController <NSTableViewDelegate, NSTableViewDataSource, NSTextFieldDelegate> {
    id _parent;
    IBOutlet WebView* content;
    IBOutlet NSTableView* userList;
    IBOutlet NSTextField* entryText;
    MVChatRoom* myRoom;
    MVChatConnection* session;
    NSMutableString* webContentbuffer;
    NSMutableArray* nicklist;
    NSString* myChannel;
    NSMutableArray* textBuffer;
    NSUInteger textBufferOffset;
}
UB WebView* content;
UB NSTableView* userList;
UB NSTextField* entryText;
@property (nonatomic, retain) id _parent;
- (id)initWithMyRoom:(MVChatRoom*)chatroom andSession:(MVChatConnection*)sess;
- (void)writeLine:(NSString*)content;
- (void)_writeLine:(NSString*)content;
- (void)resyncUserList;
- (BOOL)isMyChannel:(NSNotification *)notification;
@end
