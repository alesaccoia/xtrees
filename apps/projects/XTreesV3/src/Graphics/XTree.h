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



#ifndef XTREE_H_
#define XTREE_H_

#include <string>
#include <tr1/memory>

#ifdef XTREES_APP
#include "textInput.h"
#endif
#include "XTreeBranch.h"

#include "TwitterLayer.h"
#include "TwilioTrigger.h"
#include "TwitterTrigger.h"
#include "DatabaseTrigger.h"
#include "ofxTimer.h"
#include "GraphicUtilities.hpp"
#include "Leaf.h"

class XTreeSeed;
class FlowersLayer;
class TreesManager;

class XTree {
  friend class TreesManager;
  friend class XTreeSeed;
  friend class FlowersLayer;
  friend class LinesLayer;
public:
  XTree(int x, int y, std::string key_ , std::tr1::shared_ptr<TwitterLayer> twitterLayer_, float direction_ = -M_PI/2.F);
  
  ~XTree();
  
  void start();
  void reset();
  void stopGrowing();
  void restartGrowing();
  
  void draw();
  void update();
  void select();
  void deselect();
  
  void mouseMoved(int mouseX, int mouseY);
  bool mousePressed(int mouseX, int mouseY);
  void mouseDragged(int mouseX, int mouseY);
  
  void keyIn(int key_);  
  bool isSelected(int mouseX, int mouseY);
  void loseFocus();
  void regenerate();
  void fadeImagesQuickly();
  
  // could use this for reloading from saved seeds
  void setKeyword(std::string& m_keyword);
  
  int getCurrentLevel() {
    return m_currentLevel;
  }
  
  int currentBaloonsAlive;  
  
  static void initFbo() {
    s_fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    s_fbo.begin();   // draw the line into the texture
    ofEnableAlphaBlending();
    ofClear(0,0);
    s_fbo.end();
  }
  
  void setDirection(float val_) {
    m_direction = val_;
  }
  
  Leaf* addLeaf() {
    if (m_trunk != NULL )
      return m_trunk->addLeaf();
    else
      return NULL;
  }
  
  void realtimeStarving(bool isStarving) {
    m_treeIsStarvingOfRealtimeMessages = isStarving;
  }
  
  bool isRealtmeStarving() {
    return m_treeIsStarvingOfRealtimeMessages;
  }
  
private:
  void evolve(std::tr1::shared_ptr<MessageEvent>& args);
  
  inline float getColonizationLevel() {
    // consider also stuck the branches... FIXME kludge
    return (((float)m_numberOfBranches + ((float)m_numberOfBranchesStuck / 2.F)) / (float)g_maxBranches) * 100.F;
  }
  
  typedef struct {
    ofImage image;
    float lifespan;
    float timeToLive;
    float currentAlpha;
    float currentFadingSpeed;
    // needs to be called from the opengl thread
    inline void createBegin() {
      image.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR_ALPHA);
      s_fbo.begin();   // draw the line into the texture
      ofClear(255,255,255,0);
      ofEnableAlphaBlending();
      ofSetColor(255);      
    }
    
    // draw the tree on this fbo...    
    inline void createEnd() {
      s_fbo.end();
      s_fbo.readToPixels(image.getPixelsRef());  
      image.update();
      //kplSetMaxAlpha(&image);
    }
    
    inline void draw() {
      timeToLive -= currentFadingSpeed;
      currentAlpha = (timeToLive / lifespan) * 255.F;
      ofEnableAlphaBlending();
      ofSetColor(255, currentAlpha);      
      image.draw(0,0,ofGetWidth(), ofGetHeight());
    }
  }* fadingTreePtr, fadingTree;

  int m_originalX;
  int m_originalY;
  int m_x;
  int m_y;
  std::string m_keyword;
  float m_direction;
  float m_originalDirection;
  
  XTreeSeed* m_seed;
  XTreeBranch* m_trunk;
  MessageTrigger* m_twitterTrigger;
  MessageTrigger* m_twilioTrigger;
  MessageTrigger* m_databaseTrigger;
  std::tr1::shared_ptr<TwitterLayer> m_twitterLayer;
  bool m_isSelected;
  bool m_regenerating;
  int m_numberOfBranches;
  int m_numberOfBranchesStuck;
  int m_maxNumberOfBranches;
#ifdef XTREES_APP
  textInput m_textbox;
#endif
  
  // this is used for various tasks
  ofxTimer m_timer;
  bool m_isStarting;
  bool m_isValidating;
  int m_validationRadius;
  int m_currentLevel;
  
  bool m_scheduledForRegrowth;
  bool m_isFadingImagesQuickly;
  
  std::list<fadingTreePtr> m_fadingImages;
  
  bool m_drawMe;
  static ofFbo s_fbo;
  
  ofxTimer m_flowerTimer;
  
  float m_keywordActivity;
  
  bool m_treeIsStarvingOfRealtimeMessages;
  bool m_isStopped;

};


#endif // XTREE_H_
