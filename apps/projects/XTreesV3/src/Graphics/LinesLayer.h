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


#ifndef LINESLAYER_H_
#define LINESLAYER_H_

#include "ofMain.h"
#include <vector>
#include "TreesManager.h"
#include "ofxTimer.h"
#include <tr1/memory>

class LinesLayer {
public:
  struct XtreeLine {
    ofPoint start, end;
    float opacity;
    float life;
    float currentLife;
    void update();
  };
  LinesLayer(std::tr1::shared_ptr<TreesManager> trees);  
  ~LinesLayer();
  
  void update();
  void draw();
  void start();
  void stop();
  void addLine(ofPoint start_, ofPoint end_);
  void reset();
  void preActivate() {}
  
private:
  std::tr1::shared_ptr<TreesManager> m_trees;
  std::vector	<XtreeLine> m_lines; 
  std::vector	<XtreeLine>::iterator m_it;  
  
  bool m_active;
  bool m_running;
  ofxTimer m_timer;
  
};









#endif 
