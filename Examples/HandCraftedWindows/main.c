/*
 * Demonstrates how to create a window 'by hand'.
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

// load our background image (./background.png)
CGImageRef GetBackgroundImage(void) {
	static CGImageRef image = NULL;
	if(image) return image;
	
	CFURLRef url = CFURLCreateWithFileSystemPathRelativeToBase(kCFAllocatorDefault, CFSTR("background.png"), kCFURLPOSIXPathStyle, false, NULL);
	CGImageSourceRef imageSource = CGImageSourceCreateWithURL(url, NULL /* no options */);
	image = CGImageSourceCreateImageAtIndex(imageSource, 0, NULL /* no options */);
	
	CFRelease(imageSource);
	CFRelease(url);
	
	return image;
}

int main (int argc, const char * argv[]) {
	CGError err = noErr;
	CGSConnectionID cid = CGSMainConnectionID();
	CGImageRef backgroundImage = GetBackgroundImage();
	
    // create the region the window will occupy
	CGRect rect = {0, 0, CGImageGetWidth(backgroundImage), CGImageGetHeight(backgroundImage)};
	CGSRegionObj region = NULL;
	err = CGSNewRegionWithRect(&rect, &region);
	
	// create our window
	CGSWindowID wid = 0;
	err = CGSNewWindow(cid, kCGSBackingBuffered, 100, 100, region, &wid);
	
	// set the window as being non-opaque
	err = CGSSetWindowOpacity(cid, wid, false);
	
	// make it above everything
	err = CGSSetWindowLevel(cid, wid, kCGMaximumWindowLevel);
	
	// get a graphics context
	CGContextRef context = CGWindowContextCreate(cid, wid, 0);
	
	// clear it so it'll be transparent
	CGContextClearRect(context, rect);
	
	// draw the background
	CGContextDrawImage(context, rect, backgroundImage);
	CGContextFlush(context);
	
	// make it appear on screen
	err = CGSOrderWindow(cid, wid, kCGSOrderIn, 0);
	
	// call a run loop (just so we don't quit right away)
	CFRunLoopRun();
	
    return 0;
}
