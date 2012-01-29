/*
 * Shows when applications hang by listening to CGS notifications.
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

#include <CoreFoundation/CoreFoundation.h>
#include <Carbon/Carbon.h>
#include "CGSInternal.h"

void NotifyProc(CGSNotificationType type, void *data, unsigned int dataLength, void *userData) {
	assert(kCGSNotificationAppUnresponsive == type || kCGSNotificationAppResponsive == type);
	assert(dataLength >= sizeof(CGSProcessNotificationData));
	
	CGSProcessNotificationData *processData = (CGSProcessNotificationData *)data;
	
	// grab the name
	CFStringRef name;
	CopyProcessName(&processData->psn, &name);
	
	// figure out what our message should be
	CFStringRef message;
	if(kCGSNotificationAppUnresponsive == type) {
		message = CFStringCreateWithFormat(NULL, NULL, CFSTR("%@ (%i) is hung!"), name, processData->pid);
	} else {
		message = CFStringCreateWithFormat(NULL, NULL, CFSTR("%@ (%i) is responsive again!"), name, processData->pid);
	}
	
	// blab about it in the console
	CFShow(message);
	
	// and clean up
	CFRelease(message);
	CFRelease(name);
}

int main(int argc, const char * argv[]) {	
	// register for our responsive / unresponsive notifications
	CGSRegisterNotifyProc(&NotifyProc, kCGSNotificationAppUnresponsive, NULL);
	CGSRegisterNotifyProc(&NotifyProc, kCGSNotificationAppResponsive, NULL);
	
	// run forever
    RunApplicationEventLoop();
	
    return 0;
}
