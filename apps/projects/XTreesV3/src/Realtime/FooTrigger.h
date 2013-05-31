/*
 *  FooTrigger.h
 *  TwitterRetriever
 *
 *  Created by Alessandro Saccoia on 1/18/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 *
 *  Class for testing purposes, the queue is not touched
 *
 */


#ifndef FooTrigger_H_
#define FooTrigger_H_

#include "MessageTrigger.h"
#include "ofxTimer.h"

class FooTrigger :
  public MessageTrigger {
public:
  FooTrigger();
  ~FooTrigger();
  
  void activate();
  void deactivate();
  
  void update(ofEventArgs& args);
  
private:
  ofxTimer m_timer;
};


#endif
