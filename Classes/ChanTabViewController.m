//
//  ChanTabView.m
//  iaic
//
//  Created by Ashley Towns on 11/07/10.
//

#import "ChanTabViewController.h"
/*
 extern NSString *MVChatRoomMemberQuietedFeature;
 extern NSString *MVChatRoomMemberVoicedFeature;
 extern NSString *MVChatRoomMemberHalfOperatorFeature;
 extern NSString *MVChatRoomMemberOperatorFeature;
 extern NSString *MVChatRoomMemberAdministratorFeature;
 extern NSString *MVChatRoomMemberFounderFeature;
 
 extern NSString *MVChatRoomJoinedNotification;
 extern NSString *MVChatRoomPartedNotification;
 extern NSString *MVChatRoomKickedNotification;
 extern NSString *MVChatRoomInvitedNotification;
 
 extern NSString *MVChatRoomMemberUsersSyncedNotification;
 extern NSString *MVChatRoomBannedUsersSyncedNotification;
 
 extern NSString *MVChatRoomUserJoinedNotification;
 extern NSString *MVChatRoomUserPartedNotification;
 extern NSString *MVChatRoomUserKickedNotification;
 extern NSString *MVChatRoomUserBannedNotification;
 extern NSString *MVChatRoomUserBanRemovedNotification;
 extern NSString *MVChatRoomUserModeChangedNotification;
 extern NSString *MVChatRoomUserBrickedNotification;
 
 extern NSString *MVChatRoomGotMessageNotification;
 extern NSString *MVChatRoomTopicChangedNotification;
 extern NSString *MVChatRoomModesChangedNotification;
 extern NSString *MVChatRoomAttributeUpdatedNotification;
*/

@implementation ChanTabViewController
@synthesize content, entryText, userList, _parent;

- (id)initWithMyRoom:(MVChatRoom*)chatroom andSession:(MVChatConnection*)sess {
    self = [super initWithNibName:@"ChanTabView" bundle:nil];
    if (self) {
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(scriptCommand:) name:kCBScriptCommand object:nil];

        
        // Notifications
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(roomGotMessage:) name:MVChatRoomGotMessageNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(joinedRoom:) name:MVChatRoomJoinedNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(userJoined:) name:MVChatRoomUserJoinedNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(userParted:) name:MVChatRoomUserPartedNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(userKicked:) name:MVChatRoomUserKickedNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(userBanned:) name:MVChatRoomUserBannedNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(userBanRemoved:) name:MVChatRoomUserBanRemovedNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(userModeChange:) name:MVChatRoomUserModeChangedNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(userBricked:) name:MVChatRoomUserBrickedNotification object:nil];
        
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(nicknameAccepted:) name:MVChatConnectionNicknameAcceptedNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(nicknameDenied:) name:MVChatConnectionNicknameRejectedNotification object:nil];
        
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(userNickChange:) name:MVChatUserNicknameChangedNotification object:nil];
        
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(rawMessage:) name:MVChatConnectionGotRawMessageNotification object:nil];
        //
        myRoom = chatroom;
        session = sess;
        myChannel = chatroom.name;
        textBuffer = [[NSMutableArray arrayWithCapacity:0] retain];
        [textBuffer addObject:@""];
        textBufferOffset = -1;
        
        NSLog(@"ChanTabView initialized");
    }
    return self;
}

-(void)awakeFromNib {
    userList.delegate = self;
    userList.dataSource = self;
    entryText.delegate = self;
    [content setFrameLoadDelegate:self];
    /*chatText { //FONT:#4c4c4c
     color: #666666;
     background-color: #e6e6e6;
     font-size: 12px;
     font-family: menlo;
     }*/
    //nicklist = [[NSMutableArray arrayWithArray:[myRoom.memberUsers allObjects]] retain];
}

#pragma mark Scripting
-(void)scriptCommand:(NSNotification*)notification {
    NSDictionary* payload = notification.object;
    if ([[payload valueForKey:kCBScriptCommandType] isEqualToString:kCBScriptCommandWriteLine] && 
        [[payload valueForKey:kCBScriptCommandPayloadArgs1] isEqualToString:myChannel]) 
        NSLog(@"WriteLine script command fired");
        [self writeLine:[[payload valueForKey:kCBScriptCommandPayloadArgs0] stringByEncodingXMLEntities]];
}

#pragma mark IRC notifications
- (void)rawMessage:(NSNotification*)notification {
    //NSString* raw = [NSString stringOrNilFromData:[notification.userInfo objectForKey:@"messageData"]];
    //if (raw != nil)
    //    NSLog(@"%@", raw);
}

- (void)roomGotMessage:(NSNotification*)notification {
    NSData* message = [[notification userInfo] objectForKey:@"message"];
    NSString* user = [[notification userInfo] objectForKey:@"user"];
    //NSString* room = [[notification userInfo] objectForKey:@"room"];
    NSParameterAssert( message != nil );
	NSParameterAssert( user != nil );
    
    MVChatRoom* chat = notification.object;
    if ([chat.name isEqualToString:myChannel]) {
        /*
        NSFont *baseFont = [[NSFontManager sharedFontManager] fontWithFamily:@"Menlo" traits:( NSUnboldFontMask | NSUnitalicFontMask ) weight:5 size:12.0f];
        if( ! baseFont ) baseFont = [NSFont userFontOfSize:12.];
        NSMutableDictionary *options = [NSMutableDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithUnsignedLong:NSUTF8StringEncoding], @"StringEncoding", [NSNumber numberWithBool:[[NSUserDefaults standardUserDefaults] boolForKey:@"JVChatStripMessageColors"]], @"IgnoreFontColors", [NSNumber numberWithBool:[[NSUserDefaults standardUserDefaults] boolForKey:@"JVChatStripMessageFormatting"]], @"IgnoreFontTraits", baseFont, @"BaseFont", nil];
        NSTextStorage *messageString = [NSTextStorage attributedStringWithChatFormat:message options:options];
        
        if (!messageString) {
            [options setObject:[NSNumber numberWithUnsignedLong:NSISOLatin1StringEncoding] forKey:@"StringEncoding"];
            messageString = [NSMutableAttributedString attributedStringWithChatFormat:message options:options];
            
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
         */
        NSString* _message = [NSString stringOrNilFromData:message];
        if (_message == nil) 
            return;
        _message = [_message stringByEncodingXMLEntities];
        [self writeLine:[NSString stringWithFormat:@"&lt;<span style='color:#ff8000'>%@</span>&gt; %@", user, _message]];
    }
}

-(void)joinedRoom:(NSNotification*)notification {
    MVChatRoom* room = notification.object;
    if ([room.name isEqualToString:myChannel]) {
        NSLog(@"Really joined %@ (rec: %@)", room.name, myChannel);
        //NSString* htmlStatusFormat = @"<span style='width: 100\%; background-color: #c0c0c0;'>&nbsp;%@&nbsp;</span> <br />";
        NSString* topicHtml = [NSString stringOrNilFromData:myRoom.topic];
        if (topicHtml != nil)
            topicHtml = [NSString stringAsStatus:[NSString stringWithFormat:@"<b>Topic for</b> %@<b>:</b> %@", [myChannel asChannel], topicHtml]];
        else 
            topicHtml = [NSString stringAsStatus:[NSString stringWithFormat:@"%@ <b>Has no topic.</b>", [myChannel asChannel]]];
        NSString* joinHtml = [NSString stringAsStatus:[NSString stringWithFormat:@"%@ <b>has joined</b> %@", [session.nickname asUser], [myChannel asChannel]]];
        
        webContentbuffer = [[NSMutableString stringWithString:[NSString stringWithFormat:@"<html><head><style>"
                                                               @"body{ color: #666666; background-color: #e6e6e6;"
                                                               @"font-size: 12px; font-family: menlo; }" //line-height: 140%%;
                                                               @"</style><script>window.location='#bottom'</script></head><body>%@%@",topicHtml, joinHtml]] retain];
        NSLog(@"%@", webContentbuffer);
        [[content mainFrame] loadHTMLString:webContentbuffer baseURL:nil];
        nicklist = [[NSMutableArray arrayWithArray:[room.memberUsers allObjects]] retain];
        [userList reloadData];
        [_parent setTitle:[NSString stringWithFormat:@"%@ - %@", room.name, [NSString stringWithUTF8String:[room.topic bytes]]]];
    }
}

-(void)nicknameAccepted:(NSNotification*)notification {
    //NSLog(@"Nickname accepted! %@ - %@", notification.object, notification.userInfo);
    [self _writeLine:[NSString stringAsStatus:[NSString stringWithFormat:@"<b>You are now known as</b> %@", [session.nickname asUser]]]];
}
-(void)nicknameDenied:(NSNotification*)notification {
    NSLog(@"Nickname denied! %@ - %@", notification.object, notification.userInfo);
}
-(void)userJoined:(NSNotification*)notification {
    //NSLog(@"%@", [notification userInfo]);
    if ([self isMyChannel:notification]) {
        MVChatUser* user = [[notification userInfo] objectForKey:@"user"];
        NSString* longUser = [NSString stringWithFormat:@"%@!%@@%@", [user.nickname asUser], user.username, user.address];
        [self _writeLine:[NSString stringAsStatus:[NSString stringWithFormat:@"%@ <b>has joined</b> %@", longUser, [myChannel asChannel]]]];
        NSDictionary* payload = [NSDictionary dictionaryWithObjectsAndKeys:
                                 user.nickname,@"nick",
                                 user.username,@"user",
                                 user.address,@"host",
                                 myChannel,@"chan",
                                nil];
        [[NSNotificationCenter defaultCenter] postNotificationName:kRBonJoin object:payload];
        [self resyncUserList];
    }
}
-(void)userParted:(NSNotification*)notification {
    if ([self isMyChannel:notification]) {
        MVChatUser* user = [[notification userInfo] objectForKey:@"user"];
        NSString* longUser = [NSString stringWithFormat:@"%@!%@@%@", [user.nickname asUser], user.username, user.address];
        [self _writeLine:[NSString stringAsStatus:[NSString stringWithFormat:@"%@ <b>has left</b> %@", longUser, [myChannel asChannel]]]];
        [self resyncUserList];
    }
}
-(void)userKicked:(NSNotification*)notification {
    if ([self isMyChannel:notification]) {
        [self resyncUserList];
    }
}
-(void)userBanned:(NSNotification*)notification {
    if ([self isMyChannel:notification]) {
        [self resyncUserList];
    }
}
-(void)userBanRemoved:(NSNotification*)notification {
    if ([self isMyChannel:notification]) {
        //[self resyncUserList];
    }
}
-(void)userModeChange:(NSNotification*)notification {
    if ([self isMyChannel:notification]) {
        [self resyncUserList];
    }
}
-(void)userBricked:(NSNotification*)notification {
    if ([self isMyChannel:notification]) {
        //[self resyncUserList];
    }
}


-(void)userNickChange:(NSNotification*)notification {
    NSLog(@"%@ - %@", [notification userInfo], notification.object);
    MVChatUser* user = notification.object;
    if ([[myRoom.memberUsers allObjects] containsObject:user]) {
        NSString* oldName = [notification.userInfo objectForKey:@"oldNickname"];
        [self _writeLine:[NSString stringAsStatus:[NSString stringWithFormat:@"%@ <b>has changed nickname to</b> %@", [oldName asUser], [user.nickname asUser]]]];
        [self resyncUserList];
    }
}

-(void)resyncUserList {
    [userList reloadData];
//    nicklist = myRoom.memberUsers;
}

-(BOOL)isMyChannel:(NSNotification*)notification {
    MVChatRoom* room = notification.object;
    if ([room.name isEqualToString:myChannel])
        return YES;
    return NO;
}

#pragma mark NSTextField methods

- (NSString*)getNickFor:(NSString*)key {
    NSArray* users = [myRoom.memberUsers allObjects];
    for (MVChatUser* user in users) {
        if ([[user.nickname lowercaseString] hasPrefix:[key lowercaseString]]) {
            return [NSString stringWithFormat:@"%@ ",user.nickname];
        }
    }
    return nil;
}

- (NSString*)getNickFor:(NSString *)key withOffset:(NSUInteger)offset {
    NSArray* users = [myRoom.memberUsers allObjects];
    int i = 0;
    for (MVChatUser* user in users) {
        if ([[user.nickname lowercaseString] hasPrefix:[key lowercaseString]]) {
            i = i+1;
            if (i >= offset)
                return [NSString stringWithFormat:@"%@ ",user.nickname];
        }
    }
    return nil; 
}

- (BOOL)control:(NSControl*)control textView:(NSTextView*)textView doCommandBySelector:(SEL)commandSelector {
    BOOL result = NO;
    //NSLog(@"CommandSelector: %@", NSStringFromSelector(commandSelector));
    if (commandSelector == @selector(insertNewline:)) {
        if (![[entryText stringValue] isEqualToString:@""] && webContentbuffer != nil) {
            if ([[entryText stringValue] hasPrefix:@"/"]) {
                NSString* rawCmd = [[entryText stringValue] substringFromIndex:1];
                if ([rawCmd hasPrefix:@"say"]) {
                    NSString* text = [[entryText stringValue] substringFromIndex:4];
                    if ([rawCmd containsString:@"$" ignoringCase:YES]) {
                        NSArray* tokens = [[[entryText stringValue] substringFromIndex:4] componentsSeparatedByString:@" "];
                        NSMutableString* newtext = [[NSMutableString alloc] init];
                        for (NSString* x in tokens) {
                            if ([x hasPrefix:@"$"]) {
                                int retcode;
                                VALUE outcome = rb_eval_string_protect([x UTF8String], &retcode);
                                if (retcode == 0 && outcome != Qnil) {
                                    x = [NSString fromRubyString:outcome];
                                }
                            }
                            [newtext appendFormat:@"%@ ",x];
                        }
                        text = newtext;
                    } 
                    [myRoom sendMessage:[[NSAttributedString alloc] initWithString:text] asAction:NO];
                    [self writeLine:[NSString stringWithFormat:@"&lt;<span style=\"color: #999999\">%@</span>&gt; %@",[session nickname], text]];
                } else if ([rawCmd hasPrefix:@"echo"]) {
                    NSString* text = [[entryText stringValue] substringFromIndex:5];
                    if ([rawCmd containsString:@"$" ignoringCase:YES]) {
                        NSArray* tokens = [[[entryText stringValue] substringFromIndex:5] componentsSeparatedByString:@" "];
                        NSMutableString* newtext = [[NSMutableString alloc] init];
                        for (NSString* x in tokens) {
                            if ([x hasPrefix:@"$"]) {
                                int retcode;
                                VALUE outcome = rb_eval_string_protect([x UTF8String], &retcode);
                                if (retcode == 0 && outcome != Qnil) {
                                    x = [NSString fromRubyString:outcome];
                                }
                            }
                            [newtext appendFormat:@"%@",x];
                        }
                        text = newtext;
                    } 
                    [self writeLine:[NSString stringWithFormat:@"%@", text]];
                } else if ([rawCmd hasPrefix:@"set"]) {
                    NSArray* tokens = [[[entryText stringValue] substringFromIndex:5] componentsSeparatedByString:@" "];
                    int retcode;
                    if ([rawCmd containsString:@"$" ignoringCase:YES]) {
                        if ([[tokens objectAtIndex:0] hasPrefix:@"$"]) {
                            if ([tokens count] > 1) {
                                NSString* value = [rawCmd substringFromIndex:[(NSString*)[tokens objectAtIndex:0] length]+5];
                                NSString* setCode = [NSString stringWithFormat:@"%@ = \"%@\"", [tokens objectAtIndex:0], value];
                                rb_eval_string_protect([setCode UTF8String], &retcode);
                            }
                        }
                    }
                    NSString* response;
                    if (retcode == 0)
                        response = [NSString stringWithFormat:@"Successfully set '%@'", [tokens objectAtIndex:0]];
                    else 
                        response = [NSString stringWithFormat:@"Failed to set variable '%@'", [tokens objectAtIndex:0]];
                    [self writeLine:[NSString stringWithFormat:@"%@", response]];
                } else {
                    [session sendRawMessage:rawCmd];
                }
            } else {
                [myRoom sendMessage:[[NSAttributedString alloc] initWithString:[entryText stringValue]] asAction:NO];
                [self writeLine:[NSString stringWithFormat:@"&lt;<span style=\"color: #999999\">%@</span>&gt; %@",[session nickname], [entryText stringValue]]];
            }
            textBufferOffset = 0;
            [textBuffer addObject:[entryText stringValue]];
            [entryText setStringValue:@""];
        }
        result = YES;
    } 
    else if (commandSelector == @selector(insertTab:)) {
        NSMutableArray* words = [NSMutableArray arrayWithArray:[[entryText stringValue] componentsSeparatedByString:@" "]];
        NSString* lastWord = [words objectAtIndex:[words count]-1];
        //NSLog(@"Lastword is %@", lastWord);
        if ([self getNickFor:lastWord] != nil) {
            [words removeLastObject];
            [words addObject:[self getNickFor:lastWord]];
            
            NSString* output = [words componentsJoinedByString:@" "];
            if ([words count] <= 1)
                output = [NSString stringWithFormat:@"%@: ",output];
            [entryText setStringValue:output];
        }
        result = YES;
    } else if (commandSelector == @selector(moveUp:)) {
        if (textBufferOffset <= [textBuffer count])
            textBufferOffset += 1;
        if ([textBuffer objectAtIndex:textBufferOffset] != nil && ![[textBuffer objectAtIndex:textBufferOffset] isEqualToString:@""]) {
            [entryText setStringValue:[textBuffer objectAtIndex:([textBuffer count]-textBufferOffset)]];
        }
    } else if (commandSelector == @selector(moveDown:)) {
        if (!textBufferOffset <= 0) 
            textBufferOffset -= 1;
        if ([textBuffer objectAtIndex:textBufferOffset] != nil && ![[textBuffer objectAtIndex:textBufferOffset] isEqualToString:@""])
            [entryText setStringValue:[textBuffer objectAtIndex:([textBuffer count]-textBufferOffset)]];
    }
    return result;
}
//<a name="bottom"> at bottom of page, then window.location='#bottom' .

#pragma mark helpers
- (void)writeLine:(NSString *)_content {
    NSLog(@"Writing '%@' to the content buffer", _content);
    NSDateFormatter *timeFormatter = [[[NSDateFormatter alloc] init] autorelease];
	[timeFormatter setDateStyle:NSDateFormatterNoStyle];
	[timeFormatter setTimeStyle:NSDateFormatterMediumStyle];
    NSDate *stringTime = [NSDate date];
	NSString *timestamp = [timeFormatter stringFromDate:stringTime];
    webContentbuffer = [[NSMutableString stringWithString:[webContentbuffer stringByAppendingFormat:@"%@ %@<br />",[timestamp asTimestamp], _content]] retain];
    [[content mainFrame] loadHTMLString:[NSString stringWithFormat:@"%@<a name=\"bottom\">&nbsp;</a>",webContentbuffer] baseURL:nil];
}
- (void)_writeLine:(NSString *)_content {
    NSLog(@"Writing '%@' to the content buffer", _content);
    webContentbuffer = [[NSMutableString stringWithString:[webContentbuffer stringByAppendingFormat:@"%@", _content]] retain];
    [[content mainFrame] loadHTMLString:[NSString stringWithFormat:@"%@<a name=\"bottom\">&nbsp;</a>",webContentbuffer] baseURL:nil];
}

#pragma mark WebView methods
- (void)webView:(WebView *)sender didFinishLoadForFrame:(WebFrame*)frame {
    if(frame == [sender mainFrame]) {
        [content scrollToEndOfDocument:self];
    }
}


#pragma mark tableView methods
- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    //id theRecord, theValue;
    
    //NSParameterAssert(rowIndex >= 0 && rowIndex < [nicklist count]);
    // UserClass *newClass = [nicklist objectAtIndex:rowIndex];
    //theRecord = newClass.Nickname;
    //theValue = @"nick";//[theRecord objectForKey:[aTableColumn identifier]];
    //NSLog(@"Showing row: %i(/%i)", rowIndex, [nicklist count]);
    //NSCell *testCell = [[NSCell alloc] initTextCell:[[nicklist objectAtIndex:rowIndex] Nickname]];
    //NSCell *testCell = [[NSCell alloc] initTextCell:[[nicklist objectAtIndex:rowIndex] nickname]];
    NSCell *testCell = [[NSCell alloc] initTextCell:[[[myRoom.memberUsers allObjects] objectAtIndex:rowIndex] nickname]];
    return testCell;
}
/*
 - (void)tableView:(NSTableView *)aTableView setObjectValue:anObject forTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
 {
 id theRecord;
 
 NSParameterAssert(rowIndex >= 0 && rowIndex < [nicklist count]);
 //theRecord = [records objectAtIndex:rowIndex];
 //[theRecord setObject:anObject forKey:[aTableColumn identifier]];
 return;
 }*/

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
    return [[myRoom.memberUsers allObjects] count];
}
@end
