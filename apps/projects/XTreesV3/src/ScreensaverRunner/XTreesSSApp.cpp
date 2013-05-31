/**
 *  testApp.cpp
 *
 *  Created by Marek Bereza on 09/01/2012.
 */

#include "XTreesSSApp.h"
#include "Settings.h"


void testApp::setup() {
  g_app = this;
  g_fps = 30;

  ofEnableSmoothing();
  ofSetFrameRate(g_fps);
  ofSetRectMode(OF_RECTMODE_CORNER);  
  ofSetCircleResolution(100);  
  glEnable( GL_LINE_SMOOTH );
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  g_backgroundC = ofColor(0);
  g_treeC = ofColor(255);
  g_jointsAreCircles = true;
  g_jointC = ofColor(0,0,255);
  g_branchWidth = 16;
  g_branchLength = (float)ofGetHeight() / 3.7F;
  g_lengthDecrease = .80;
  g_widthDecrease = .76;
  g_pastTweets = 10;
  g_jointThickness = 3;
  g_jointLength = 12;
  g_tweetMsgC = ofColor(255);
  g_tweetMsgSize = 14;
  g_tweetUsC = ofColor(255, 255, 255);
  g_tLineC = ofColor(255);
  g_tLineWidth = 1;
  g_growthMin = 1750;
  g_growthMax = 2950;
  g_timedExhibit = true;;
  g_showDuration = 10;
  g_minFrequency = 2000;
  g_maxFrequency = 3637;
  g_minAngle = 15;
  g_maxAngle = 35;
  g_showThumbs = true;
  g_fertility = 3;
  g_maxLevel = 13;
  g_shapeV1 = false;
  g_floatingSpeed = 10.F/300.F;
  g_webbyMin = 2840;
  g_webbyMax = 4616;
  g_crazyLineWidth = 2;
  g_crazyCircleWidth = 10;
  g_useBackground = true;
  g_tweetMinLife = 10;
  g_tweetMaxLife = 50;
  g_scene = ofRectangle(0,0,ofGetWidth(), ofGetHeight());
  g_backgroundImage = "Backgrounds/grunge_wall_2.jpg";
  g_minColonization = 70;
  g_computeMaxBranches();
  
  if (g_useBackground)
    background.loadImage(g_backgroundImage);
  
  g_initializeBranchImages();  
  g_initializeFonts();
  
  m_twitterLayer = new TwitterLayer();
  m_treesLayer = new TreesManager(m_twitterLayer);
  m_webbyLayer = new WebbyLayer(m_treesLayer);
  
	ofBackground(g_backgroundC);
  XTree::initFbo();
  
  int increment = ofGetWidth() / (g_numberOfseeds + 1);
  int runningIncrement = increment;
  int currentX;
  int currentY;
  float direction;
  for (int i = 0; i < g_numberOfseeds; ++i, runningIncrement += increment) {
    currentX = runningIncrement + ofRandom(-increment/5, increment/5);
    currentY = ofGetHeight() - 60 + ofRandom(-60/5, 60/5);
    direction = -M_PI /2. + ofRandom(-M_PI/20, M_PI/20);
    switch (i) {
      case 0:
        m_treesLayer->addTree(currentX, currentY, g_seed1, direction);
        
        break;
      case 1:
        m_treesLayer->addTree(currentX, currentY, g_seed2, direction);
        break;
      case 2:
        m_treesLayer->addTree(currentX, currentY, g_seed3, direction);
        break;
      case 3:
        m_treesLayer->addTree(currentX, currentY, g_seed4, direction);
        break;
      case 4:
        m_treesLayer->addTree(currentX, currentY, g_seed5, direction);
        break;
      default:
        break;
    }
  }
  m_treesLayer->start();
  m_webbyLayer->start();
}

void testApp::update() {
  m_treesLayer->update();
  m_twitterLayer->update();
  m_webbyLayer->update();
}

void testApp::draw() {
  ofSetColor(255, 255, 255, 255);  
  if (g_useBackground)
    background.draw(0,0,ofGetWidth(), ofGetHeight());
  m_treesLayer->draw();
  m_twitterLayer->draw();
  m_webbyLayer->draw();
	//ofDrawBitmapString("ciao mondo", ofPoint(100, 200));
}