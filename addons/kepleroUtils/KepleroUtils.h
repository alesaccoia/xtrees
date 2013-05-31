/*
 *  KepleroUtils.h
 *  TwitterRetriever
 *
 *  Created by Alessandro Saccoia on 1/20/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */
 
#ifndef KEPLEROUTILS_H_
#define KEPLEROUTILS_H_

#include "ofMain.h"
#include <string>

int getClientWidth();

int getClientHeight();

void utf8toWStr(std::wstring& dest, const std::string& src);

std::wstring toWstring(std::string s);

ofColor mixColor(const ofColor& rhs, const ofColor lhs, float percentage);

int kplToss(int number);

int kplScreenSelection();

int kplFullscreen();

int kplGetNumberOfDisplays();

#endif // KEPLEROGRAPHICS_H_
