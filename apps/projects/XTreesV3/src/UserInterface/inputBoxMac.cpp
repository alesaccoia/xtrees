/*
 * Copyright (c) 2012-2013 Agnes Chavez and Alessandro Saccoia
 * Written by Alessandro Saccoia, <alessandro.saccoia@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Sotware.
 */
 
#include "inputBox.h"


#include "Carbon/Carbon.h"

#include <cstring>

// Inputbox

char inputBoxString[512];

ControlRef pInputBoxMessage, pInputBoxEdit, pInputBoxOk, pInputBoxCancel;
WindowRef pInputBoxWindow;
		
int result;
																																	
OSStatus inputBoxEventHandler(EventHandlerCallRef inHandlerCallRef, EventRef inEvent, void *inUserData)
{
	ControlRef pControl;	
	GetEventParameter(inEvent, kEventParamDirectObject, typeControlRef, NULL, sizeof(ControlRef), NULL, &pControl);
	
	if (pControl == pInputBoxOk) {
		Size actualSize;
		GetControlData(pInputBoxEdit, 0, kControlEditTextTextTag, 255, inputBoxString, &actualSize);
		inputBoxString[actualSize]=0;
		QuitAppModalLoopForWindow(pInputBoxWindow);
		result = 0;
		return noErr;
	} else if (pControl == pInputBoxCancel) {
		inputBoxString[0] = 0;
		QuitAppModalLoopForWindow(pInputBoxWindow);
		result = 1;
		return noErr;
	}
	return eventNotHandledErr;
}

int inputBox(char* prompt, char* title, char* def)
{
	static const EventTypeSpec	kInputBoxEvents[] = {{kEventClassControl, kEventControlHit}};
	
	result = 1;

	Rect bounds = {50, 50, 50 + 125, 50 + 363};
	CreateNewWindow(kMovableModalWindowClass, kWindowStandardHandlerAttribute, &bounds, &pInputBoxWindow);
	SetThemeWindowBackground(pInputBoxWindow, kThemeBrushDialogBackgroundActive, false);
	RepositionWindow(pInputBoxWindow, NULL, kWindowCenterOnMainScreen);
	
	CFStringRef titleCF = CFStringCreateWithCString(NULL, title, kCFStringEncodingMacRoman);
	SetWindowTitleWithCFString(pInputBoxWindow, titleCF);
	CFRelease(titleCF);
		
	CFStringRef defCF = CFStringCreateWithCString(NULL, def, kCFStringEncodingMacRoman);		
	//bounds = (Rect){125 - 62 - 16, 23, 125 - 62, 23 + 317};
	bounds = (Rect){47, 23, 63, 330};
	CreateEditUnicodeTextControl (pInputBoxWindow, &bounds, defCF, false, NULL, &pInputBoxEdit);
	CFRelease(defCF);

	bounds = (Rect){125 - 21 - 20, 276, 125 - 21, 276 + 66};
	CreatePushButtonControl(pInputBoxWindow, &bounds, CFSTR("OK"), &pInputBoxOk);
	SetWindowDefaultButton(pInputBoxWindow, pInputBoxOk);

	bounds = (Rect){125 - 21 - 20, 187, 125 - 21, 187 + 79};
	CreatePushButtonControl(pInputBoxWindow, &bounds, CFSTR("Cancel"), &pInputBoxCancel);
	SetWindowCancelButton(pInputBoxWindow, pInputBoxCancel);
	
	CFStringRef promptCF=CFStringCreateWithCString(NULL, prompt, kCFStringEncodingMacRoman);		
	bounds = (Rect){129-89-20, 20, 129-89, 20+323};
	CreateStaticTextControl(pInputBoxWindow, &bounds, promptCF, NULL, &pInputBoxMessage);
	CFRelease(promptCF);
						
	ShowWindow(pInputBoxWindow);
	SelectWindow(pInputBoxWindow);
	SetKeyboardFocus(pInputBoxWindow, pInputBoxEdit, kControlFocusNextPart);
			
	InstallWindowEventHandler(pInputBoxWindow, inputBoxEventHandler, GetEventTypeCount(kInputBoxEvents), kInputBoxEvents, NULL, NULL);
	RunAppModalLoopForWindow(pInputBoxWindow);

	EndAppModalStateForWindow(pInputBoxWindow);
	DisposeWindow(pInputBoxWindow); 
	
	strcpy(def, inputBoxString);

	return result;
}