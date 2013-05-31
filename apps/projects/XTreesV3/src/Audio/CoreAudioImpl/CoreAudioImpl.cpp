/*
 *  CoreAudioImpl.cpp
 *  XTreesV3
 *
 *  Created by Alessandro Saccoia on 6/21/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "CoreAudioImpl.h"
#include "Debugging++.h"

InteractiveAudio::InteractiveAudio() :
  m_active(false){
  
  OSStatus result = noErr;

  m_audioPlayers[kNewBranch].path = CFSTR("/Volumes/Data/wwwroot/trunk/samples/audio/sounds/hyper-reality/refreshing drink.aif");
  
  //LoadAudioFiles();
  
  // new AU graph
  RequireNoErr(NewAUGraph(&mGraph));  
  
	AudioUnit output;
  OSType              componentType;          /* A unique 4-byte code indentifying the command set */
  OSType              componentSubType;       /* Particular flavor of this instance */
  OSType              componentManufacturer;  /* Vendor indentification */
  UInt32              componentFlags;         /* 8 each for Component,Type,SubType,Manuf/revision */
  UInt32              componentFlagsMask; 
  
  AudioComponentDescription output_desc = {0};
  output_desc.componentType = kAudioUnitType_Output;
  output_desc.componentSubType = kAudioUnitSubType_DefaultOutput;
  output_desc.componentManufacturer = kAudioUnitManufacturer_Apple;  
  output_desc.componentFlags = 0;
  output_desc.componentFlagsMask = 0;
  
  AudioComponentDescription mixer_desc  = {0};
  mixer_desc.componentType = kAudioUnitType_Mixer;
  mixer_desc.componentSubType = kAudioUnitSubType_StereoMixer;
  mixer_desc.componentManufacturer = kAudioUnitManufacturer_Apple;
  mixer_desc.componentFlags = 0;
  mixer_desc.componentFlagsMask = 0;

  // Add the nodes to the graph,
  // The AudioComponentDescription object used to find and open the audio unit 
  // that you are adding as a new node.
	RequireNoErr(AUGraphAddNode(mGraph, &output_desc, &m_outputNode));
	RequireNoErr(AUGraphAddNode(mGraph, &mixer_desc, &m_mixerNode));
  
  
	RequireNoErr(AUGraphConnectNodeInput(	mGraph,
											m_mixerNode, // AUNode inSourceNode,
                      0, //UInt32 inSourceOutputNumber,
											m_outputNode, // inDestNode
                      0 )); // inDestInputNumber


  //Upon return from this function call, the audio units belonging to the graph 
  // are open but not initialized. Specifically, no resource allocation occurs.
	RequireNoErr(AUGraphOpen(mGraph));

  // Calling this function calls the AudioUnitInitialize function on each opened node 
  // or audio unit that is involved in a interaction. If a node is not involved, it 
  // is initialized after it becomes involved in an interaction.
  RequireNoErr(AUGraphInitialize(mGraph));

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

void InteractiveAudio::LoadAudioFiles() {
  
  std::map<AudioDestination, MyAUGraphPlayer>::iterator m_it = m_audioPlayers.begin();
  
  while (m_it != m_audioPlayers.end()) {  
    CFURLRef inputFileURL = CFURLCreateWithFileSystemPath(
                                kCFAllocatorDefault,
                                m_it->second.path,
                                kCFURLPOSIXPathStyle,
                                false);
    RequireNoErrString(AudioFileOpenURL(inputFileURL,
                                kAudioFileReadPermission,
                                0,
                                &m_it->second.inputFile),
                                "AudioFileOpenURL failed");
    CFRelease(inputFileURL);
    // Get the audio data format from the file
    UInt32 propSize = sizeof(m_it->second.inputFormat);
    RequireNoErrString(AudioFileGetProperty(m_it->second.inputFile,
                                    kAudioFilePropertyDataFormat,
                                    &propSize,
                                    &m_it->second.inputFormat),
               "Couldn't get file's data format");
  }

}

