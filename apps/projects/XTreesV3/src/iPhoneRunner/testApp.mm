#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	
	// register touch events
	ofRegisterTouchEvents(this);
	
	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	//iPhoneAlerts will be sent to this.
	ofxiPhoneAlerts.addListener(this);
	
	//If you want a landscape oreintation 
	iPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT);
	
  //g_app = this;
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
  g_tLineC = ofColor(255, 255, 255);
  g_tLineWidth = 3;
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
  
  g_treesLayerOpacity = 255;
  g_twitterLayerOpacity = 255;
  g_splashOpacity = 255;
  g_flowersOpacity = 255;

  g_computeMaxBranches();
  
  //if (g_useBackground)
    //background.loadImage(g_backgroundImage);
  
  g_initializeBranchImages();  
  g_initializeFonts();
  g_initializeFlowerImages();
  
  m_twitterLayer = new TwitterLayer();
  m_treesLayer = new TreesManager(m_twitterLayer);
  m_webbyLayer = new WebbyLayer(m_treesLayer);
  g_leavesLayer = m_leavesLayer = new LeavesLayer();
  
	ofBackground(g_backgroundC);
  XTree::initFbo();
  
  int increment = ofGetWidth() / (g_numberOfseeds + 1);
  int runningIncrement = increment;
  int currentX;
  int currentY;
  float direction;
  m_treesLayer->addTree(ofGetWidth()/2, ofGetHeight() - 30, "ciao", 3./2. * M_PI);
  m_treesLayer->start();
  m_webbyLayer->start();
}

//--------------------------------------------------------------
void testApp::update(){
  m_twitterLayer->update();
  m_treesLayer->update();
  m_webbyLayer->update();
}

//--------------------------------------------------------------
void testApp::draw(){
	m_treesLayer->draw();
  m_webbyLayer->draw();
  m_twitterLayer->draw();
}

//--------------------------------------------------------------
void testApp::exit(){

}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::lostFocus(){

}

//--------------------------------------------------------------
void testApp::gotFocus(){

}

//--------------------------------------------------------------
void testApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void testApp::deviceOrientationChanged(int newOrientation){

}


//--------------------------------------------------------------
void testApp::touchCancelled(ofTouchEventArgs& args){

}

