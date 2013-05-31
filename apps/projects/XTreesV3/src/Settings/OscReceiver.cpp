/*
 *  OscReceiver.cpp
 *  XTreesV3
 *
 *  Created by Alessandro Saccoia on 2/26/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "OscReceiver.h"
#include "Settings.h"
#include "testApp.h"

#define LISTENING_PORT 57120

// implement the fact that the controller writes a file in the data directory for when it is attached, otherwise 
// somebody could sniff :D

OscReceiver::OscReceiver() {
  cout << "listening for osc messages on port " << LISTENING_PORT << "\n";
	receiver.setup( LISTENING_PORT );

}

void OscReceiver::threadedFunction() {
  while(isThreadRunning() != 0){
    while( receiver.hasWaitingMessages() )
    {
      ofxOscMessage m;
      receiver.getNextMessage( &m );
      doAction(m);
    }
    ofSleepMillis(30);
  }
}
void OscReceiver::doAction(ofxOscMessage& m) {
  std::string address = m.getAddress();
  if (address == "/parameters/backgroundR") {
    g_backgroundC.r = m.getArgAsInt32(0);
    g_updateBackground();
  } else if (address == "/parameters/backgroundG") {
    g_backgroundC.g = m.getArgAsInt32(0);
    g_updateBackground();
  } else if (address == "/parameters/backgroundB") {
    g_backgroundC.b = m.getArgAsInt32(0);
    g_updateBackground();
  } else if (address == "/parameters/treeR") {
    g_treeC.r = m.getArgAsInt32(0);
    //g_changeBranchImages = true;
  } else if (address == "/parameters/treeG") {
    g_treeC.g = m.getArgAsInt32(0);
    //g_changeBranchImages = true;
  } else if (address == "/parameters/treeB") {
    g_treeC.b = m.getArgAsInt32(0);
    //g_changeBranchImages = true;
  } else if (address == "/parameters/treeA") {
    g_treeC.a = m.getArgAsInt32(0);
    //g_changeBranchImages = true;
  } else if (address == "/parameters/jointsAreCircles") {
    g_jointsAreCircles = m.getArgAsInt32(0);
  }  else if (address == "/parameters/jointR") {
    g_jointC.r = m.getArgAsInt32(0);
  }  else if (address == "/parameters/jointG") {
    g_jointC.g = m.getArgAsInt32(0);
  }  else if (address == "/parameters/jointB") {
    g_jointC.b = m.getArgAsInt32(0);
  } else if (address == "/parameters/branchWidth") {
    g_branchWidth = (float)(m.getArgAsInt32(0));
  } else if (address == "/parameters/branchLength") {
    g_branchLength = (float)(m.getArgAsInt32(0));
  } else if (address == "/parameters/widthDecrease") {
    g_widthDecrease = (float)(m.getArgAsInt32(0)) / 100.F;
  } else if (address == "/parameters/lengthDecrease") {
    g_lengthDecrease = (float)(m.getArgAsInt32(0)) / 100.F;
  } else if (address == "/parameters/pastTweets") {
    g_pastTweets = m.getArgAsInt32(0);
  } else if (address == "/parameters/jointThickness") {
    g_jointThickness = m.getArgAsInt32(0);
  } else if (address == "/parameters/jointLength") {
    g_jointLength = m.getArgAsInt32(0);
  }  else if (address == "/parameters/tweetMsgR") {
    g_tweetMsgC.r = m.getArgAsInt32(0);
  } else if (address == "/parameters/tweetMsgG") {
    g_tweetMsgC.g = m.getArgAsInt32(0);
  } else if (address == "/parameters/tweetMsgB") {
    g_tweetMsgC.b = m.getArgAsInt32(0);
  } else if (address == "/parameters/tweetMsgSize") {
    g_setTwitterMsgFontSize(m.getArgAsInt32(0));
  }  else if (address == "/parameters/tweetUsR") {
    g_tweetUsrC.r = m.getArgAsInt32(0);
  } else if (address == "/parameters/tweetUsG") {
    g_tweetUsrC.g = m.getArgAsInt32(0);
  } else if (address == "/parameters/tweetUsB") {
    g_tweetUsrC.b = m.getArgAsInt32(0);
  }  else if (address == "/parameters/tLineR") {
    g_tLineC.r = m.getArgAsInt32(0);
  } else if (address == "/parameters/tLineG") {
    g_tLineC.g = m.getArgAsInt32(0);
  } else if (address == "/parameters/tLineB") {
    g_tLineC.b = m.getArgAsInt32(0);
  } else if (address == "/parameters/tLineWidth") {
    g_tLineWidth = m.getArgAsInt32(0);
  } else if (address == "/parameters/growthMin") {
    g_growthMin = m.getArgAsInt32(0);
  } else if (address == "/parameters/growthMax") {
    g_growthMax = m.getArgAsInt32(0);
  } else if (address == "/parameters/timedExhibit") {
    g_timedExhibit = m.getArgAsInt32(0);
  } else if (address == "/parameters/showDuration") {
    g_showDuration = m.getArgAsInt32(0);
  } else if (address == "/parameters/minFrequency") {
    g_minFrequency = m.getArgAsInt32(0);
  } else if (address == "/parameters/maxFrequency") {
    g_maxFrequency = m.getArgAsInt32(0);
  } else if (address == "/parameters/minAngle") {
    g_minAngle = m.getArgAsInt32(0);
  } else if (address == "/parameters/maxAngle") {
    g_minAngle = m.getArgAsInt32(0);
  } else if (address == "/parameters/showThumbs") {
    g_showThumbs = m.getArgAsInt32(0);
  } else if (address == "/parameters/fertility") {
    g_fertility = m.getArgAsInt32(0);
    g_computeMaxBranches();
  }  else if (address == "/parameters/maxLevel") {
    g_maxLevel = m.getArgAsInt32(0);
    g_computeMaxBranches();
  } else if (address == "/parameters/floatingSpeed") {
    g_floatingSpeed = (float)(m.getArgAsInt32(0)) / 300.F;
  } else if (address == "/parameters/splashR") {
    g_splashC.r = m.getArgAsInt32(0);
  } else if (address == "/parameters/splashG") {
    g_splashC.g = m.getArgAsInt32(0);
  } else if (address == "/parameters/splashB") {
    g_splashC.b = m.getArgAsInt32(0);
  } else if (address == "/parameters/soundVolume") {
    g_soundVolume = (float)(m.getArgAsInt32(0));
  } else if (address == "/parameters/flowersMin") {
    g_flowersMin = m.getArgAsInt32(0);
  } else if (address == "/parameters/flowersMax") {
    g_flowersMax = m.getArgAsInt32(0);
  } else if (address == "/parameters/crazyLineWidth") {
    g_crazyLineWidth = m.getArgAsInt32(0);
  } else if (address == "/parameters/crazyCircleWidth") {
    g_crazyCircleWidth = m.getArgAsInt32(0);
  } else if (address == "/parameters/crazyLineWidth") {
    g_crazyLineWidth = m.getArgAsInt32(0);
  } else if (address == "/parameters/useBackgroundImage") {
    g_app->use_background = m.getArgAsInt32(0);
  } else if (address == "/parameters/useInteractiveAudio") {
    g_useInteractiveAudio = m.getArgAsInt32(0);
  } else if (address == "/parameters/Background") {
    g_backgroundImage = m.getArgAsString(0);
  } else if (address == "/parameters/Soundtrack") {
    g_soundtrack = m.getArgAsString(0);
    g_activateSoundtrack();
  } else if (address == "/parameters/minColonization") {
    g_minColonization = (float)(m.getArgAsInt32(0));
  } else if (address == "/parameters/minColonizationForLeaves") {
    g_minColonizationForLeaves = (float)(m.getArgAsInt32(0));
  } else if (address == "/parameters/treesOpacity") {
    g_treesLayerOpacity = (float)(m.getArgAsInt32(0));
  } else if (address == "/parameters/twitterOpacity") {
    g_twitterLayerOpacity = (float)(m.getArgAsInt32(0));
  } else if (address == "/parameters/splashOpacity") {
    g_splashOpacity = (float)(m.getArgAsInt32(0));
  } else if (address == "/parameters/flowersOpacity") {
    g_flowersOpacity = (float)(m.getArgAsInt32(0));
  } else if (address == "/parameters/showFlowers") {
    g_showFlowers = (float)(m.getArgAsInt32(0)) ;
    g_activateFlowers();
  }
  
}


  
