/*
 * Demonstrates universal owners, window lists, and setting a window list's alpha.
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

#include "CGSInternal.h"
#include <stdbool.h>

bool BecomeUniversalOwner(CGSConnectionID cid) {
	// since we don't have any code loaded into the Dock to make us a universal owner, we have to kill the Dock
	// note that the Dock also controls Spaces, Exposé, and Dashboard...
	system("killall Dock");

	// it's possible (seems to be the case in Leopard) that the Dock will start before we execute the next line!
	return kCGErrorSuccess == CGSSetUniversalOwner(cid);
}

int main (int argc, const char * argv[]) {
    int lastErr = noErr;
	CGSConnectionID cid = CGSMainConnectionID();
	int windowCount = 0;
	CGSWindowID *windowList = NULL;

	// make us the universal owner
	assert(BecomeUniversalOwner(cid));
	
	// grab the count
	lastErr = CGSGetOnScreenWindowCount(cid, kCGSNullConnectionID, &windowCount);
	assert(!lastErr && "Error getting the on screen window count.");
	
	// allocate our window list
	windowList = (CGSWindowID*)calloc(windowCount, sizeof(CGSWindowID));
	assert(windowList && "Error allocating window list.");
	
	// fill in the list
	lastErr = CGSGetOnScreenWindowList(cid, kCGSNullConnectionID, windowCount, windowList, &windowCount);
	assert(!lastErr && "Error getting on screen window list.");
	
	// set the alpha of the windows to be 0.5 over the course of 10 seconds
	CGSSetWindowListAlpha(cid, windowList, windowCount, 0.5, 10);
	
	// setting the the background window's opacity is a BAD thing, so reset it to 1.0, ideally we wouldn't even include this in our list, but I'm lazy :-)
	CGSWindowID backgroundWID = 0;
	CGSGetSystemBackgroundWindow(cid, CGSMainDisplayID(), &backgroundWID);
	CGSSetWindowAlpha(cid, backgroundWID, 1);
	
	sleep(10);
	
	// And revert it all back to 1.0
	CGSSetWindowListAlpha(cid, windowList, windowCount, 1, 10);
	
    return 0;
}
