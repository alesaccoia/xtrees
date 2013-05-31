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

/*
 * generated with
 * http://www.kjetil-hartveit.com/blog/1/setter-and-getter-generator-for-php-javascript-c%2B%2B-and-csharp#SG_Arguments
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <string>
#include <map>
#include "ofMain.h"
#include "ofConstants.h"

#ifndef XTREES_IOS
#include <FTGL/ftgl.h>
#else
#include "FTGL/ftgles.h"
#endif

#include "ofEvents.h"
#include "MSACore.h"
#include "MSAFluidSolver.h"
#include "LeavesLayer.h"

using namespace MSA;


#define BALOON_MSG_SIZE 300
#define BALOON_MWG_LINESPACING .9F


#define notifySetting(eventNr) Setting tont = eventNr; ofNotifyEvent(ofConfigurationChanged, tont, this)

extern unsigned int g_globalCounter;
extern unsigned int g_globalCounterSec;
extern unsigned int g_splashFrequency;
extern unsigned int g_splashDuration;
extern float g_splashOpacity;

class testApp;
class LeavesLayer;

extern testApp* g_app;

extern ofColor g_backgroundC;
extern ofColor g_treeC;
extern bool g_jointsAreCircles;
extern ofColor g_jointC;
extern float g_branchWidth;
extern float g_branchMinWidth;
extern float g_branchLength;
extern float g_lengthDecrease;
extern float g_widthDecrease;
extern int g_pastTweets;
extern int g_jointThickness;
extern int g_jointLength;

extern ofColor g_tweetMsgC;
extern ofColor g_tweetUsrC;
extern ofColor g_twilioMsgC;
extern ofColor g_twilioUsrC;
extern ofColor g_databaseMsgC;
extern ofColor g_databaseUsrC;

extern ofColor g_tLineC;
extern int g_tLineWidth;
extern int g_growthMin;
extern int g_growthMax;
extern bool g_timedExhibit;
extern float g_showDuration;
extern int g_minFrequency;
extern int g_maxFrequency;
extern int g_minAngle;
extern int g_maxAngle;
extern bool g_showThumbs;
extern int g_fertility;
extern int g_maxLevel; // how many branches per tree given the currentLevel
extern int g_maxBranches; // how many branches per tree given the currentLevel
extern float g_floatingSpeed;
extern ofColor g_splashC;
extern int g_flowersMin;
extern int g_flowersMax;
extern int g_crazyLineWidth;
extern int g_crazyCircleWidth;
extern bool g_useBackground;
extern int g_tweetMinLife;
extern int g_tweetMaxLife;
	
extern std::map<string, int> g_textAnchorX;
extern std::map<string, int> g_textAnchorY;


extern FTFont*  g_guiFont;
extern FTSimpleLayout* g_guiRenderer;

extern FTSimpleLayout* g_twitterLayout;
extern FTSimpleLayout* g_twilioLayout;
extern FTSimpleLayout* g_databaseLayout;

extern FTFont* g_twitterFont;
extern FTFont* g_twilioFont;
extern FTFont* g_databaseFont;

extern std::vector<ofImage*> g_originalImages;
extern std::vector<ofImage*> g_images;

extern std::vector<ofImage*> g_leafImages;
extern std::vector<ofImage*> g_backgroundImages;

extern bool g_changeBranchImages;

extern ofRectangle g_scene;
extern ofRectangle g_sceneBounded;

extern float g_soundVolume;

extern bool g_useInteractiveAudio;

extern bool g_useBackgroundImage;
extern std::string g_backgroundImage;
extern std::string g_soundtrack;

extern float g_minColonization;
extern float g_minColonizationForLeaves;

extern bool g_treesCanFade;

extern float g_fps;
extern float g_currentFps;

extern int g_numberOfseeds;
extern char g_seed1[60];
extern char g_seed2[60];
extern char g_seed3[60];
extern char g_seed4[60];
extern char g_seed5[60];

extern float g_treesLayerOpacity;
extern float g_twitterLayerOpacity;
extern float g_flowersOpacity;

extern ofImage g_flowerImage;

extern bool g_showFlowers;

extern FluidSolver *s_solver;
extern Vec2f s_windowSize;
extern Vec2f s_invWindowSize;

extern LeavesLayer* g_leavesLayer;

extern float g_leavesFertility;
extern ofColor g_leavesEndColor;
extern float g_leavesOpacity;
extern float g_leavesLife;
extern float g_leavesMinWidth;
extern float g_leavesMaxWidth;
extern float g_leavesMinFreq;
extern float g_leavesMaxFreq;

extern ofColor g_flowersColor;
extern ofColor g_flowersLineColor;

extern bool g_useTwilio;

extern int g_leavesRows;
extern int g_leavesColumns;

extern bool g_leavesActive;

extern int g_treesFadeTime;

extern int g_backgroundTransitionTime;

extern bool g_useArchive;

extern volatile int g_activeThreads;
extern ofMutex g_activeThreadsMutex;

extern float g_waitSeedTime; // wait in seed mode
extern float g_waitLinesTime; // tree full, wait to start lines
extern float g_startDetachingLeavesTime;
extern float g_waitRegenerateTime; // after leaves fell how long does it take to regenerate




extern int g_msgOpacity;

extern float g_linesMin;
extern float g_linesMax;

extern float g_linesMinNorm;
extern float g_linesMaxNorm;
extern float g_linesMinAcc;
extern float g_linesMaxAcc;

extern int g_firstIterations;

/////////////////////////////////////////////////////////////////////////

void g_initializeFonts();
void g_initializeBranchImages();
void g_initializeLeafImages();
void g_initializeBackgroundImages();
void g_releaseBranchImages();
void g_releaseLeafImages();
void g_initializeFlowerImages();
void g_colorizeBranchImages();
void g_setTwitterMsgFontSize(int fontsize);
void g_setTwilioMsgFontSize(int fontsize);
void g_setDatabaseMsgFontSize(int fontsize);

void g_colorizeSplashImages();
void g_computeMaxBranches();
#ifndef XTREES_SCREENSAVER
void g_updateBackground();
void g_activateSoundtrack();
void g_activateFlowers();
#endif

#endif //

