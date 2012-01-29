/*
 * Private routines from the PreferencePanes framework.
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

@interface NSPrefPaneUtils : NSObject
{
}

+ (void)enableControls:(BOOL)fp8 inView:(id)fp12 deep:(BOOL)fp16;
+ (BOOL)isMacOSXServer;
+ (BOOL)hasBattery;
+ (BOOL)_needFN;
+ (id)_labelForKey:(unsigned int)fp8;
+ (id)_stringForVirtualKey:(unsigned int)fp8 modifiers:(unsigned int)fp12;
+ (BOOL)isFunctionKey:(unsigned int)fp8;
+ (id)stringForModifiers:(unsigned int)fp8;
+ (id)humanStringForModifiers:(unsigned int)fp8;
+ (id)humanStringForModifiers:(unsigned int)fp8 charKey:(id)fp12;
+ (id)humanStringForModifiers:(unsigned int)fp8 key:(unsigned int)fp12;
+ (id)stringForVirtualKey:(unsigned int)fp8 modifiers:(unsigned int)fp12;
+ (id)stringForSymbolicKey:(unsigned int)fp8;
+ (id)centerTruncateString:(id)fp8 toFit:(int)fp12;

@end
