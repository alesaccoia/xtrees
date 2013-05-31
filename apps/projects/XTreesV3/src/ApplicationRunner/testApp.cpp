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

#include "testApp.h"


#include "Settings.h"
#include "GraphicUtilities.hpp"
#include "XTree.h"
#include "XTreeBranch.h"
#include "TwitterTrigger.h"
#include "FlowersLayer.h"
#include "LinesLayer.h"
#include "TwitterLayer.h"
#include "LeavesLayer.h"

#include "RealtimeFetcher.h"

#define MIN_X 150
#define MIN_Y 210

//--------------------------------------------------------------

float vSavedLinesSpeedMin;
float vSavedLinesSpeedMax;

testApp::testApp() :
  m_state(STATE_SETUP) {
}


void testApp::setup(){
  m_screenshot = false;
  g_activeThreads = 0;
  g_app = this;
  g_fps = 30;
  m_simulationState = TIME_GROWING;
  m_simulationTimer = 0;
  m_numberOfIterations = 0;
  
  ofEnableSmoothing();
  ofSetFrameRate(g_fps);
  ofSetRectMode(OF_RECTMODE_CORNER);  
  ofSetCircleResolution(100);  
  glEnable( GL_LINE_SMOOTH );
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  ofBackground(0);
	ofSetVerticalSync(false);
	
  m_internetOk = true;
  m_splashActive = false;
  
  if (!CheckInternetConnection()) {
    m_internetOk = false;
    return;
  }
  
  // needs to be called after the settings have been loaded!
  g_initializeBranchImages();  
  g_initializeFonts();
  g_initializeFlowerImages();
  g_initializeLeafImages();
  g_initializeBackgroundImages();
  TwitterBaloon::initFbo();
  XTree::initFbo();
  
  m_backgroundFadeTime = 1;
  g_useTwilio = true;
  
  std::ifstream myfile ("../../../data/archive.txt");
  if (myfile.is_open())
  {
    g_useArchive = true;
    myfile.close();
  } else {
    g_useArchive = false;
  }
  
  m_leavesLayer.reset(new LeavesLayer());
  g_leavesLayer = m_leavesLayer.get();
  m_twitterLayer.reset(new TwitterLayer());
  m_treesLayer.reset(new TreesManager(m_twitterLayer));
//  m_flowersLayer = new FlowersLayer(m_treesLayer);
  m_linesLayer.reset(new LinesLayer(m_treesLayer));
  
  m_goBtn.reset(new XTreeButton("Go", 80, 50, 30));
  m_resetBtn.reset(new XTreeButton("Reset", 80, 150, 30));
  

  
  g_useInteractiveAudio = true;
  
  if (g_useInteractiveAudio) {    
    InteractiveAudio::instance()->init(this);  
    InteractiveAudio::instance()->start();  
  }

  
  setupControlPanel();
  controlPanel.loadSettings(ofToDataPath("configurations/default.xml"));
  
  
  if(use_background) {
    int size = g_backgroundImages.size();
    if (size == 0) {
      use_background = false;
    } else {
      m_back1 = kplToss(g_backgroundImages.size());
      m_back2 = kplToss(g_backgroundImages.size());
    }

  }
  
  ofAddListener(ofEvents.mouseMoved, this, &testApp::mouseMoved);
  ofAddListener(ofEvents.mousePressed, this, &testApp::mousePressed);
  ofAddListener(ofEvents.mouseDragged, this, &testApp::mouseDragged);
  ofAddListener(ofEvents.keyPressed, this, &testApp::keyPressed);
  
  ofAddListener(m_goBtn->clickedEvent, this,  &testApp::goClicked);
  ofAddListener(m_resetBtn->clickedEvent, this,  &testApp::resetClicked);
  
  /* signal the soundsystem how many branches will grow at the same time
  // to avoid clipping and decide the mix parameters  
  if (g_useInteractiveAudio) {    
    InteractiveAudio::instance()->sendFloat(kBranchPolyphony, 
      ofClamp(ceil((float)g_growthMax / (float)g_growthMin), 1, 4));
    InteractiveAudio::instance()->sendFloat(kActivateBirds, 0);
    InteractiveAudio::instance()->sendFloat(kActivateVinyl, 0);
    InteractiveAudio::instance()->sendFloat(kActivateRand1, 0);
  }
  */
  
  InteractiveAudio::instance()->sendBang(kStart);
  
  m_fadeRectangle.init(ofGetWidth(), ofGetHeight() / 7);
  // to set the color of the fading rectangle
  g_updateBackground();
  
  g_scene = ofRectangle(0,0,ofGetWidth(), ofGetHeight());
  g_sceneBounded = ofRectangle(20,0,ofGetWidth()-20, ofGetHeight());
  m_layerObfuscationColor  = ofColor(0);  
  
  m_splash.loadImage(ofToDataPath("splash/splash.png"));
    
  m_splashPosition.x = std::max(ofGetWidth()/2 - m_splash.getWidth()/2, m_splash.getWidth()/2);
  m_splashPosition.y = ofGetHeight()/2 - m_splash.getHeight()/2;
  

  m_goBtn->showImmediately();
  m_resetBtn->showImmediately();
  
#ifdef XTREE_EXHIBITION
  m_receiver.startThread();
#endif


  // save seeds
  m_treesLayer->loadSeeds();
}

//--------------------------------------------------------------
testApp::~testApp() {
  if(!m_internetOk) 
    return;
  // save seeds
  m_treesLayer->saveSeeds();

  if (!m_internetOk) {
    return;
  }
  if (m_state = STATE_SIMULATION) {
    RealtimeFetcher::instance()->stop(); 
  }
#ifdef XTREE_EXHIBITION
  m_receiver.stopThread();
#endif
  g_releaseBranchImages();  

}

void testApp::start() {
  if (!m_internetOk) {
    return;
  }
  
  if (!m_treesLayer->start())
    return;
  g_globalCounterSec = 1;
  
  ofHideCursor();
  
  m_simulationState = TIME_GROWING;
  m_splashActive = false;
  m_numberOfIterations = 0;
  
  RealtimeFetcher::instance()->start();
  
  InteractiveAudio::instance()->sendBang(kBreath);
      
  InteractiveAudio::instance()->sendBang(kSimulOn); 
  
  //ofHideCursor();
  m_goBtn->hide();
  m_resetBtn->hide();
  if (g_showFlowers) {
    //m_flowersLayer->start();
    //m_linesLayer->start();
  }
  //if (g_leavesActive)
    //m_leavesLayer->start();
  //        m_twitterLayer->prestart();
  m_state = STATE_SIMULATION;

  if (!g_useInteractiveAudio) {
    //soundtrack.play();
  }
}

void testApp::stop() {
  if (!m_internetOk) {
    return;
  }
  
  ofShowCursor();

  RealtimeFetcher::instance()->stop();

  
  InteractiveAudio::instance()->sendBang(kSimulOff);
  
  //m_treesLayer->stopGrowing();
  m_treesLayer->stop();
//  m_flowersLayer->stop();
  m_linesLayer->stop();
  m_twitterLayer->stop();
  m_leavesLayer->reset();
  
  m_goBtn->show();
  m_resetBtn->show();
  //ofShowCursor();
  m_state = STATE_SETUP;
  if (!g_useInteractiveAudio) {
    soundtrack.stop();
  }
  m_splashActive = false;
}

//--------------------------------------------------------------
void testApp::keyPressed(ofKeyEventArgs& args) {
  if (STATE_SETUP && g_activeThreads > 0) {
    return;
  }
  if (!m_internetOk) {
    return;
  }
  if (args.key == ',') {
    if (bDebug) {
      if (m_state == STATE_SETUP) {
        m_goBtn->showImmediately();
        m_resetBtn->showImmediately();
      } else {
        ofHideCursor();
      }
    } else {
      if (m_state == STATE_SETUP) {
        m_goBtn->hideImmediately();
        m_resetBtn->hideImmediately();
      } else {
        ofShowCursor();
      }
    }
    bDebug = !bDebug;    
    return;
  }
  switch (m_state) {
    case STATE_SETUP:
      if (args.key == '.') {
        start();
      } else {
        m_treesLayer->keyPressed(args);
      }

      break;
    case STATE_SIMULATION:
      if (args.key == ' ') {
        m_maxThreads = g_activeThreads;
        stop();
      } else if (args.key == 's') {
        m_screenshot = m_screenshot ? false : true;
      }
    default:
      break;
  }
}

//--------------------------------------------------------------
void testApp::update(){
  if (m_screenshot) {
    return;
  }
  if (!m_internetOk) {
    return;
  }
  if (STATE_SETUP && g_activeThreads > 0) {
    return;
  }
  
  RealtimeFetcher::instance()->update();
  
  m_twitterLayer->update();
  m_treesLayer->update();
  m_linesLayer->update();
//  m_flowersLayer->update();
  m_leavesLayer->update();
  if(use_background) {
    ++m_backgroundFadeTime;
    if (m_backgroundFadeTime > g_backgroundTransitionTime * g_fps) {
      m_backgroundFadeTime = 1;
      m_back1 = m_back2;
      m_back2 = kplToss(g_backgroundImages.size());
    }
  }
  // this stuff needs to be done in the main thread!
  if (g_changeBranchImages) {
    g_colorizeBranchImages();
    g_changeBranchImages = false;
  }
  
  // reduce sampling rate for other operations: every 3 seconds
  if (m_state == STATE_SIMULATION && g_globalCounter % (int)g_fps == 0) {
    g_globalCounterSec++;
    g_globalCounter = 0;
    updateSimulationState();
  }
  /*
  if (m_state == STATE_SIMULATION && g_globalCounterSec % g_splashFrequency == 0) {
    // start
    m_splashActive = true;
    m_splashEnd = g_globalCounterSec + g_splashDuration;
    m_splashOpacity = 0;
  }
  
  if(g_globalCounterSec == m_splashEnd) {
    m_splashActive = false;
  }
  
  if(m_splashActive) {
    if (g_globalCounterSec > m_splashEnd - 5) {
      m_splashOpacity -= 0.33 / g_fps;
      m_splashOpacity = std::max(m_splashOpacity, 0.0F);    
    } else {      
      m_splashOpacity += 0.33 / g_fps;
      m_splashOpacity = std::min(m_splashOpacity, 1.0f);
    }

    
  }
  */
  ++g_globalCounter;
}



void testApp::updateSimulationState() {
  if (m_treesLayer->averageMaturationLevel() > g_minColonization) {
    RealtimeFetcher::instance()->pause();
    InteractiveAudio::instance()->sendBang(kMsgOff);
    m_treesLayer->stopGrowing();
  }
  if (m_simulationState == TIME_GROWING) {
    if (m_numberOfIterations < g_firstIterations) {
      if (m_treesLayer->averageMaturationLevel() > g_minColonization) {
        m_simulationTimer = 0;
        m_simulationState = TIME_LEAVESFALLING;
      }    
    } else {
      if (m_treesLayer->averageMaturationLevel() > g_minColonizationForLeaves) {
        m_simulationState = TIME_LEAVES;
        InteractiveAudio::instance()->sendBang(kBreath);
        InteractiveAudio::instance()->sendBang(kLeavesOn);
        m_leavesLayer->start();
      }  
    }
  } else if (m_simulationState == TIME_LEAVES) {
    if (m_leavesLayer->numberOfLeaves() >= g_leavesRows * g_leavesColumns) {
      m_leavesLayer->stop();        
      m_simulationTimer = 0;
      m_simulationState = TIME_LINES_TRANS;
    }
  } else if (m_simulationState == TIME_LINES_TRANS) {
    if (m_simulationTimer >= g_waitLinesTime) { // HERE WE USE A TIMER
      g_linesMin = (float)g_linesMinNorm;
      g_linesMax = (float)g_linesMaxNorm;
        InteractiveAudio::instance()->sendBang(kLinesOn);
      m_linesLayer->start();
      m_simulationTimer = 0;
      m_simulationState = TIME_LINES;
    }
  } else if (m_simulationState == TIME_LINES) {
    if (m_simulationTimer >= g_startDetachingLeavesTime) { // HERE WE USE A TIMER
      m_leavesLayer->setWindForce(0);
      m_leavesLayer->detachLeaves();
      InteractiveAudio::instance()->sendBang(kLeavesOff);
      g_linesMin = (float)g_linesMinAcc;
      g_linesMax = (float)g_linesMaxAcc;
      m_simulationState = TIME_LEAVESFALLING;
      // NB: do not reset the counter
    }
  } else if (m_simulationState == TIME_LEAVESFALLING) {
    if (m_leavesLayer->numberOfLeaves() == 0 && m_twitterLayer->numberOfBaloons() == 0) { // 12 seconds
      m_linesLayer->stop();
      if(m_numberOfIterations >= g_firstIterations) {
        InteractiveAudio::instance()->sendBang(kLinesOff);
      } else {
        InteractiveAudio::instance()->sendBang(kReadyToRegenerate);
      }
      m_simulationTimer = 0;
      m_simulationState = TIME_REGENERATE;
    }
  } else if (m_simulationState == TIME_REGENERATE) {
    if (m_simulationTimer >= g_waitRegenerateTime) {
      m_simulationTimer = 0;
      m_treesLayer->regenerate();
      InteractiveAudio::instance()->sendBang(kRegen);
      if(m_numberOfIterations >= g_firstIterations) {
        m_treesLayer->fadeImagesQuickly();
        m_numberOfIterations = 0;
      } else {        
        ++m_numberOfIterations;
      }      
      InteractiveAudio::instance()->sendBang(kRegen);
      m_simulationState = TIME_GROWING_TRANS;      
    }
  } else if (m_simulationState == TIME_GROWING_TRANS) {
    if (m_simulationTimer == g_waitSeedTime - 3) {
      InteractiveAudio::instance()->sendBang(kBreath);
    }
    if (m_simulationTimer >= g_waitSeedTime) {
      m_treesLayer->restartGrowing();
      RealtimeFetcher::instance()->restart();
      InteractiveAudio::instance()->sendBang(kMsgOn);
      m_simulationState = TIME_GROWING;
    }
  }
  ++m_simulationTimer;

}

void testApp::mousePressed(ofMouseEventArgs& args) {
  if (!m_internetOk || bDebug) {
    return;
  }
  if (STATE_SETUP && g_activeThreads > 0) {
    return;
  }
  Leaf* nleaf;
  switch (m_state) {
    case STATE_SETUP:
      if (!(args.x < MIN_X && args.y < MIN_Y))
        m_treesLayer->mousePressed(args);
      break;
    default:
      break;
  }
}

void testApp::mouseMoved(ofMouseEventArgs& args) {
  if (!m_internetOk || bDebug) {
    return;
  }
  if (STATE_SETUP && g_activeThreads > 0) {
    return;
  }
  switch (m_state) {
    case STATE_SETUP:
      if (!(args.x < MIN_X && args.y < MIN_Y))
        m_treesLayer->mouseMoved(args);
      break;
    default:
      break;
  }
}


void testApp::mouseDragged(ofMouseEventArgs& args) {
  if (!m_internetOk || bDebug) {
    return;
  }
  if (STATE_SETUP && g_activeThreads > 0) {
    return;
  }
  switch (m_state) {
    case STATE_SETUP:
      if (!(args.x < MIN_X && args.y < MIN_Y))
        m_treesLayer->mouseDragged(args);
      break;
    default:
      break;
  }
}

//--------------------------------------------------------------
void testApp::draw(){
  if (m_state == STATE_SETUP && g_activeThreads > 0) {
    ofSetColor(255, 255, 255);
    ofDrawBitmapString("Resetting: " + ofToString(100.f - ((float)g_activeThreads/m_maxThreads) * 100.F) + " %", ofGetWidth()/2 - 200, ofGetHeight()/2);
    return;
  }
  if (!m_internetOk) {
    ofDrawBitmapString("This program requires an active internet connection,", ofGetWidth()/2 - 200, ofGetHeight()/2);
    ofDrawBitmapString("please check your configuration!", ofGetWidth()/2 - 200, ofGetHeight()/2 + 30);
    return;
  }
  pushTransforms();
  
  /*
  if (use_background) {
    float completion = 255 * ((float)m_backgroundFadeTime / (float)(g_backgroundTransitionTime * g_fps));
    ofSetColor(255);
    g_backgroundImages[m_back1]->draw(0,0,ofGetWidth(), ofGetHeight());
    ofSetColor(255, completion);
    g_backgroundImages[m_back2]->draw(0,0,ofGetWidth(), ofGetHeight());
  }
  */
  ofEnableAlphaBlending();
  m_treesLayer->draw(); 
  m_linesLayer->draw();  
  m_leavesLayer->draw();  
  m_twitterLayer->draw();
  m_fadeRectangle.draw();
  
  
  if (m_splashActive) {
    ofSetColor(g_backgroundC, m_splashOpacity * g_splashOpacity);
    ofRect(0, 0, g_scene.width, g_scene.height);
    ofSetColor(255);
    ofSetColor(255,255.0F * m_splashOpacity);
    m_splash.draw(m_splashPosition.x, m_splashPosition.y);
  }
  
  popTransforms();
  
  ofSetLineWidth(1);
}

void testApp::goClicked(bool& do_) {
  if (!m_internetOk) {
    return;
  }
  if (STATE_SETUP && g_activeThreads > 0) {
    return;
  }
  start();
}

void testApp::resetClicked(bool& do_) {
  if (!m_internetOk) {
    return;
  }
  if (STATE_SETUP && g_activeThreads > 0) {
    return;
  }
  RealtimeFetcher::instance()->reset();
  m_treesLayer->reset();
  m_leavesLayer->reset();
}



void testApp::setupControlPanel() {
  
#include "../Settings/controlsCommon.cpp"

  // warp control
  controlPanel.addPanel("Transformer", 1, 0);  
	setRenderSize(ofGetWidth(), ofGetHeight());
	setTransforms(true, true, false, true, true); 
  
	addTransformControls(panelNr,0);
  
  controlPanel.setupEvents();
	controlPanel.enableEvents();
  
  ofAddListener(controlPanel.guiEvent, this, &testApp::controlChanged);
  
}

void testApp::controlChanged(guiCallbackData & data) {
  if (data.isElement("backgroundR")) {
    g_backgroundC.r = data.getInt(0);
    g_updateBackground();
  } else if (data.isElement("backgroundG")) {
    g_backgroundC.g = data.getInt(0);
    g_updateBackground();
  } else if (data.isElement("backgroundB")) {
    g_backgroundC.b = data.getInt(0);
    g_updateBackground();
  } else if (data.isElement("treeR")) {
    g_treeC.r = data.getInt(0);
    g_changeBranchImages = true;
  } else if (data.isElement("treeG")) {
    g_treeC.g = data.getInt(0);
    g_changeBranchImages = true;
  } else if (data.isElement("treeB")) {
    g_treeC.b = data.getInt(0);
    g_changeBranchImages = true;
  } else if (data.isElement("treeA")) {
    g_treeC.a = data.getInt(0);
    g_changeBranchImages = true;
  } else if (data.isElement("jointsAreCircles")) {
    g_jointsAreCircles = data.getInt(0);
  }  else if (data.isElement("jointR")) {
    g_jointC.r = data.getInt(0);
  }  else if (data.isElement("jointG")) {
    g_jointC.g = data.getInt(0);
  }  else if (data.isElement("jointB")) {
    g_jointC.b = data.getInt(0);
  } else if (data.isElement("branchWidth")) {
    g_branchWidth = data.getFloat(0);
  } else if (data.isElement("branchMinWidth")) {
    g_branchMinWidth = data.getFloat(0);
  } else if (data.isElement("branchLength")) {
    g_branchLength = data.getFloat(0);
  } else if (data.isElement("widthDecrease")) {
    g_widthDecrease = data.getFloat(0) / 100.F;
  } else if (data.isElement("lengthDecrease")) {
    g_lengthDecrease = data.getFloat(0) / 100.F;
  } else if (data.isElement("pastTweets")) {
    g_pastTweets = data.getInt(0);
  } else if (data.isElement("jointThickness")) {
    g_jointThickness = data.getInt(0);
  } else if (data.isElement("jointLength")) {
    g_jointLength = data.getInt(0);
  }  else if (data.isElement("tweetMsgR")) {
    g_tweetMsgC.r = data.getInt(0);
  } else if (data.isElement("tweetMsgG")) {
    g_tweetMsgC.g = data.getInt(0);
  } else if (data.isElement("tweetMsgB")) {
    g_tweetMsgC.b = data.getInt(0);
  } else if (data.isElement("tweetMsgSize")) {
    g_setTwitterMsgFontSize(data.getInt(0));
  }  else if (data.isElement("tweetUsR")) {
    g_tweetUsrC.r = data.getInt(0);
  } else if (data.isElement("tweetUsG")) {
    g_tweetUsrC.g = data.getInt(0);
  } else if (data.isElement("tweetUsB")) {
    g_tweetUsrC.b = data.getInt(0);
  }  else if (data.isElement("twilioMsgR")) {
    g_twilioMsgC.r = data.getInt(0);
  } else if (data.isElement("twilioMsgG")) {
    g_twilioMsgC.g = data.getInt(0);
  } else if (data.isElement("twilioMsgB")) {
    g_twilioMsgC.b = data.getInt(0);
  } else if (data.isElement("twilioMsgSize")) {
    g_setTwilioMsgFontSize(data.getInt(0));
  } else if (data.isElement("twilioUsR")) {
    g_twilioUsrC.r = data.getInt(0);
  } else if (data.isElement("twilioUsG")) {
    g_twilioUsrC.g = data.getInt(0);
  } else if (data.isElement("twilioUsB")) {
    g_twilioUsrC.b = data.getInt(0);
  } else if (data.isElement("databaseMsgR")) {
    g_databaseMsgC.r = data.getInt(0);
  } else if (data.isElement("databaseMsgG")) {
    g_databaseMsgC.g = data.getInt(0);
  } else if (data.isElement("databaseMsgB")) {
    g_databaseMsgC.b = data.getInt(0);
  } else if (data.isElement("databaseMsgSize")) {
    g_setDatabaseMsgFontSize(data.getInt(0));
  }  else if (data.isElement("databaseUsR")) {
    g_databaseUsrC.r = data.getInt(0);
  } else if (data.isElement("databaseUsG")) {
    g_databaseUsrC.g = data.getInt(0);
  } else if (data.isElement("databaseUsB")) {
    g_databaseUsrC.b = data.getInt(0);
  } else if (data.isElement("tLineR")) {
    g_tLineC.r = data.getInt(0);
  } else if (data.isElement("tLineG")) {
    g_tLineC.g = data.getInt(0);
  } else if (data.isElement("tLineB")) {
    g_tLineC.b = data.getInt(0);
  } else if (data.isElement("tLineWidth")) {
    g_tLineWidth = data.getInt(0);
  } else if (data.isElement("growthMin")) {
    g_growthMin = data.getInt(0);
  } else if (data.isElement("growthMax")) {
    g_growthMax = data.getInt(0);
  } else if (data.isElement("timedExhibit")) {
    g_timedExhibit = data.getInt(0);
  } else if (data.isElement("showDuration")) {
    g_showDuration = data.getInt(0);
  } else if (data.isElement("minFrequency")) {
    g_minFrequency = data.getInt(0);
  } else if (data.isElement("maxFrequency")) {
    g_maxFrequency = data.getInt(0);
  } else if (data.isElement("minAngle")) {
    g_minAngle = data.getInt(0);
  } else if (data.isElement("maxAngle")) {
    g_minAngle = data.getInt(0);
  } else if (data.isElement("showThumbs")) {
    g_showThumbs = data.getInt(0);
  } else if (data.isElement("fertility")) {
    g_fertility = data.getInt(0);
    g_computeMaxBranches();
  }  else if (data.isElement("maxLevel")) {
    g_maxLevel = data.getInt(0);
    g_computeMaxBranches();
  } else if (data.isElement("floatingSpeed")) {
    g_floatingSpeed = data.getFloat(0) / 300.F;
  } else if (data.isElement("splashR")) {
    g_splashC.r = data.getInt(0);
  } else if (data.isElement("splashG")) {
    g_splashC.g = data.getInt(0);
  } else if (data.isElement("splashB")) {
    g_splashC.b = data.getInt(0);
  } else if (data.isElement("soundVolume")) {
    g_soundVolume = data.getFloat(0) / 100.F;
  } else if (data.isElement("flowersMin")) {
    g_flowersMin = data.getInt(0);
  } else if (data.isElement("flowersMax")) {
    g_flowersMax = data.getInt(0);
  } else if (data.isElement("crazyLineWidth")) {
    g_crazyLineWidth = data.getInt(0);
  } else if (data.isElement("crazyCircleWidth")) {
    g_crazyCircleWidth = data.getInt(0);
  } else if (data.isElement("useBackgroundImage")) {
    use_background = data.getInt(0);
  }/* else if (data.isElement("useInteractiveAudio")) {
    g_useInteractiveAudio = data.getInt(0);
    g_activateSoundtrack();
  }*/ else if (data.isElement("Soundtrack")) {
    g_soundtrack = data.getString(2);
    g_activateSoundtrack();
  } else if (data.isElement("minColonization")) {
    g_minColonization = data.getFloat(0);
  } else if (data.isElement("minColonizationForLeaves")) {
    g_minColonizationForLeaves = data.getFloat(0);
  } else if (data.isElement("treesOpacity")) {
    g_treesLayerOpacity = data.getFloat(0);
  } else if (data.isElement("twitterOpacity")) {
    g_twitterLayerOpacity = data.getFloat(0);
  } else if (data.isElement("splashOpacity")) {
    g_splashOpacity = data.getFloat(0);
  } else if (data.isElement("flowersOpacity")) {
    g_flowersOpacity = data.getFloat(0);
  } else if (data.isElement("showFlowers")) {
    g_showFlowers = data.getInt(0);
  } else if (data.isElement("tLeavesR")) {
    g_leavesEndColor.r = data.getInt(0);
  } else if (data.isElement("tLeavesG")) {
    g_leavesEndColor.g = data.getInt(0);
  } else if (data.isElement("tLeavesB")) {
    g_leavesEndColor.b = data.getInt(0);
  } else if (data.isElement("tLeavesA")) {
    g_leavesOpacity = data.getFloat(0);
  } else if (data.isElement("leafFertility")) {
    g_leavesFertility = data.getFloat(0);
  } else if (data.isElement("leafMinWidth")) {
    g_leavesMinWidth = data.getFloat(0);
  } else if (data.isElement("leafMaxWidth")) {
    g_leavesMaxWidth = data.getFloat(0);
  } else if (data.isElement("tweetMin")) {
    g_tweetMinLife = data.getFloat(0);
  } else if (data.isElement("tweetMax")) {
    g_tweetMaxLife = data.getFloat(0);
  }/* else if (data.isElement("useTwilio")) {
    g_useTwilio = data.getInt(0);
  }*/ else if (data.isElement("leavesRows")) {
    g_leavesRows = data.getInt(0);
  } else if (data.isElement("leavesColumns")) {
    g_leavesColumns = data.getInt(0);
  } else if (data.isElement("showLeaves")) {
    g_leavesActive = data.getInt(0);
  } else if (data.isElement("leavesMinFreq")) {
    g_leavesMinFreq = data.getInt(0);
  } else if (data.isElement("leavesMaxFreq")) {
    g_leavesMaxFreq = data.getInt(0);
  } else if (data.isElement("treesFadeTime")) {
    g_treesFadeTime = data.getInt(0);
  } else if (data.isElement("backgroundTransitionTime")) {
    g_backgroundTransitionTime = data.getInt(0);
  } else if (data.isElement("tFlowersR")) {
    g_flowersColor.r = data.getInt(0);
  } else if (data.isElement("tFlowersG")) {
    g_flowersColor.g = data.getInt(0);
  } else if (data.isElement("tFlowersB")) {
    g_flowersColor.b = data.getInt(0);
  } else if (data.isElement("tFlowersLinesR")) {
    g_flowersLineColor.r = data.getInt(0);
  } else if (data.isElement("tFlowersLinesG")) {
    g_flowersLineColor.g = data.getInt(0);
  } else if (data.isElement("tFlowersLinesB")) {
    g_flowersLineColor.b = data.getInt(0);
  } else if (data.isElement("waitSeedTime")) {
    g_waitSeedTime = data.getInt(0);
  } else if (data.isElement("waitLinesTime")) {
    g_waitLinesTime = data.getInt(0);
  } else if (data.isElement("startDetachingLeavesTime")) {
    g_startDetachingLeavesTime = data.getInt(0);
  } else if (data.isElement("waitRegenerateTime")) {
    g_waitRegenerateTime = data.getInt(0);
  } else if (data.isElement("leavesLife")) {
    g_leavesLife = data.getInt(0);
  } else if (data.isElement("msgOpacity")) {
    g_msgOpacity = data.getInt(0);
  } else if (data.isElement("LinesMin")) {
    g_linesMin = data.getInt(0);
    g_linesMinNorm = data.getInt(0);
    g_linesMinAcc = g_linesMinNorm * 1.5;
  } else if (data.isElement("LinesMax")) {
    g_linesMax = data.getInt(0);
    g_linesMaxNorm = data.getInt(0);
    g_linesMaxAcc = g_linesMaxNorm * 1.5;
  } else if (data.isElement("firstIterations")) {
    g_firstIterations = data.getInt(0);
  } else if (data.isElement("splashFrequency")) {
    g_splashFrequency = data.getInt(0);
  } else if (data.isElement("splashDuration")) {
    g_splashDuration = data.getInt(0);
  } else if (data.isElement("splashOpacity")) {
    g_splashOpacity = data.getFloat(0);
  } else if (data.isElement("eq100")) {
    InteractiveAudio::instance()->sendFloat(kEq100, data.getFloat(0) / 100.F);
  } else if (data.isElement("eq250")) {
    InteractiveAudio::instance()->sendFloat(kEq250, data.getFloat(0) / 100.F);
  } else if (data.isElement("eq700")) {
    InteractiveAudio::instance()->sendFloat(kEq700, data.getFloat(0) / 100.F);
  } else if (data.isElement("eq1500")) {
    InteractiveAudio::instance()->sendFloat(kEq1500, data.getFloat(0) / 100.F);
  } else if (data.isElement("eq3000")) {
    InteractiveAudio::instance()->sendFloat(kEq3000, data.getFloat(0) / 100.F);
  } else if (data.isElement("eq7000")) {
    InteractiveAudio::instance()->sendFloat(kEq7000, data.getFloat(0) / 100.F);
  } else if (data.isElement("eq15000")) {
    InteractiveAudio::instance()->sendFloat(kEq15000, data.getFloat(0) / 100.F);
  } else if (data.isElement("eqGain")) {
    InteractiveAudio::instance()->sendFloat(kEqGain, data.getFloat(0) / 100.F);
  }
  
}

void testApp::windowResized(int w, int h) {
}

#if USE_PDLIB
//--------------------------------------------------------------
void testApp::audioReceived(float * input, int bufferSize, int nChannels) {
	InteractiveAudio::instance()->audioReceived(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void testApp::audioRequested(float * output, int bufferSize, int nChannels) {
	InteractiveAudio::instance()->audioRequested(output, bufferSize, nChannels);
  float *thisOut = output;
  int numberOfSamples = bufferSize * nChannels;
  for (int i = 0; i < numberOfSamples; ++i, ++thisOut) {
    *thisOut *= g_soundVolume;
  }
}
#endif

