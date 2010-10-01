//
//  tabModal.h
//  iaic
//
//  Created by Ashley Towns on 11/07/10.
//  Copyright 2010 Mogeneration. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface tabModel : NSObject {
	BOOL						_isProcessing;
	NSImage					*_icon;
	NSString					*_iconName;
	NSInteger					_objectCount;
	BOOL						_isEdited;
}

// creation/destruction
- (id)init;

// accessors
- (BOOL)isProcessing;
- (void)setIsProcessing:(BOOL)value;
- (NSImage *)icon;
- (void)setIcon:(NSImage *)icon;
- (NSString *)iconName;
- (void)setIconName:(NSString *)iconName;
- (NSInteger)objectCount;
- (void)setObjectCount:(NSInteger)value;
- (BOOL)isEdited;
- (void)setIsEdited:(BOOL)value;

@end
