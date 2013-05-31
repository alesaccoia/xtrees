/*
 *  FpsWatchDog.h
 *  TwitterRetriever
 *
 *  Created by Alessandro Saccoia on 1/20/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef FPSWATCHDOG_H_
#define FPSWATCHDOG_H_

#include "ofMain.h"
#include "ofxTimer.h"
#include "ofEvents.h"

#define MIN_PERIOD 1000
#define MAX_PERIOD 3000

#define THRESHOLD 5.F

class FpsWatchDog {
public:
  enum State {
    OK,
    UNDER
  };

  FpsWatchDog(float requiredFps_) :
    m_requiredFps(requiredFps_),
    m_state(OK),
    m_threshold(m_requiredFps * THRESHOLD / 100.F) {
    ofAddListener(ofEvents.update, this, &FpsWatchDog::update);
  }
  
  ~FpsWatchDog() {
    
  }
  
  void update(ofEventArgs& args) {
    m_currentFps = ofGetFrameRate();
    if (m_currentFps < m_requiredFps) {
      if (m_currentFps - m_requiredFps < m_threshold) {
        return;
      }
      if (m_state == OK) {
        m_lastWrongFps = m_currentFps;
        m_toSignal = ofClamp(1.F - (m_currentFps / m_requiredFps) * 5.F, .0F, 1.F);
        ofNotifyEvent(badFpsEvent, m_toSignal ,this);
        m_timer.set();
        m_timer.setAlarm(ofRandom(MIN_PERIOD, MAX_PERIOD));
      } else {
        if (m_timer.alarm()) {
        
          if (m_currentFps < m_lastWrongFps) {
            m_toSignal = ofClamp(1.F - (m_currentFps / m_requiredFps) * 10.F, .0F, 1.F);
            ofNotifyEvent(badFpsEvent, m_toSignal,this);
          }
          
          //set a new timer
          m_timer.set();
          m_timer.setAlarm(ofRandom(MIN_PERIOD, MAX_PERIOD));
        }
      }
    } else {
      m_state = OK;
    }
  }
  
  ofEvent<float> badFpsEvent; // ratio

private:
  float m_requiredFps;
  float m_lastWrongFps;
  float m_currentFps;
  float m_toSignal; // avoid the stack every update
  float m_threshold;
  ofxTimer m_timer;
  State m_state;
  
};



#endif
