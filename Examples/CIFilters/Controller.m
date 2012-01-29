/*
 * Applies a CoreImage filter to a window.
 *
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

@implementation Controller
- (void)awakeFromNib {
	[window setDelegate:self];
}

// we need to remove the filter when we're minimized or bad things will happen
- (void)windowWillMiniaturize:(NSNotification *)notification {
	[self removeFilter];
}

// since we're exiting, we need to remove the filter or bad things will happen
- (void)windowWillClose:(NSNotification *)notification {
	[self removeFilter];
}

// apply a new filter
- (IBAction)apply:(id)sender {
	// remove the existing filter before adding a new one
	[self removeFilter];
	
	// find our filter
	CGError err = CGSNewCIFilterByName(CGSMainConnectionID(), (CFStringRef)[nameField stringValue], &filterID);
	if(noErr != err) {
		NSBeep();
		return;
	}
	
	// and apply it to our window
	err = CGSAddWindowFilter(CGSMainConnectionID(), (CGSWindowID)[window windowNumber], filterID, 0);
	hasCreatedFilter = YES;
}

// remove any existing filter
- (void)removeFilter {
	if(hasCreatedFilter) {
		CGSRemoveWindowFilter(CGSMainConnectionID(), (CGSWindowID)[window windowNumber], filterID);
		CGSReleaseCIFilter(CGSMainConnectionID(), filterID);
		hasCreatedFilter = NO;
	}
}

@end
