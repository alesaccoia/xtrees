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

 
#ifndef TWITTERBALOON_H_
#define TWITTERBALOON_H_

#include "MessageTrigger.h"
#include "ofMain.h"
#include <string>

#ifndef XTREES_IOS
#include "ofxFTGLFont.h"
#include <FTGL/ftgl.h>
#endif

#include "KepleroUtils.h"

#define DRAW_BOXES 0

class XTree;

class TwitterBaloon{	
public:
	TwitterBaloon(XTree* tree_,
    std::string& key_,
    std::string& user_,
    std::string& time_,
    std::tr1::shared_ptr<ofPixels> image_,
    ofVec2f* position_,
    float branchRotation_,
    MessageSource src_);
  
  ~TwitterBaloon();
  
	void draw();
  
  void update();

  inline int life() {
    return m_currentLife;
  }
  
  static void initFbo() {
    s_fbo.allocate(500, 200);
    s_images.resize(60);
    s_busy.resize(60);
    for (int i = 0; i < 60; ++i) {
      s_images[i].allocate(500, 200, OF_IMAGE_COLOR_ALPHA);
      s_busy[i] = false;
    } 
  }
  
  static int returnFreeSlot() {
    for (int i = 0; i < 60; ++i) {
      if (!s_busy[i]) {
        return i;
      }
    }
  }
  
  bool isDead();

private:
  enum AttachSide {
    kLeftSide,
    kBottomSide, 
    kRightSide
  };
  
  XTree* m_tree;
  AttachSide m_attachSide;

  std::wstring m_key;
  std::wstring m_user;
  std::wstring m_time;
  ofImage* m_image;
  
  ofImage m_baloon;
  
  // member colors
  int m_life, m_currentLife, m_width, m_height, m_userRowHeight, m_x, m_y ;
  
  ofVec2f m_initialVelocity, *m_positionRef, m_lastAttach;
  
  float m_dx, m_dy, m_vy, m_vx, m_rotation;
  float initialY;
  int m_pictureMargin;
  int m_topRowMargin;
  FTSimpleLayout* m_layout;  
  FTFont* m_font;
  ofColor m_msgColor;
  ofColor m_userColor;
  
  static std::vector<ofImage> s_images;
  static std::vector<bool> s_busy;
  static ofFbo s_fbo;
  size_t m_slot;
};

#endif

