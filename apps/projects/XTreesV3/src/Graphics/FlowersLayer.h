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

#ifndef FLOWERSLAYER_H_
#define FLOWERSLAYER_H_

#include "ofMain.h"
#include <vector>
#include "TreesManager.h"
#include "ofxTimer.h"

class FlowersLayer {
public:
  struct FlowersOrb {
    ofPoint start, end;
    float circleWidth;
    float opacity;
    float life;
    float maxRadius;
    float currentLife;
    void update();
  };
  FlowersLayer(TreesManager* trees);  
  ~FlowersLayer();
  
  void update();
  void draw();
  void start();
  void stopGrowing();
  void stop();
  void addLine(ofPoint start_, ofPoint end_);
  void reset();

  
private:
  TreesManager* m_trees;
  std::vector	<FlowersOrb> m_lines; 
  std::vector	<FlowersOrb>::iterator m_it;  
  
  bool m_active;
  ofxTimer m_timer;
  
};



#endif 
