/*
 *  FooTrigger.cpp
 *  TwitterRetriever
 *
 *  Created by Alessandro Saccoia on 1/18/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "FooTrigger.h"

FooTrigger::FooTrigger() {
 
}

FooTrigger::~FooTrigger() {
 
}

void FooTrigger::activate() {
  ofAddListener(ofEvents.update, this, &FooTrigger::update);
  m_timer.set();
  m_timer.setAlarm(ofRandom(m_minPeriod, m_maxPeriod));
}

void FooTrigger::deactivate() {
  ofRemoveListener(ofEvents.update, this, &FooTrigger::update);
  m_timer.stop();
}
  
void FooTrigger::update(ofEventArgs& args) {
  if (m_timer.alarm()) {
    MessageEvent* theEvent = new MessageEvent();
    theEvent->message = "Ciao bello come va";
    ofNotifyEvent(newMessageEvent,theEvent,this);    
    //set a new timer
    m_timer.set();
    m_timer.setAlarm(ofRandom(m_minPeriod, m_maxPeriod));
  }
}

