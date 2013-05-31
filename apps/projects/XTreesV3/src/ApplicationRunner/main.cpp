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

#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"
#include "ofxAppUtils.h"
#include "KepleroUtils.h"
#include "inputBox.h"

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif


//========================================================================
int main( ){

  int numberOfDisplays = kplGetNumberOfDisplays();
  
  ofAppGlutWindow window;
  //window.setGlutDisplayString("rgba double samples>=4 depth");
  


#ifdef __APPLE__    
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef mainURL = CFBundleCopyExecutableURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(mainURL, TRUE, (UInt8 *)path, PATH_MAX))
    {
      std::cout << "Doesn't work like this..." << std::endl;
        // error!
    }
    CFRelease(mainURL);
    
    string strExecFile(path);  
    int found = strExecFile.find_last_of("/");  
    string strPath = strExecFile.substr(0, found+1);  
    strPath.append("../../../data/screenDimensions.txt");
#endif

  std::ifstream myfile (strPath.c_str());
  std::string line;
  if (myfile.is_open())
  {
    getline (myfile,line);
    int dimensionX = ofToInt(line);
    getline (myfile,line);
    int dimensionY = ofToInt(line);
    myfile.close();
    ofSetupOpenGL(&window,  dimensionX,dimensionY, OF_FULLSCREEN);
    ofRunAppWithAppUtils( new testApp());
    return 0;
  }
  
  
  if (numberOfDisplays == 1) {
        char answer[512] = "";
      int res;
      int resX, resY;
      do {
        res = inputBox("Horizontal resolution:", "Horizontal resolution", answer);
        resX = atoi(answer);
      } while (res != 0);
      
      do {
        res = inputBox("Vertical resolution:", "Vertical resolution", answer);
        resY = atoi(answer);
      } while (res != 0);
    ofSetupOpenGL(&window,  resX,resY, OF_FULLSCREEN);
  } else {
    // which screen
    int screenToUse = kplScreenSelection();  
    if (screenToUse == -1) {
      exit(1);
    }
    if (screenToUse == 0) { // primary
            char answer[512] = "";
      int res;
      int resX, resY;
      do {
        res = inputBox("Horizontal resolution:", "Horizontal resolution", answer);
        resX = atoi(answer);
      } while (res != 0);
      
      do {
        res = inputBox("Vertical resolution:", "Vertical resolution", answer);
        resY = atoi(answer);
      } while (res != 0);
    ofSetupOpenGL(&window,  resX,resY, OF_FULLSCREEN);
      
    } else {
      // ask the user the resolution of the second screen because we cannot retrieve it... ?????
      char answer[512] = "";
      int res;
      int resX, resY;
      do {
        res = inputBox("Horizontal resolution:", "Horizontal resolution", answer);
        resX = atoi(answer);
      } while (res != 0);
      
      do {
        res = inputBox("Vertical resolution:", "Vertical resolution", answer);
        resY = atoi(answer);
      } while (res != 0);
      
      ofSetupOpenGL(&window,  resX, resY, OF_FULLSCREEN);			// <-------- setup the GL context    
      ofSetWindowPosition(getClientWidth()+1, 0);
    }
  }
  ofRunAppWithAppUtils( new testApp());
}
