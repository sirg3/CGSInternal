/*
 * Grabs information from other applications' windows.
 * This example is deprecated by the CGWindow API, added in Leopard.
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

// gets the name of the application that owns this connection
CFStringRef GetConnectionName(CGSConnectionID cid) {
	int lastErr = noErr;
	
	// get the pid of the connection
	pid_t pid = 0;
	lastErr = CGSConnectionGetPID(cid, &pid);
	
	// get the PSN for the pid
	ProcessSerialNumber psn = {0};
	lastErr = GetProcessForPID(pid, &psn);
	
	// grab the name
	CFStringRef name = NULL;
	lastErr = CopyProcessName(&psn, &name);
	
	return name;
}

// since CFRelease will crash if obj is NULL, wrap it
void SafeCFRelease(CFTypeRef obj) {
	if(obj) CFRelease(obj);
}

int main(int argc, const char * argv[]) {
    int lastErr = noErr;
	CGSConnectionID cid = CGSMainConnectionID();
	int windowCount = 0;
	CGSWindowID *windowList = NULL;
	
	// grab the count
	lastErr = CGSGetOnScreenWindowCount(cid, kCGSNullConnectionID, &windowCount);
	
	// allocate our window list
	windowList = (CGSWindowID*)calloc(windowCount, sizeof(CGSWindowID));
	
	// fill in the list
	lastErr = CGSGetOnScreenWindowList(cid, kCGSNullConnectionID, windowCount, windowList, &windowCount);
	
	// loop over the window list
	int i = 0;
	for(i = 0; i < windowCount; i++) {
		// get the window title
		CFStringRef title = NULL;
		lastErr = CGSGetWindowProperty(cid, windowList[i], CFSTR("kCGSWindowTitle"), &title);
		if(!title) title = CFSTR("(null)");
		
		// get the window rect
		CGRect rect = CGRectZero;
		lastErr = CGSGetScreenRectForWindow(cid, windowList[i], &rect);
		
		// get the connection that owns this window
		CGSConnectionID ownerCID = kCGSNullConnectionID;
		lastErr = CGSGetWindowOwner(cid, windowList[i], &ownerCID);
		
		// get the name of the connection
		CFStringRef appName = GetConnectionName(ownerCID);
		if(!appName) appName = CFSTR("(null)");
		
		// now that we have all of our info, we can print it out
		printf("[%s] %s - {%f, %f, %f, %f}\n", CFStringGetCStringPtr(appName, kCFStringEncodingMacRoman), CFStringGetCStringPtr(title, kCFStringEncodingMacRoman), rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
		
		// clean up
		SafeCFRelease(appName);
		SafeCFRelease(title);
	}
	
    return 0;
}
