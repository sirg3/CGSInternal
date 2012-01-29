/*
 * Demonstrates the use of a full screen CoreGraphics transition.
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

#define TRANSITION_DURATION 1

int main (int argc, const char * argv[]) {
	CGError err = noErr;
	CGSConnectionID cid = CGSMainConnectionID();
	
	// setup our options
	CGSTransitionSpec spec = {
		0,
		kCGSTransitionCube,
		kCGSTransitionDirectionRight,
		0, // fullscreen
		NULL // black background
	};
	
	// create the transition
	int transition = 0;
	err = CGSNewTransition(cid, &spec, &transition);
	
	// invoke the transition
	err = CGSInvokeTransition(cid, transition, TRANSITION_DURATION);
	
	// wait for it to finish
	sleep(TRANSITION_DURATION);
	
	// and clean up
	err = CGSReleaseTransition(cid, transition);

    return 0;
}
