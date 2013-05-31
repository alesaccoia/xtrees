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


#include "XTreeSeed.h"
#include "XTree.h"
#include "MathUtilities.h"

#define SEED_RADIUS 40
#define HALF_RADIUS_HOVER 20
#define HALF_RADIUS_BACK 10

#define HANDLE_RADIUS 20
#define HANDLE_DISTANCE 35

#define RADIUS_BLINK 40

#define BLINK_SPEED_MIN 0.2
#define BLINK_SPEED_MAX 0.35

XTreeSeed::XTreeSeed(XTree* parent_) :
  m_parent(parent_),
  m_displayHandle(true),
  m_displayCircle(true),
  m_mouseHover(false),
  m_blinking(true),
  m_starting(false),
  blink_radius(0.F) {
  resetPosition();
  m_blinkSpeed = BLINK_SPEED_MIN + ofRandomuf() * (BLINK_SPEED_MAX - BLINK_SPEED_MIN);
}

XTreeSeed::~XTreeSeed() {

}


void XTreeSeed::resetPosition() {
  ofVec2f par(m_parent->m_x, m_parent->m_y);
  m_handlePosition = ofPointFromPivot(par, m_parent->m_direction + M_PI, (float)(SEED_RADIUS/2 + HANDLE_DISTANCE + HANDLE_RADIUS/2));
}

void XTreeSeed::startBlinking() {
  m_timer.setAlarm(ofRandom(4800, 5200));
  m_starting = true;
  m_blinking = true;  
  showHandle();
}

void XTreeSeed::stopBlinking() {
  m_blinking = false;
  blink_radius = .0F;
  hideHandle();
}

void XTreeSeed::hideHandle() {  
  m_displayHandle = false;
  m_displayCircle = false;
}

void XTreeSeed::showHandle() {
  m_displayHandle = true;
  m_displayCircle = true;
}

void XTreeSeed::draw() {
  ofPushMatrix();
  ofTranslate(m_parent->m_x, m_parent->m_y, 0);
  ofSetLineWidth(2);
  if (m_displayCircle) {
    if (m_parent->m_isSelected == true) {
      ofFill();
      ofSetColor(m_starting ? m_timer.getDiffN() * 120.F : 120);
      ofEllipse(0, 0, SEED_RADIUS, SEED_RADIUS);
    }
    if (m_mouseHover) {
      ofFill();
      ofSetColor(m_starting ? m_timer.getDiffN() * 255.F : 255);
      ofEllipse(0, 0, SEED_RADIUS, SEED_RADIUS);
    }
    ofNoFill();
    ofSetColor(m_starting ? m_timer.getDiffN() * 255.F : 255);
    ofEllipse(0, 0, SEED_RADIUS, SEED_RADIUS);
  }
  if (m_blinking) {
    ofSetColor(255,(255.F - 255.F * (float)blink_radius / (float)RADIUS_BLINK) * (m_starting ? m_timer.getDiffN() : 1.0F));
    ofFill();
    ofEllipse(0, 0, blink_radius, blink_radius);
  }
  ofSetColor(g_jointC, m_starting ? m_timer.getDiffN() * 255.F : 255);
  if (!g_jointsAreCircles) {
    ofSetLineWidth(g_jointThickness);
    ofLine(-g_jointLength, -g_jointLength, g_jointLength, g_jointLength);
    ofLine(-g_jointLength, g_jointLength, g_jointLength, -g_jointLength);    
  } else {
    ofFill();
    ofCircle(0,0, g_jointLength);
  }
  //ofLine(-HALF_RADIUS_BACK, -HALF_RADIUS_BACK, HALF_RADIUS_BACK, HALF_RADIUS_BACK);
  //ofLine(HALF_RADIUS_BACK, -HALF_RADIUS_BACK, -HALF_RADIUS_BACK, HALF_RADIUS_BACK);

  ofPopMatrix();
  
  if (m_displayHandle) {
    if (m_handleHover) {
      ofSetColor(255,0,0);
    } else {
      ofSetColor(190, 0, 0);
    }
    ofLine(m_handlePosition.x, m_handlePosition.y, m_parent->m_x, m_parent->m_y);  
    ofEllipse(m_handlePosition.x, m_handlePosition.y, HANDLE_RADIUS, HANDLE_RADIUS);
  }
  
  ofSetColor(255);
  if (m_starting) {
    if (m_timer.alarm()) {
      m_starting = false;
    }
  }
}

void XTreeSeed::update() {
  if (m_blinking) {
    if (blink_radius < RADIUS_BLINK) {
      blink_radius += m_blinkSpeed;
    } else {
      blink_radius = .0F;
    }
  }
}

void XTreeSeed::mouseMoved(int x, int y) {
  if (ofDist(m_parent->m_x, m_parent->m_y, x, y) < SEED_RADIUS/2) {
    m_mouseHover = true;
  } else {
    if (ofDist(m_handlePosition.x, m_handlePosition.y, x, y) < HANDLE_RADIUS/2) {
      m_handleHover = true;
    } else {
      m_handleHover = false;
    }
    m_mouseHover = false;
  }

}

void XTreeSeed::mouseDragged(int x, int y) {
  if (m_mouseHover) {
    int deltaX = x - m_startX;
    int deltaY = y - m_startY;
    m_parent->m_x += deltaX;
    m_parent->m_y += deltaY;
    m_parent->m_originalX = m_parent->m_x;
    m_parent->m_originalY = m_parent->m_y;
    m_handlePosition.x += deltaX;
    m_handlePosition.y += deltaY;
    m_startX = x;
    m_startY = y;
  } else {
    if (m_handleHover) {
      ofPoint par(m_parent->m_x, m_parent->m_y);
      ofPoint now(x,y);
      m_parent->m_direction = ofAngle(now,par);
      m_parent->m_originalDirection = m_parent->m_direction;
      m_handlePosition = ofPointFromPivot(par, m_parent->m_direction + M_PI, (float)(SEED_RADIUS/2 + HANDLE_DISTANCE + HANDLE_RADIUS/2));
    }
  }

}

bool XTreeSeed::mousePressed(int x, int y) {
  if (ofDist(m_parent->m_x, m_parent->m_y, x, y) < SEED_RADIUS/2) {
    m_parent->select();
    m_startX = x;
    m_startY = y;
    return true;
  } else {
    if (ofDist(m_handlePosition.x, m_handlePosition.y, x, y) < HANDLE_RADIUS/2) {
      return true;
    } else {
      return false;
    }
  }
}

