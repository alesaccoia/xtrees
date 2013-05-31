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

#ifndef INTERACTIVEAUDIO_H_
#define INTERACTIVEAUDIO_H_

#define USE_PDLIB 1
#define USE_OSC 0
#define USE_NOAUDIO 0


#include "Singleton.hpp"
#include "AudioEvents.h"
#include "ofMain.h"

#if USE_PDLIB
#include "ofxPd.h"
#elif USE_OSC
#include <map>
#include <string>
#include "ofxOsc.h"
#endif

class InteractiveAudio : 
public Singleton<InteractiveAudio>
{
public:
  InteractiveAudio();
  ~InteractiveAudio();
  
  void init(ofBaseApp* app_);
  
  void start();  
  void stop();  
  void sendBang(AudioDestination destination_);  
  void sendFloat(AudioDestination destination_, float arg_);
  void sendFloatList(AudioDestination destination_, std::vector<float>& args_ );
  
#if USE_PDLIB
  void audioReceived(float * input, int bufferSize, int nChannels);
  void audioRequested(float * output, int bufferSize, int nChannels);
#endif
  
private:
#if USE_PDLIB
  ofxPd* m_pd;
  pd::Patch m_patch;
#endif
#if USE_OSC
  ofxOscSender m_sender;
#endif
  bool m_active;
  std::map<AudioDestination, std::string> m_destinations;
};




#endif
