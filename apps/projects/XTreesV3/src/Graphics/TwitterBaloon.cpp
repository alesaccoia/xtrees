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

#include "TwitterBaloon.h"
#include "XTree.h"
#include "GraphicUtilities.hpp"
#include "Settings.h"

#ifdef XTREES_IOS
#include "FTGL/ftgles.h"
#endif

#define PI3OVER8 1.178097245096172
#define PI5OVER8 1.963495408493621

#define LINE_MARGIN 3

ofFbo TwitterBaloon::s_fbo;

std::vector<ofImage> TwitterBaloon::s_images;
std::vector<bool> TwitterBaloon::s_busy;

  
TwitterBaloon::TwitterBaloon(XTree* tree_, 
    std::string& key_,
    std::string& user_,
    std::string& time_,
    std::tr1::shared_ptr<ofPixels> image_,
    ofVec2f* position_,
    float branchRotation_,
    MessageSource src_) :
  m_tree(tree_),
  m_positionRef(position_),
  m_lastAttach(*m_positionRef),
  m_currentLife(0),
  m_x(0),
  m_y(0),
  m_vx(0),
  m_vy(0),
  m_image(NULL) {
  
  switch (src_) {
    case SRC_TWITTER:
      m_layout = g_twitterLayout;
      m_font = g_twitterFont;
      m_msgColor = g_tweetMsgC;
      m_userColor = g_tweetUsrC;
      break;
    case SRC_TWILIO:
      m_layout = g_twilioLayout;
      m_font = g_twilioFont;
      m_msgColor = g_twilioMsgC;
      m_userColor = g_twilioUsrC;
      break;
    case SRC_DATABASE:
      m_layout = g_databaseLayout;
      m_font = g_databaseFont;
      m_msgColor = g_databaseMsgC;
      m_userColor = g_databaseUsrC;
      m_userColor.a = 0;
      break;
    default:
      break;
  }
  m_pictureMargin = m_font->FaceSize() / 2;
  m_topRowMargin = BALOON_MWG_LINESPACING * m_font->FaceSize() / 2;
  
  m_tree->currentBaloonsAlive++;
  
  // until now it's the real branch rotation
  branchRotation_ = kplNormalizeAngle(branchRotation_);
  
  // now m_branchRotation it's the angle between the horizontal line passing thrugh the apex  real branch rotation  
  if (branchRotation_ >=0.F && branchRotation_ <= PI/2.F) {
    // towards BOTTOM-RIGHT, keep it in the left part
    m_attachSide = kLeftSide;
    m_rotation = 13.F / 8.F * M_PI + branchRotation_ / 2.F;
  } else if (branchRotation_ > PI/2.F && branchRotation_ <= M_PI) {
    // towards BOTTOM-LEFT, keep it in the right part
    m_attachSide = kRightSide;
    m_rotation = 13.F / 8.F * M_PI  - ((branchRotation_ - M_PI/2.F) / 2.);
  } else if (branchRotation_ > M_PI && branchRotation_ <= 3. * M_PI / 2.) {
    // towards BOTTOM-LEFT, keep it in the right part
    m_attachSide = kRightSide;
    m_rotation = 9.F / 8.F * M_PI  + ((3. / 2. * M_PI - branchRotation_) / 2.);
  } else if (branchRotation_ > 3. * M_PI / 2.) {
    // towards BOTTOM-LEFT, keep it in the right part
    m_attachSide = kLeftSide;
    m_rotation = 15.F / 8.F * M_PI  - ((branchRotation_ - 3. / 2. * M_PI) / 2.);
  }
#ifdef XTREES_IOS
  m_dx = 20.F * cos(m_rotation);
  m_dy = 150.F * sin(m_rotation);
#else
  m_dx = 80.F * cos(m_rotation);
  m_dy = 80.F * sin(m_rotation);
#endif
  if (image_.get() != NULL) {
    if (image_->getWidth() > 48 || image_->getHeight() > 48) {
      image_->resize(48, 48);
    }
    m_image = new ofImage(*image_.get());
  }



  // convert unicode to wstring
  utf8toWStr(m_key, key_);
  utf8toWStr(m_user, user_);
  utf8toWStr(m_time, time_);
  
  m_life = ofRandom(g_tweetMinLife, g_tweetMaxLife);
#ifdef XTREES_IOS
  // get the width and height of the bounding box
  m_userRowHeight=17;m_height=48;m_userRowHeight=17;m_width=357;
  if (m_attachSide == kRightSide) {
    m_dx -= m_width;
  }
  update();
  return;
#endif
  FTBBox boxMsg = m_layout->BBox(m_key.c_str());
  FTBBox boxUser = m_layout->BBox(m_user.c_str());
  m_userRowHeight = (int)abs(boxUser.Upper().Y() - boxUser.Lower().Y()) + m_topRowMargin;
  //FTBBox boxTime = s_timeLayout->BBox(m_time.c_str());
  
  /*
     [    ] user       |
     [    ] msg        |
                  time |
  */
  // NOTE time not used
  
  m_width = abs(boxMsg.Upper().X() - boxMsg.Lower().X()) + 2; 
  if (m_image != NULL) {
    m_width += m_pictureMargin + m_image->getWidth();
  } 
  m_height = std::max((int)abs(boxMsg.Upper().Y() - boxMsg.Lower().Y()) + 2 + m_userRowHeight, g_showThumbs ? (int)m_image->getWidth(): 0);
    
  if (m_attachSide == kRightSide) {
    m_dx -= m_width;
  }
  
  update();

  initialY = m_y;
  
  m_slot = returnFreeSlot();
  s_busy[m_slot] = true;
  
  //create image
  //m_baloon.allocate(m_width, m_height, OF_IMAGE_COLOR_ALPHA);
  //s_fbo.allocate(m_width, m_height);
  s_fbo.begin();   // draw the line into the texture
  
  ofEnableBlendMode(OF_BLENDMODE_ADD);
  ofClear(g_backgroundC.r,g_backgroundC.g,g_backgroundC.b,g_msgOpacity);
  ofSetColor(255);
  // DRAW
  int currentX = 0;
  if (m_image != NULL) {    
    m_image->draw(currentX, 0);
    currentX += m_pictureMargin + m_image->getWidth();
  }
  glPushMatrix();
  glTranslatef(currentX,
    BALOON_MWG_LINESPACING * m_font->FaceSize() - 2,
    0);
  
  glScalef(1,-1,1);
  
  ofSetColor(m_userColor);
  m_layout->Render(m_user.c_str());  
  glTranslatef(0, -m_userRowHeight, 0);
  ofSetColor(m_msgColor);
  m_layout->Render(m_key.c_str());
  glPopMatrix();
  
  s_fbo.end();
  s_fbo.readToPixels(s_images[m_slot].getPixelsRef());  
  s_images[m_slot].update();

}

bool TwitterBaloon::isDead() {
  return (m_y + m_height < 0) || (m_currentLife >= m_life);
}

void TwitterBaloon::update() {
  if (*m_positionRef != m_lastAttach ) {
    m_lastAttach = *m_positionRef;
  }
  ++m_currentLife;
  
  m_x = m_lastAttach.x + m_dx;
  m_y = m_lastAttach.y + m_dy;
  
  
  if (m_currentLife > 100) {
    m_vy -= g_floatingSpeed;
  }
  
  m_dy += m_vy;
}

TwitterBaloon::~TwitterBaloon() {
  s_busy[m_slot] = false;
  --m_tree->currentBaloonsAlive;
  if (g_showThumbs) {
    delete m_image;
  }
}

void TwitterBaloon::draw() {

  int alphaLevel = g_twitterLayerOpacity - g_twitterLayerOpacity * (float)m_currentLife / (float)m_life;

  glPushMatrix();
  
  glTranslatef(m_x, m_y, 0);
  
  ofSetColor(255, alphaLevel);
  
  s_images[m_slot].draw(0,0);
  
  
  glPopMatrix();
  ofSetLineWidth(g_tLineWidth);
  ofSetColor(g_tLineC, std::min((float)alphaLevel, 255.f * (float)m_y/initialY));
  if (m_attachSide == kLeftSide) {
    ofLine(m_x - LINE_MARGIN, m_y + m_height/2, m_lastAttach.x, m_lastAttach.y);
  } else {
    ofLine(m_x + m_width + LINE_MARGIN, m_y + m_height/2, m_lastAttach.x, m_lastAttach.y);
  }
  
}

