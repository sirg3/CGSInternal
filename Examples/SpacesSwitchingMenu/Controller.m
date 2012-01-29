/*
 * Copyright (C) 2007-2008 Alacatia Labs
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 * 
 * Joe Ranieri joe@alacatia.com
 *
 */

#import "Controller.h"

// private CoreDock routines
extern bool CoreDockGetWorkspacesEnabled();
extern void CoreDockGetWorkspacesCount(int *rows, int *columns);

//! Gets called when the current space changes.
void SpaceChangedCallback(CGSNotificationType type, CGSWorkspaceID *workspace, unsigned int dataLength, Controller *self) {
	if(*workspace != kCGSTransitioningWorkspaceID) {
		[self updateState];
	}
}

@implementation Controller

//! Opens the Spaces preference pane in System Preferences.
- (void)openSpacesPreferences:(id)sender {
	NSAppleScript *script = [[NSAppleScript alloc] initWithSource:@"tell application \"System Preferences\" to activate (reveal anchor \"Spaces\" of pane id \"com.apple.preference.expose\")"];
	[script executeAndReturnError:nil];
	[script release];
}

//! Switches the current workspace to `spaceID`.
- (void)switchToSpace:(CGSWorkspaceID)spaceID {
	// note that we're using a notification here -- if we used the CGS APIs, the Dock wouldn't show the switching UI
	[[NSDistributedNotificationCenter defaultCenter] postNotificationName:@"com.apple.switchSpaces"
																   object:[NSString stringWithFormat:@"%i", spaceID - 1]];
}

//! Gets the total number of workspaces.
- (int)workspacesCount {
	int rows, columns;
	CoreDockGetWorkspacesCount(&rows, &columns);
	
	return rows * columns;
}

//! Gets the current workspace.
- (CGSWorkspaceID)currentWorkspace {
	CGSWorkspaceID result;
	CGSGetWorkspace(CGSMainConnectionID(), &result);
	
	return result;
}

//! Called when the user clicks one of the spaces to switch.
- (void)menuItemPressed:(NSMenuItem *)sender {
	[self switchToSpace:[sender tag]];
}

//! Reloads the status item's menu. Since we have no way of being notified when the 
//! user changes the number of spaces, we have to update this each time the menu needs
//! to be shown.
- (NSMenu *)buildMenu {
	NSMenu *result = [[NSMenu alloc] init];
	
	if(CoreDockGetWorkspacesEnabled()) {
		for(CGSWorkspaceID ws = 1; ws <= [self workspacesCount]; ws++) {
			NSMenuItem *item = [result addItemWithTitle:[NSString stringWithFormat:@"%i", ws]
												 action:@selector(menuItemPressed:)
										  keyEquivalent:@""];
			[item setTarget:self];
			[item setTag:ws];
			
			// if this is the current workspace, give it a nice check
			if(ws == [self currentWorkspace]) {
				[item setState:1];
			}
		}
	} else {
		NSMenuItem *item = [result addItemWithTitle:@"Spaces is not enabled" action:nil keyEquivalent:@""];
		[item setEnabled:NO];
	}
	
	[result addItem:[NSMenuItem separatorItem]];
	[[result addItemWithTitle:@"Open Spaces Preferences…" action:@selector(openSpacesPreferences:) keyEquivalent:@""] setTarget:self];
	
	return [result autorelease];
}

//! Sets the status item's title.
- (void)setStatusItemTitle:(NSString *)value {
	// figure out how wide we need to be
	NSDictionary *attributes = [NSDictionary dictionaryWithObject:[NSFont systemFontOfSize:[NSFont systemFontSize]]
														   forKey:NSFontAttributeName];
	
	CGFloat newWidth = [value sizeWithAttributes:attributes].width + 15;
	
	// we need to disable screen updates or it flickers
	NSDisableScreenUpdates();	
	[statusItem setLength:newWidth];
	[statusItem setTitle:value];
	NSEnableScreenUpdates();
}

//! Updates all of our state.
- (void)updateState {
	if(CoreDockGetWorkspacesEnabled()) {
		[self setStatusItemTitle:[NSString stringWithFormat:@"%i", [self currentWorkspace]]];
	} else {
		[self setStatusItemTitle:@"N/A"];
	}
}

//! Gets called when the user enables or disables Spaces.
- (void)spacesEnabledChanged:(NSNotification *)notification {
	[self updateState];
}

//! Gets called when the user clicks our status item.
- (void)showMenu:(id)sender {
	[statusItem popUpStatusItemMenu:[self buildMenu]];
}

- (void)awakeFromNib {
	// setup the status item
	statusItem = [[[NSStatusBar systemStatusBar] statusItemWithLength:0] retain];
	[statusItem setHighlightMode:YES];
	[statusItem setTarget:self];
	[statusItem setAction:@selector(showMenu:)];
	[statusItem sendActionOn:NSLeftMouseDownMask];
	[self updateState];
	
	// listen for when Spaces is enabled or disabled
	[[NSDistributedNotificationCenter defaultCenter] addObserver:self
														 selector:@selector(spacesEnabledChanged:)
															 name:@"SpacesEnableChange"
														   object:nil
											   suspensionBehavior:NSNotificationSuspensionBehaviorDeliverImmediately];
	
	// listen for when the current Space changes
	CGSRegisterNotifyProc((CGSNotifyProcPtr)SpaceChangedCallback, kCGSNotificationWorkspaceChanged, self);
}

- (void)dealloc {
	[statusItem release];
	[super dealloc];
}

@end
