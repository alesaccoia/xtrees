/**
 *  main.cpp
 *
 *  Created by Marek Bereza on 06/01/2012.
 */

#include "XTreesSSMain.h"
#include "ofxAppMacScreenSaver.h"
#include "XTreesSSApp.h"
ofxAppMacScreenSaver *ofxScreensaver_main() {
	ofxAppMacScreenSaver *screensaver = new ofxAppMacScreenSaver();
	screensaver->setApp(new testApp());
	return screensaver;
}