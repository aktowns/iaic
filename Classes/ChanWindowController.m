//
//  ChanWindowController.m
//  iaic
//
//  Created by Ashley Towns on 11/07/10.
//

#import "ChanWindowController.h"

@interface ChanWindowController (Private)
	-(void)tabBarSetup;
@end

@implementation ChanWindowController
@synthesize tabBar,tabView;
- (id)init
{
    self = [super initWithWindowNibName:@"ChanWindow"];
    if (self)
    {
        NSLog(@"ChanWindowController loaded");
        [tabBar setTabView:tabView];
        //initialize stuff
    }
    return self;
}


#pragma mark UI Related
#pragma mark -

- (void)windowWillClose:(NSNotification *)notification {
	//[ircSession sendRawMessage:@"quit objirc" immediately:YES];
	exit(0);
}

-(void)windowDidLoad {
    self.window.title = @"Ikes awkward irc client";
	tabBar.delegate = self;

    [self tabBarSetup];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(testCallback:) name:kRbTest object:nil];
    [RubyInterop ruby_initialize];
    //RubyInterop* ri = [[RubyInterop alloc] initWithScript:@"testScript.rb"];
    //RubyInterop* ri2 = [[RubyInterop alloc] initWithScript:@"testScript2.rb"];
	//NSLog(@"%@ ~ %@", ri, ri2);
    //NSLog(@"%@ - %@", tabBar, [tabBar tabView]);
    //ChanTabViewController* ctvc = [[ChanTabViewController alloc] init];
    //[[tabView tabViewItemAtIndex:0] setView:[ctvc view]];
    //[[tabView tabViewItemAtIndex:0] setLabel:@"test"];
    //[[tabView tabViewItemAtIndex:1] setLabel:@"baw"];
    ircSession = [[[MVChatConnection alloc] initWithServer:@"irc.streetgeek.com.au" type:MVChatConnectionIRCType port:6667 user:@"objircc"] retain];
    [ircSession connect];
    //NSString* mainChan = @"#streetgeek";
    //[ircSession joinChatRoomNamed:mainChan];
    //[ircSession joinChatRoomNamed:@"#test"];
    //[channels addObject:[[ircSession chatRoomWithName:mainChan] retain]];
    //[channels addObject:[[ircSession chatRoomWithName:@"#test"] retain]];
//    [self addNewTab:self objectCount:0];
	[self createTabForChannel:@"#test"];
    //[self addNewTab:self objectCount:0];
    //[self addNewTab:self objectCount:1];
//    [self addNewTab:self objectCount:0];
	/*
    [[tabView tabViewItemAtIndex:0] setLabel:@"Home"];
    HomeViewController* hvc = [[HomeViewController alloc] init];
    [[tabView tabViewItemAtIndex:0] setView:[hvc view]];
    [[tabView tabViewItemAtIndex:3] setLabel:@"Scripts"];
    ScriptViewController* svc = [[ScriptViewController alloc] init];
    [[tabView tabViewItemAtIndex:3] setView:[svc view]];*/
/*
    [[tabView tabViewItemAtIndex:1] setLabel:mainChan];
    ChanTabViewController* ctvc = [[ChanTabViewController alloc] 
                                   initWithMyRoom:[[ircSession chatRoomWithName:mainChan]retain]
                                   andSession:[ircSession retain]
                                  ];
    ctvc._parent = self;
    [[tabView tabViewItemAtIndex:1] setView:[ctvc view]];
    [[tabView tabViewItemAtIndex:2] setLabel:@"#test"];
    ChanTabViewController* ctvc2 = [[ChanTabViewController alloc] 
                                   initWithMyRoom:[[ircSession chatRoomWithName:@"#test"]retain]
                                   andSession:[ircSession retain]
                                   ];
    ctvc2._parent = self;
    [[tabView tabViewItemAtIndex:2] setView:[ctvc2 view]];
 */
    //[tabView selectTabViewItem:[tabView tabViewItemAtIndex:1]];
    //ChanTabViewController* ctvc = [[ChanTabViewController alloc] init];
    //[[tabView tabViewItemAtIndex:0] setView:[ctvc view]];
    //[[tabView tabViewItemAtIndex:1] setLabel:@"#test"];
    //[[tabView tabViewItemAtIndex:2] setLabel:@"@Plugins"];
    //ircChannel = [[ircSession chatRoomWithName:@"#main"] retain];
    //NSLog(@"Joined %@", gah.name);
    //NSAttributedString* wth = [[NSAttributedString alloc] initWithString:@"werd"];
    //[ircChannel sendMessage:wth asAction:YES];
}

-(void)setTitle:(NSString*) title {
    self.window.title = title;
}

#pragma mark tabView

-(IBAction)addNewTab:(id)sender objectCount:(NSUInteger)count {
	tabModel *newModel = [[tabModel alloc] init];
    [newModel setObjectCount: count];
	NSTabViewItem *newItem = [[[NSTabViewItem alloc] initWithIdentifier:newModel] autorelease];
	[newItem setLabel:@"Untitled"];
	[tabView addTabViewItem:newItem];
	//[tabView selectTabViewItem:newItem]; // this is optional, but expected behavior
	[newModel release];
}

-(void)tabBarSetup {
    [[tabBar addTabButton] setTarget:self];
	[[tabBar addTabButton] setAction:@selector(addNewTab:)];   
    
    [tabBar setTabView:tabView];
    
    NSArray *existingItems = [[tabBar tabView] tabViewItems];
	NSEnumerator *e = [existingItems objectEnumerator];
	NSTabViewItem *item;
	while(item = [e nextObject]) {
		[tabView removeTabViewItem:item];
	}
}

-(void)testCallback:(NSNotification*) notification {
    NSLog(@"yipeee");
}

-(void)tabViewIncrementObjectforTabLabeled:(NSString*)label {
	NSTabViewItem* tab = [self tabViewForTabViewLabeled:label];
	[[tab identifier] setObjectCount:[[tab identifier] objectCount]+1];
}

-(void)tabViewSetObjectCount:(int)count forTabLabeled:(NSString*)label {
	NSTabViewItem* tab = [self tabViewForTabViewLabeled:label];
	[[tab identifier] setObjectCount:count];
}

-(NSTabViewItem*)tabViewForTabViewLabeled:(NSString*)label {
	for (NSTabViewItem* tab in [tabView tabViewItems]) {
		if ([[tab label] isEqualToString:label])
			return tab;
	}
	return nil;
}

-(BOOL)isCurrentTab:(NSString*)channel {
	if ([[tabView selectedTabViewItem] identifier] == [[self tabViewForTabViewLabeled:channel] identifier]) {
		return YES;
	}
	return NO;
}

-(void)createTabForChannel:(NSString*)channel {
	[ircSession joinChatRoomNamed:channel];
	[channels addObject:[[ircSession chatRoomWithName:channel] retain]];
	[self addNewTab:self objectCount:0];
	[[tabView tabViewItemAtIndex:[[tabView tabViewItems] count]-1] setLabel:channel];
	ChanTabViewController* ctvc = [[ChanTabViewController alloc] 
                                   initWithMyRoom:[[ircSession chatRoomWithName:channel]retain]
                                   andSession:[ircSession retain]
								   ];
    ctvc._parent = self;
    [[tabView tabViewItemAtIndex:[[tabView tabViewItems] count]-1] setView:[ctvc view]];
}
-(void)closeTabForChannel:(NSString*)channel withMessage:(NSString*)message {
	NSLog(@"Leaving.. %@", channel);
	[channels removeObject:[ircSession chatRoomWithName:channel]];
	NSAttributedString* attrStr = [NSAttributedString attributedStringWithHTMLFragment:message baseURL:nil];
	[[ircSession chatRoomWithName:channel] partWithReason:attrStr];
	[tabView removeTabViewItem:[self tabViewForTabViewLabeled:channel]];
}

- (void)tabView:(NSTabView *)aTabView didSelectTabViewItem:(NSTabViewItem *)tabViewItem {
	[[tabViewItem identifier] setObjectCount:0];
}

- (IBAction)closeTab:(id)sender {
	NSLog(@"Closing tab..");
}

- (void)stopProcessing:(id)sender {
	[[[tabView selectedTabViewItem] identifier] setValue:[NSNumber numberWithBool:NO] forKeyPath:@"isProcessing"];
}

- (void)setIconNamed:(id)sender {
	//NSString *iconName = [sender titleOfSelectedItem];
    [[[tabView selectedTabViewItem] identifier] setValue:nil forKeyPath:@"icon"];
    [[[tabView selectedTabViewItem] identifier] setValue:@"None" forKeyPath:@"iconName"];
}
- (void)setObjectCount:(id)sender {
	[[[tabView selectedTabViewItem] identifier] setValue:[NSNumber numberWithInteger:[sender integerValue]] forKeyPath:@"objectCount"];
}

- (IBAction)isProcessingAction:(id)sender {
	[[[tabView selectedTabViewItem] identifier] setValue:[NSNumber numberWithBool:[sender state]] forKeyPath:@"isProcessing"];
}

- (IBAction)isEditedAction:(id)sender {
	[[[tabView selectedTabViewItem] identifier] setValue:[NSNumber numberWithBool:[sender state]] forKeyPath:@"isEdited"];
}

- (IBAction)setTabLabel:(id)sender {
	[[tabView selectedTabViewItem] setLabel:[sender stringValue]];
}
- (BOOL)tabView:(NSTabView *)aTabView shouldCloseTabViewItem:(NSTabViewItem *)tabViewItem {
	[channels removeObject:[ircSession chatRoomWithName:[[tabView selectedTabViewItem] label]]];
	[[ircSession chatRoomWithName:[[tabView selectedTabViewItem] label]] part];
	[tabView removeTabViewItem:[tabView selectedTabViewItem]];
    return YES;
}
- (void)tabView:(NSTabView *)aTabView didCloseTabViewItem:(NSTabViewItem *)tabViewItem {
	NSLog(@"didCloseTabViewItem: %@", [tabViewItem label]);
}

- (PSMTabBarControl *)tabBar {
	return tabBar;
}

#pragma mark GUI Actions

//this is a simple override of -showWindow: to ensure the window is always centered
-(IBAction)showWindow:(id)sender
{
    [super showWindow:sender];
    [[self window] center];
}

@end
