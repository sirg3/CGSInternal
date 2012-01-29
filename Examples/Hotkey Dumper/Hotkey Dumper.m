/*
 * Dumps a list of symbolic hotkeys and their keyboard shortcut.
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

#import <Foundation/Foundation.h>
#import "CGSInternal.h"
#import "NSPrefPaneUtils.h"

int main (int argc, const char * argv[]) {
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	
    int i = 0;
	for(i = 0; i < 1000; i++) {
		// we need to know if this hotkey actually has any sort of value
		int virtualKeyCode = 0;
		CGSGetSymbolicHotKeyValue((CGSSymbolicHotKey)i, NULL, &virtualKeyCode, NULL);
		
		// nope, no value, don't print this hotkey
		if(!virtualKeyCode) continue;
		
		// print out disabled if it is disabled
		NSString *disabledStatus = @"";
		if(!CGSIsSymbolicHotKeyEnabled((CGSSymbolicHotKey)i)) disabledStatus = @" (disabled)";
		
		// this uses a private call in the PreferencePane framework - I didn't want to go through the insane hassle of messing with virtual key codes, etc
		NSLog(@"%i - '%@'%@\n", i, [NSPrefPaneUtils stringForSymbolicKey:i], disabledStatus);
	}
	
    [pool release];
    return 0;
}
