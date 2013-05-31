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


#include "InteractiveAudio.h"
#include "ofMain.h"


#if USE_PDLIB

extern ofSoundStream soundStreamInput;
extern ofSoundStream soundStreamOutput;

extern "C" void moog_tilde_setup(void);
extern "C" void freeverb_tilde_setup(void);
extern "C" void spigot_tilde_setup(void);

extern "C" void bandpass_setup(void);
extern "C" void highpass_setup(void);
extern "C" void lowpass_setup(void);

extern "C" void para_bp2_tilde_setup(void);

extern "C" void expr_setup(void);

//extern "C" void expr_setup(void);


InteractiveAudio::InteractiveAudio() :
  m_active(false){
  m_destinations[kStart] = "start";
  m_destinations[kSimulOn] = "simulOn";
  m_destinations[kSimulOff] = "simulOff";
  m_destinations[kNewBranch] = "newBranch";
  m_destinations[kNewLeaf] = "newLeaf";
  m_destinations[kMsgOn] = "msgOn";  
  m_destinations[kLeavesOn] = "leavesOn"; 
  m_destinations[kLeavesOff] = "leavesOff"; 
  m_destinations[kLinesOn] = "linesOn"; 
  m_destinations[kLinesOff] = "linesOff";
  m_destinations[kNewLine] = "newLine";
  m_destinations[kBreath] = "breath";
  m_destinations[kReadyToRegenerate] = "readyToRegen";
  m_destinations[kRegen] = "regen";  
  m_destinations[kBranchPolyphony] = "polyphony";
  m_destinations[kActivateBirds] = "activateBirds";
  m_destinations[kActivateVinyl] = "activateVinyl";
  m_destinations[kActivateDrone] = "activateDrone";
  m_destinations[kActivateRand1] = "activateRandom1";
  m_destinations[kPlaceSeed] = "placeSeed";
  
  m_destinations[kEq100] = "eq100";
  m_destinations[kEq250] = "eq250";
  m_destinations[kEq700] = "eq700";
  m_destinations[kEq1500] = "eq1500";
  m_destinations[kEq3000] = "eq3000";
  m_destinations[kEq7000] = "eq7000";
  m_destinations[kEq15000] = "eq15000";
  m_destinations[kEqGain] = "eqGain";
  
}

InteractiveAudio::~InteractiveAudio() {
  m_pd->closePatch(m_patch);
  delete m_pd;
}

void InteractiveAudio::init(ofBaseApp* app_) {
  int ticksPerBuffer = 8;
  ofSoundStreamSetup(2, 0, app_, 44100, ofxPd::blockSize()*ticksPerBuffer, 3);
  m_pd = new ofxPd();
  if(!m_pd->init(2, 0, 44100, ticksPerBuffer)) {
		ofLog(OF_LOG_ERROR, "Could not init pd");
		OF_EXIT_APP(1);
	}
  
  /* initialize externals here */
  
  moog_tilde_setup();
  freeverb_tilde_setup();
  spigot_tilde_setup();
  
  bandpass_setup();
  highpass_setup();
  lowpass_setup();
  para_bp2_tilde_setup();
  
  expr_setup();
  
#if 0
  ofSetLogLevel(OF_LOG_VERBOSE);
  ofSoundStreamListDevices();
#endif
  
  
  m_pd->addToSearchPath("pd");
  m_patch = m_pd->openPatch("pd/MainApplication.pd"); // OPEN
#if 0
  cout << m_patch << endl;
#endif
	m_pd->start(); // DSP ON
}

void InteractiveAudio::start() {
  m_active = true;
}

void InteractiveAudio::stop() {
  m_active = false;
}

// will be never called
void InteractiveAudio::audioReceived(float * input, int bufferSize, int nChannels) {
  m_pd->audioIn(input, bufferSize, nChannels);
}

void InteractiveAudio::audioRequested(float * output, int bufferSize, int nChannels) {
  if (m_active) {
    //std:cout << ",";
    m_pd->audioOut(output, bufferSize, nChannels);
  }
}

void InteractiveAudio::sendBang(AudioDestination destination_) {
  m_pd->sendBang(m_destinations[destination_]);
}

void InteractiveAudio::sendFloat(AudioDestination destination_, float arg_) {
  m_pd->sendFloat(m_destinations[destination_], arg_);
}

void InteractiveAudio::sendFloatList(AudioDestination destination_, std::vector<float>& args_ ) {
  m_pd->startMessage();
    for (std::vector<float>::iterator it = args_.begin(); it != args_.end() ; ++it)
      m_pd->addFloat(*it);
	m_pd->finishList(m_destinations[destination_]);
}



#elif USE_OSC

#define HOST "localhost"
#define PORT 12345

InteractiveAudio::InteractiveAudio() :
  m_active(false){
  m_destinations[kStart] = "/events/start";
  m_destinations[kSimulOn] = "/events/simulOn";
  m_destinations[kSimulOff] = "/events/simulOff";
  m_destinations[kNewBranch] = "/events/newBranch";
  m_destinations[kNewLeaf] = "/events/newLeaf";
  m_destinations[kMsgOn] = "/events/msgOn";  
  m_destinations[kLeavesOn] = "/events/leavesOn"; 
  m_destinations[kLeavesOff] = "/events/leavesOff"; 
  m_destinations[kLinesOn] = "/events/linesOn"; 
  m_destinations[kLinesOff] = "/events/linesOff";
  m_destinations[kNewLine] = "/events/newLine";
  m_destinations[kBreath] = "/events/breath";
  m_destinations[kReadyToRegenerate] = "/events/readyToRegen";
  m_destinations[kRegen] = "/events/regen"; 
  m_destinations[kBranchPolyphony] = "/events/polyphony";
  m_destinations[kActivateBirds] = "/events/activateBirds";
  m_destinations[kActivateVinyl] = "/events/activateVinyl";
  m_destinations[kActivateDrone] = "/events/activateDrone";
  m_destinations[kActivateRand1] = "/events/activateRandom1";
  m_destinations[kPlaceSeed] = "/events/placeSeed";
    
  m_destinations[kEq100] = "/settings/eq100";
  m_destinations[kEq250] = "/settings/eq250";
  m_destinations[kEq700] = "/settings/eq700";
  m_destinations[kEq1500] = "/settings/eq1500";
  m_destinations[kEq3000] = "/settings/eq3000";
  m_destinations[kEq7000] = "/settings/eq7000";
  m_destinations[kEq15000] = "/settings/eq15000";
  m_destinations[kEqGain] = "/settings/eqGain";
}

InteractiveAudio::~InteractiveAudio() {

}

void InteractiveAudio::init(ofBaseApp* app_) {
  m_sender.setup( HOST, PORT );
}

void InteractiveAudio::start() {
  m_active = true;
}

void InteractiveAudio::stop() {
  m_active = false;
}

void InteractiveAudio::sendBang(AudioDestination destination_) {
  ofxOscMessage m;
  m.setAddress(m_destinations[destination_]);
  m_sender.sendMessage( m );
}

void InteractiveAudio::sendFloat(AudioDestination destination_, float arg_) {
  ofxOscMessage m;
  m.setAddress(m_destinations[destination_]);
  m.addFloatArg(arg_);
  m_sender.sendMessage( m );
}

void InteractiveAudio::sendFloatList(AudioDestination destination_, std::vector<float>& args_ ) {
  ofxOscMessage m;
  m.setAddress(m_destinations[destination_]);
    for (std::vector<float>::iterator it = args_.begin(); it != args_.end() ; ++it)
      m.addFloatArg(*it);
	m_sender.sendMessage( m );
}

#elif USE_NOAUDIO

InteractiveAudio::InteractiveAudio() :
  m_active(false){
}

InteractiveAudio::~InteractiveAudio() {

}

void InteractiveAudio::init(ofBaseApp* app_) {

}

void InteractiveAudio::start() {

}

void InteractiveAudio::stop() {

}

void InteractiveAudio::sendBang(AudioDestination destination_) {

}

void InteractiveAudio::sendFloat(AudioDestination destination_, float arg_) {

}

void InteractiveAudio::sendFloatList(AudioDestination destination_, std::vector<float>& args_ ) {

}

#endif



