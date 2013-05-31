/*
 *  CoreAudioImpl.h
 *  XTreesV3
 *
 *  Created by Alessandro Saccoia on 6/21/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef COREAUDIOIMPL_H_
#define COREAUDIOIMPL_H_

#import <AudioToolbox/AudioToolbox.h>

#define kPlaybackFileLocation    CFSTR("../../../Data/Soundtracks/xtreeSoundtrack.mp3")
#define kNumberPlaybackBuffers    3

#include "Singleton.hpp"
#include "AudioEvents.h"
#include "ofMain.h"
#include "Debugging++.h"

typedef struct MyAUGraphPlayer
{
    CFStringRef path;
    AudioStreamBasicDescription inputFormat;
    AudioFileID inputFile;
    AUGraph graph;
    AudioUnit destination;
    
    void play() {
      AUNode fileNode;
      AudioComponentDescription cd;
      cd.componentType = kAudioUnitType_Generator;
      cd.componentSubType = kAudioUnitSubType_AudioFilePlayer;

    }
    
} MyAUGraphPlayer;

class InteractiveAudio : 
public Singleton<InteractiveAudio> {
public:
  InteractiveAudio();
  ~InteractiveAudio();
  
  void init(ofBaseApp* app_);
  
  void start();  
  void stop();  
  void sendBang(AudioDestination destination_);  
  void sendFloat(AudioDestination destination_, float arg_);
  void sendFloatList(AudioDestination destination_, std::vector<float>& args_ );
  
private:

  void LoadAudioFiles();

	AUGraph		mGraph;
	AudioUnit	m_mixerAu;
	AUNode m_outputNode;
	AUNode m_mixerNode;
  
  bool m_active;
  std::map<AudioDestination, MyAUGraphPlayer> m_audioPlayers;
};

#endif
