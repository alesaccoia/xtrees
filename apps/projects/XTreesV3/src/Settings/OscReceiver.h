/*
 *  OscReceiver.h
 *  XTreesV3
 *
 *  Created by Alessandro Saccoia on 2/26/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __OSCRECEIVER_H__
#define __OSCRECEIVER_H__

#include "ofxOsc.h"
#include "ofThread.h"

class OscReceiver : public ofThread {
public:
	OscReceiver();

protected:
  virtual void threadedFunction();
  void doAction(ofxOscMessage& m);

private:
	ofxOscReceiver	receiver;
};

#endif // __OSCRECEIVER_H__
