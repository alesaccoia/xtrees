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

#include "KepleroUtils.h"
#include "KplHttpRequest.h"

void utf8toWStr(std::wstring& dest, const std::string& src){
	dest.clear();
	wchar_t w = 0;
	int bytes = 0;
	wchar_t err = L'�';
	for (size_t i = 0; i < src.size(); i++){
		unsigned char c = (unsigned char)src[i];
		if (c <= 0x7f){//first byte
			if (bytes){
				dest.push_back(err);
				bytes = 0;
			}
			dest.push_back((wchar_t)c);
		}
		else if (c <= 0xbf){//second/third/etc byte
			if (bytes){
				w = ((w << 6)|(c & 0x3f));
				bytes--;
				if (bytes == 0)
					dest.push_back(w);
			}
			else
				dest.push_back(err);
		}
		else if (c <= 0xdf){//2byte sequence start
			bytes = 1;
			w = c & 0x1f;
		}
		else if (c <= 0xef){//3byte sequence start
			bytes = 2;
			w = c & 0x0f;
		}
		else if (c <= 0xf7){//3byte sequence start
			bytes = 3;
			w = c & 0x07;
		}
		else{
			dest.push_back(err);
			bytes = 0;
		}
	}
	if (bytes)
		dest.push_back(err);
}


ofColor mixColor(const ofColor& rhs, const ofColor lhs, float percentage) {
  ofColor result;
  result.r = (lhs.r - rhs.r) * percentage;
  result.g = (lhs.g - rhs.g) * percentage;
  result.b = (lhs.b - rhs.b) * percentage;
  return result;
}

ofColor kplToColor(const ofColor& rhs, float percentage) {
  ofColor result;
  float inv = 255.f * (1.0f - percentage);
  result.r = inv  + rhs.r * percentage;
  result.g = inv  + rhs.g * percentage;
  result.b = inv  + rhs.b * percentage;
  return result;
}

int kplToss(int number) {
  float division = 1.0F / ((float)number);
  float randd = ofRandomuf();
  for (int i = 1; i <= number; ++i) {
    if (randd <= division * (float)i) {
      return i - 1; 
    }
  }
}

bool CheckInternetConnection() {
  char* data;
  int nbytes;
  KplHttpRequest req;
  // hope they have 99.9% uptime
  bool isok = req.getUrlSync("http://www.agneschavez.com", &data, nbytes);
  if (isok && nbytes) {
    delete [] data;
    return true;
  }
  return false;
}

#ifdef __APPLE__

#ifndef XTREES_IOS
#include <Carbon/Carbon.h>
#endif

//http://developer.apple.com/library/mac/#documentation/GraphicsImaging/Reference/Quartz_Services_Ref/Reference/reference.html
#ifndef XTREES_IOS
int getClientWidth() {
  return (int)CGDisplayPixelsWide(kCGDirectMainDisplay);
}

int getClientHeight() {
  return (int)CGDisplayPixelsHigh(kCGDirectMainDisplay);
}

std::wstring toWstring(std::string s) {
  wchar_t* buf = new wchar_t[ s.size() ];
  size_t num_chars = mbstowcs( buf, s.c_str(), s.size() );
  std::wstring ws( buf, num_chars );
  delete[] buf;
  return ws;
}
#endif

#define KPL_MAX_URL_LENGTH 60
char s_url [KPL_MAX_URL_LENGTH];

#ifndef XTREES_IOS
std::string kplToUrlEncoding(std::string str_) {
  CFStringRef temp;
  temp = CFStringCreateWithCString (NULL, str_.c_str(), kCFStringEncodingUTF8);
  
  CFStringRef escapeds;
  escapeds = CFURLCreateStringByAddingPercentEscapes(NULL,
    temp,
    NULL,
    NULL,
    kCFStringEncodingUTF8);
  
  CFStringGetCString(escapeds, s_url, KPL_MAX_URL_LENGTH, kCFStringEncodingASCII);
  
  CFRelease(escapeds);
  CFRelease(temp);
  return s_url;
}

int kplGetNumberOfDisplays() {
  int i = 0;
  
  while(CGDisplayIsActive(i++)) ;
  
  return --i;
  
}
#else
std::string kplToUrlEncoding(std::string str_) {
  return str_;
}

#endif

#ifdef XTREES_APP
int kplScreenSelection() {
    AlertStdCFStringAlertParamRec alertParams;
    GetStandardAlertDefaultParams(&alertParams, kStdCFStringAlertVersionOne);
    alertParams.movable = true;
    alertParams.cancelText = CFSTR("Secondary screen");
    alertParams.defaultButton = kAlertStdAlertOKButton;
    DialogRef dialog;
    OSStatus alertErr = CreateStandardAlert(kAlertCautionAlert,
        CFSTR("Press OK to run the app on the primary screen, or choose the secondary screen in the case you want to run it with a projector?"),
        CFSTR("This is needed to setup the screen correctly in an extended desktop situation."),
        &alertParams, &dialog);
    if (alertErr == noErr)
    {
        alertErr = SetWindowTitleWithCFString(GetDialogWindow(dialog), CFSTR("Screen selection"));
        DialogItemIndex itemHit;
        alertErr = RunStandardAlert(dialog, NULL, &itemHit);
        if (alertErr == noErr)
        {
            if (itemHit == kAlertStdAlertCancelButton) {
                return 1;
            } else {
                return 0;
            }
        }
    } else {
      return -1;
    }
}

int kplFullscreen() {
    AlertStdCFStringAlertParamRec alertParams;
    GetStandardAlertDefaultParams(&alertParams, kStdCFStringAlertVersionOne);
    alertParams.movable = true;
    alertParams.cancelText = CFSTR("Window mode");
    alertParams.defaultButton = kAlertStdAlertOKButton;
    DialogRef dialog;
    OSStatus alertErr = CreateStandardAlert(kAlertCautionAlert,
        CFSTR("Press OK to run the app in fullscreen?"),
        CFSTR(""),
        &alertParams, &dialog);
    if (alertErr == noErr)
    {
        alertErr = SetWindowTitleWithCFString(GetDialogWindow(dialog), CFSTR("Fullscreen selection"));
        DialogItemIndex itemHit;
        alertErr = RunStandardAlert(dialog, NULL, &itemHit);
        if (alertErr == noErr)
        {
            if (itemHit == kAlertStdAlertCancelButton) {
                return 1;
            } else {
                return 0;
            }
        }
    } else {
      return -1;
    }

}

#endif

#else


#include <stdlib.h>

// TODO: avoid leak
// see http://msdn.microsoft.com/en-us/library/k1f9b8cy(vs.71).aspx

std::wstring toWstring(std::string str_) {
  wchar_t *pwc = (wchar_t *)malloc( sizeof( wchar_t ));
  mbstowcs(pwc, str_.c_str(), MB_CUR_MAX );
  std::wstring returnMe(pwc);
  free pwc;
  return returnMe;
}


#endif


