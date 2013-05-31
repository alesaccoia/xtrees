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

#include "XTreeButton.h"
#include "Settings.h"


XTreeButton::XTreeButton(std::string label_, int x, int y, int radius_) :
  m_label(label_),
  m_x(x),
  m_y(y),
  m_radius(radius_),
  m_hover(false),
  m_mainColor(255),
  m_hoverColor(255),
  m_textColor(255),
  m_textHoverColor(0),
  m_fadingIn(false),
  m_fadingOut(false),
  m_transitionTime(0),
  m_alphaLevel(.0F),
  m_visible(false)  {
  
}

void XTreeButton::listenToMouse(bool listen_) {
  if (listen_) {
    ofAddListener(ofEvents.mouseMoved, this, &XTreeButton::mouseMoved);
    ofAddListener(ofEvents.mousePressed, this, &XTreeButton::mousePressed); 
  } else {
    ofRemoveListener(ofEvents.mouseMoved, this, &XTreeButton::mouseMoved);
    ofRemoveListener(ofEvents.mousePressed, this, &XTreeButton::mousePressed);     
  }
}


void XTreeButton::hide() {
  if (!m_visible) {
    ofAddListener(ofEvents.draw, this, &XTreeButton::draw);
    ofAddListener(ofEvents.update, this, &XTreeButton::update);
    m_visible = true;
  }
  if (m_fadingIn) {
    m_transitionTime = m_timer.getDiff();
  }
  listenToMouse(false);
  m_timer.set();
  m_timer.setAlarm(BTN_FADE_TIME - m_transitionTime);
  m_fadingOut = true;
  m_hover = false;
}

void XTreeButton::show() {
  if (!m_visible) {
    ofAddListener(ofEvents.draw, this, &XTreeButton::draw);
    ofAddListener(ofEvents.update, this, &XTreeButton::update);
    m_visible = true;
  }
  if (m_fadingOut) {
    m_transitionTime = m_timer.getDiff();
  }
  m_timer.set();
  m_timer.setAlarm(BTN_FADE_TIME - m_transitionTime);
  m_fadingIn = true; 
  m_hover = false;
}

void XTreeButton::hideImmediately() {
  if (m_visible) {
    ofRemoveListener(ofEvents.draw, this, &XTreeButton::draw);
    ofRemoveListener(ofEvents.update, this, &XTreeButton::update);
    m_visible = false;
  }
  listenToMouse(false);
  m_fadingOut = false;
  m_fadingIn = false;
  m_hover = false;
}

void XTreeButton::showImmediately() {
  if (!m_visible) {
    ofAddListener(ofEvents.draw, this, &XTreeButton::draw);
    ofAddListener(ofEvents.update, this, &XTreeButton::update);    
    m_visible = true;
  }
  listenToMouse(true);
  m_fadingOut = false;
  m_fadingIn = false;
  m_hover = false;
  m_alphaLevel = 1.f;
}

void XTreeButton::draw(ofEventArgs& args) {
  ofPushMatrix();
  ofSetLineWidth(2);
  if (m_hover) {
    ofSetColor(m_hoverColor);
    ofFill();
  } else {
    ofSetColor(m_mainColor, 255.F * m_alphaLevel);
    ofNoFill();
  }
  ofTranslate(m_x, m_y, 0);
  ofCircle(0,0, m_radius);
  if (m_hover) {
    ofSetColor(m_textHoverColor);
  } else {
    ofSetColor(m_textColor, 255.F * m_alphaLevel);
  }
  glScalef(1,-1,1);
  ofTranslate(-150, -4,0);
  g_guiRenderer->Render(m_label.c_str());
  ofSetColor(255);
  ofPopMatrix();
}

void XTreeButton::mouseMoved(ofMouseEventArgs& args) {
  if (ofDist(args.x, args.y, m_x, m_y) <= m_radius) {
    m_hover = true;
  } else {
    m_hover = false;
  }
}

void XTreeButton::mousePressed(ofMouseEventArgs& args) {
  bool fff = true;
  if (ofDist(args.x, args.y, m_x, m_y) <= m_radius)
    ofNotifyEvent(clickedEvent, fff, this);
}

void XTreeButton::update(ofEventArgs& args) {
  if (m_fadingOut) {
    if (m_timer.alarm()) {
      m_timer.stop();
      ofRemoveListener(ofEvents.draw, this, &XTreeButton::draw);
      ofRemoveListener(ofEvents.update, this, &XTreeButton::update);
      m_visible = false;
      m_fadingOut = false;
      m_transitionTime = 0;
    } else {
      m_alphaLevel = 1.F - (m_transitionTime + m_timer.getDiff()) / BTN_FADE_TIME;
    }
  } else if (m_fadingIn) {
    if (m_timer.alarm()) {
      m_timer.stop();
      listenToMouse(true);
      m_fadingIn = false;
      m_transitionTime = 0;
    } else {
      m_alphaLevel = (m_transitionTime + m_timer.getDiff()) / BTN_FADE_TIME;
    }
  }
}



 
 