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

#ifndef XTREEBUTTON_H_
#define XTREEBUTTON_H_

#include "ofMain.h"
#include "ofxTimer.h"

#define BTN_FADE_TIME 2000.F

class XTreeButton {
public:
  enum Type {
    CIRCULAR
  };
  
  XTreeButton(std::string label, int x, int y, int radius_);
  
  void hide();
  
  void show();
  
  void hideImmediately();
  
  void showImmediately();
  
  void setOpacity(float percent_);
  
  ofEvent<bool> clickedEvent;
  
private:
  void draw(ofEventArgs& args);
  
  void update(ofEventArgs& args);
  
  void mouseMoved(ofMouseEventArgs& args);  
  
  void mousePressed(ofMouseEventArgs& args);  
  
  void listenToMouse(bool listen_);

  std::string m_label;
  int m_x, m_y, m_radius;
  bool m_hover;
  bool m_fadingIn, m_fadingOut, m_visible, m_listening;
  ofxTimer m_timer;
  float m_alphaLevel;
  int m_transitionTime;
  ofColor m_mainColor, m_hoverColor, m_textColor, m_textHoverColor;
};

#endif
