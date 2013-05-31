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

#pragma once

#include "ofMain.h"
#include "TreesManager.h"
#include "InteractiveAudio.h"
#include "ofxApp.h"
#include "XTreeButton.h"
#include "AlphaRect.h"

#include <tr1/memory>

#ifdef XTREE_EXHIBITION
  #include "OscReceiver.h"
#endif

class FlowersLayer;
class TwitterLayer;
class LeavesLayer;
class LinesLayer;

class testApp : public ofxApp{
	public:
    testApp();
    ~testApp();
    
		void setup();
		void update();
		void draw();
    
    void mousePressed(ofMouseEventArgs& args);
    void mouseMoved(ofMouseEventArgs& args);
    void mouseDragged(ofMouseEventArgs& args);
    void keyPressed(ofKeyEventArgs& args);
    void windowResized(int w, int h);

#if USE_PDLIB
		void audioReceived(float * input, int bufferSize, int nChannels);
		void audioRequested(float * output, int bufferSize, int nChannels);
#endif
    
    
  ofImage m_splash;  
  ofVec2f m_splashPosition;

  AlphaRect m_fadeRectangle;
  
  ofSoundPlayer soundtrack;
  FlowersLayer* m_flowersLayer;  
  
private:
  void start();
  void stop();

  void goClicked(bool& do_);    
  void resetClicked(bool& do_);  
  void controlChanged(guiCallbackData & data);
  
  void setupControlPanel();
  
  
  void updateSimulationState();
  
  enum GuiState {
    STATE_SETUP,
    STATE_GUI,
    STATE_SIMULATION
  };
  
  enum TimelineStates {
    TIME_GROWING, // starts growing trees
    TIME_LEAVES, // stop trees, start leaves
    TIME_LINES_TRANS, // wait some more time after leaves have fallen
    TIME_LINES, // starts the lines
    TIME_LEAVESFALLING_TRANS, // stops for N seconds with all the leaves in place
    TIME_LEAVESFALLING, // leaves fall, 
    TIME_LINES_FAST, // when no leaves are anymore present, accelerate lines
    TIME_STOP_LINES, // stop
    TIME_REGENERATE, // regenerate
    TIME_GROWING_TRANS, // wait some time before restarting
  };
  
  
  GuiState m_state;

  std::tr1::shared_ptr<TreesManager> m_treesLayer;
  std::tr1::shared_ptr<TwitterLayer> m_twitterLayer;
  std::tr1::shared_ptr<LinesLayer> m_linesLayer;
  std::tr1::shared_ptr<LeavesLayer> m_leavesLayer;
  
  std::auto_ptr<XTreeButton> m_goBtn;
  std::auto_ptr<XTreeButton> m_resetBtn;
  
  ofColor m_layerObfuscationColor;
  simpleFileLister m_backgroundsLister;
  simpleFileLister m_soundtracksLister;
  bool m_internetOk;
  
  int m_backgroundFadeTime;
  
  int m_back1, m_back2;
  
  float m_maxThreads;
  
  bool m_screenshot;
  
  TimelineStates m_simulationState;
  
  unsigned int m_simulationTimer;
  
  unsigned int m_numberOfIterations;
  
  bool m_splashActive;  
  float m_splashOpacity;
  unsigned int m_splashEnd;
  
#ifdef XTREE_EXHIBITION
  OscReceiver m_receiver;
#endif
  
};
