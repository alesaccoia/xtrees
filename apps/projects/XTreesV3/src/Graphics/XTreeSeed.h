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


#ifndef XTREESEED_H_
#define XTREESEED_H_

#define SEED_RADIUS 20
#define SPEED 3 // revolutions per second

#include "ofxTimer.h"

class XTree;

class XTreeSeed {
public:
  XTreeSeed(XTree* parent_);
  ~XTreeSeed();
  
  void hideHandle();
  void showHandle();
  void mouseMoved(int x, int y);
  void mouseDragged(int x, int y);
  bool mousePressed(int x, int y);
  void draw();
  void update();
  void startBlinking();
  void stopBlinking();
  void move(int x, int y) {
    m_startX += x;
    m_startY += y;
    m_handlePosition.x += x;
    m_handlePosition.y += y;
  }
  
  void resetPosition();

private:
  XTree* m_parent;
  bool m_displayHandle;
  bool m_displayCircle;
  bool m_mouseHover;
  bool m_handleHover;
  bool m_blinking;
  bool m_starting;
  float m_blinkSpeed;
  ofxTimer m_timer;
  float blink_radius;
  ofVec2f m_handlePosition;
  int m_startX, m_startY;
};






#endif //XTREESEED_H_
