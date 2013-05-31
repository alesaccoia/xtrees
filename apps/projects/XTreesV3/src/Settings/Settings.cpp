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


#include "Settings.h"
#include "FlowersLayer.h"

#ifdef XTREES_SCREENSAVER
#include "XTreesSSApp.h"
#endif


#ifdef XTREES_APP
#include "testApp.h"
#endif

#include "LeavesLayer.h"  
#include "ofMain.h"
#include "GraphicUtilities.hpp"
#include "MSACore.h"
#include "MSAFluidSolver.h"

using namespace MSA;

#include <fstream>
#include <iostream>

#define loadNumber(name, method) if (name == name_) { \
  int value = XML->getValue("Value", 0.0, 0); \
  method(value); \
}


#ifndef XTREES_IOS
testApp* g_app;
#endif

unsigned int g_globalCounter;
unsigned int g_globalCounterSec;
unsigned int g_splashFrequency;
unsigned int g_splashDuration;
float g_splashOpacity;

ofColor g_backgroundC;
ofColor g_treeC;
bool g_jointsAreCircles;
ofColor g_jointC;
float g_branchWidth;
float g_branchMinWidth;
float g_branchLength;
float g_lengthDecrease;
float g_widthDecrease;
int g_pastTweets;
int g_jointThickness;
int g_jointLength;
ofColor g_tweetMsgC;
ofColor g_tweetUsrC;
ofColor g_twilioMsgC;
ofColor g_twilioUsrC;
ofColor g_databaseMsgC;
ofColor g_databaseUsrC;
ofColor g_tLineC;
int g_tLineWidth;
int g_growthMin;
int g_growthMax;
bool g_timedExhibit;
float g_showDuration;
int g_minFrequency;
int g_maxFrequency;
int g_minAngle;
int g_maxAngle;
bool g_showThumbs;
int g_fertility;
int g_maxLevel;
int g_maxBranches; 
float g_floatingSpeed;
ofColor g_splashC;
int g_flowersMin;
int g_flowersMax;
int g_crazyLineWidth;
int g_crazyCircleWidth;
bool g_useBackground;
int g_tweetMinLife;
int g_tweetMaxLife;

std::map<string, int> g_textAnchorX;
std::map<string, int> g_textAnchorY;

FTFont*  g_guiFont;
FTSimpleLayout* g_guiRenderer;

FTSimpleLayout* g_twitterLayout;
FTSimpleLayout* g_twilioLayout;
FTSimpleLayout* g_databaseLayout;

FTFont* g_twitterFont;
FTFont* g_twilioFont;
FTFont* g_databaseFont;

std::vector<ofImage*> g_originalImages;
std::vector<ofImage*> g_images;
std::vector<ofImage*> g_leafImages;
std::vector<ofImage*> g_backgroundImages;

ofRectangle g_scene;
ofRectangle g_sceneBounded;

float g_soundVolume;

bool g_useInteractiveAudio;

bool g_useBackgroundImage;
bool g_changeBranchImages;

std::string g_backgroundImage;
std::string g_soundtrack;

float g_minColonization;
float g_minColonizationForLeaves;
bool g_treesCanFade;
float g_fps;
float g_currentFps;

int g_numberOfseeds;
char g_seed1[60];
char g_seed2[60];
char g_seed3[60];
char g_seed4[60];
char g_seed5[60];

float g_treesLayerOpacity;
float g_twitterLayerOpacity;
float g_flowersOpacity;

ofImage g_flowerImage;
bool g_showFlowers;

FluidSolver *s_solver;
Vec2f s_windowSize;
Vec2f s_invWindowSize;

LeavesLayer* g_leavesLayer;

float g_leavesFertility;
ofColor g_leavesEndColor;
float g_leavesOpacity;
float g_leavesLife;
float g_leavesMinWidth;
float g_leavesMaxWidth;
ofColor g_flowersColor;
ofColor g_flowersLineColor;

bool g_useTwilio;

int g_leavesRows;
int g_leavesColumns;

bool g_leavesActive;

float g_leavesMinFreq;
float g_leavesMaxFreq;

int g_treesFadeTime;

int g_backgroundTransitionTime;
bool g_useArchive;

volatile int g_activeThreads;
ofMutex g_activeThreadsMutex;


float g_waitSeedTime; // wait in seed mode
float g_waitLinesTime; // tree full, wait to start lines
float g_linesTime; // how long lines
float g_flowersTime; // how long flowers
float g_waitLeavesTime; // how long wait for leaves
float g_waitRegenerateTime; // after leaves fell how long does it take to regenerate
float g_startDetachingLeavesTime;

int g_msgOpacity;

float g_linesMin;
float g_linesMax;
float g_linesMinNorm;
float g_linesMaxNorm;
float g_linesMinAcc;
float g_linesMaxAcc;


int g_firstIterations;

void g_initializeFonts() {
  int fontsize = 16;
  
  // this is for the seeds
  g_guiFont = new FTTextureFont(ofToDataPath("fonts/ArialUnicode.ttf").c_str());
  g_guiFont->Outset(0.0f, fontsize);
  g_guiFont->CharMap(ft_encoding_unicode);

  if(g_guiFont->Error()){
    ofLog(OF_LOG_ERROR, "error loading font");
    exit(1);
  }    

  if(!g_guiFont->FaceSize(fontsize)){
    ofLog(OF_LOG_ERROR, "Failed to set size");
    exit(1);
  }


  g_guiRenderer = new FTSimpleLayout();
  g_guiRenderer->SetFont(g_guiFont); 
  g_guiRenderer->SetLineLength(300);
  g_guiRenderer->SetAlignment(FTGL::ALIGN_CENTER);
  g_guiRenderer->SetLineSpacing(.9F);
  
  fontsize = 12;
  
  // this is for twitter
  g_twitterFont = new FTTextureFont(ofToDataPath("fonts/ArialUnicode.ttf").c_str());
  g_twitterFont->Outset(0.0f, fontsize);
  g_twitterFont->CharMap(ft_encoding_unicode);
  if(g_twitterFont->Error()) {
    ofLog(OF_LOG_ERROR, "error loading font");
    exit(1);
  }    
  if(!g_twitterFont->FaceSize(fontsize)) {
    ofLog(OF_LOG_ERROR, "Failed to set size");
    exit(1);
  }
  g_twitterLayout = new FTSimpleLayout();
  g_twitterLayout->SetFont(g_twitterFont); 
  g_twitterLayout->SetLineLength(BALOON_MSG_SIZE);
  g_twitterLayout->SetAlignment(FTGL::ALIGN_JUSTIFY);
  g_twitterLayout->SetLineSpacing(BALOON_MWG_LINESPACING);

  // this is for twilio
  g_twilioFont = new FTTextureFont(ofToDataPath("fonts/ArialUnicode.ttf").c_str());
  g_twilioFont->Outset(0.0f, fontsize);
  g_twilioFont->CharMap(ft_encoding_unicode);
  if(g_twilioFont->Error()) {
    ofLog(OF_LOG_ERROR, "error loading font");
    exit(1);
  }    
  if(!g_twilioFont->FaceSize(fontsize)) {
    ofLog(OF_LOG_ERROR, "Failed to set size");
    exit(1);
  }
  g_twilioLayout = new FTSimpleLayout();
  g_twilioLayout->SetFont(g_twilioFont); 
  g_twilioLayout->SetLineLength(BALOON_MSG_SIZE);
  g_twilioLayout->SetAlignment(FTGL::ALIGN_JUSTIFY);
  g_twilioLayout->SetLineSpacing(BALOON_MWG_LINESPACING);

  // this is for twilio
  g_databaseFont = new FTTextureFont(ofToDataPath("fonts/ArialUnicode.ttf").c_str());
  g_databaseFont->Outset(0.0f, fontsize);
  g_databaseFont->CharMap(ft_encoding_unicode);
  if(g_databaseFont->Error()) {
    ofLog(OF_LOG_ERROR, "error loading font");
    exit(1);
  }    
  if(!g_databaseFont->FaceSize(fontsize)) {
    ofLog(OF_LOG_ERROR, "Failed to set size");
    exit(1);
  }
  g_databaseLayout = new FTSimpleLayout();
  g_databaseLayout->SetFont(g_databaseFont); 
  g_databaseLayout->SetLineLength(BALOON_MSG_SIZE);
  g_databaseLayout->SetAlignment(FTGL::ALIGN_JUSTIFY);
  g_databaseLayout->SetLineSpacing(BALOON_MWG_LINESPACING);
}

void g_setTwitterMsgFontSize(int fontsize) {
  g_twitterFont->Outset(0.0f, fontsize);
  g_twitterFont->FaceSize(fontsize);
}
void g_setTwilioMsgFontSize(int fontsize) {
  g_twilioFont->Outset(0.0f, fontsize);
  g_twilioFont->FaceSize(fontsize);
}

void g_setDatabaseMsgFontSize(int fontsize) {
  g_databaseFont->Outset(0.0f, fontsize);
  g_databaseFont->FaceSize(fontsize);
}



void g_initializeBranchImages() {  
  g_changeBranchImages = false;
    
  ofDirectory texturesDir(ofToDataPath(""));
  texturesDir.listDir( "linesTex" );
  std::vector<ofFile> files = texturesDir.getFiles();
  //std::map<std::string, int>& ax = g_getTextAnchorX();
  //std::map<std::string, int>& ay = g_getTextAnchorY();
  g_images.resize(files.size());
  g_originalImages.resize(files.size());
  for (int i = 0; i < files.size(); ++i) {
    g_originalImages[i] = new ofImage(files[i].getAbsolutePath());
    g_images[i] = new ofImage(*(g_originalImages[i]));
    //mimages[i]->setAnchorPoint(5, 0);
  }
}

void g_initializeBackgroundImages() {  
  ofDirectory texturesDir(ofToDataPath(""));
  texturesDir.listDir( "backgrounds" );
  std::vector<ofFile> files = texturesDir.getFiles();
  g_backgroundImages.resize(files.size());
  for (int i = 0; i < files.size(); ++i)
    g_backgroundImages[i] = new ofImage(files[i].getAbsolutePath());
}


void g_initializeFlowerImages() {  
  g_flowerImage.loadImage(ofToDataPath("graphics/flowers/flower1.png"));
}

void g_initializeLeafImages() {  
    
  ofDirectory texturesDir(ofToDataPath(""));
  texturesDir.listDir( "graphics/leaves" );
  std::vector<ofFile> files = texturesDir.getFiles();
  //std::map<std::string, int>& ax = g_getTextAnchorX();
  //std::map<std::string, int>& ay = g_getTextAnchorY();
  g_leafImages.resize(files.size());
  for (int i = 0; i < files.size(); ++i) {
    g_leafImages[i] = new ofImage(files[i].getAbsolutePath());
    //mimages[i]->setAnchorPoint(5, 0);
  }
}


void g_releaseBranchImages() {
  for (int i = 0; i < g_images.size(); ++i) {
    delete g_originalImages[i];
    delete g_images[i];
  }
}

void g_releaseLeafImages() {
  for (int i = 0; i < g_leafImages.size(); ++i) {
    delete g_leafImages[i];
  }
}


// colorize function over the pixels of the stored textures
void g_colorizeBranchImages() {
  for (int i = 0; i < g_images.size(); ++i) {
    kplColorize(g_originalImages[i], g_images[i], g_treeC);
  }
}

void g_computeMaxBranches() {
  g_maxBranches = 0;
  for (int i = 0; i < g_maxLevel; ++i)
    g_maxBranches += 2 + (i + g_fertility) / 3;
}

#ifdef XTREES_APP

void g_updateBackground() {
  ofBackground(g_backgroundC);
  g_app->m_fadeRectangle.SetColor(g_backgroundC);
}

void g_activateSoundtrack() {
  //if (!g_useInteractiveAudio) {
    //g_app->soundtrack.unloadSound();
    //g_app->soundtrack.loadSound("soundtracks/" + g_soundtrack, true);
  //}
}

void g_activateFlowers() {
  if (!g_showFlowers) {
    g_app->m_flowersLayer->stop();
  } else {
    g_app->m_flowersLayer->start();
  }
}
#endif


