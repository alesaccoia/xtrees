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


#ifndef __LEAVESLAYER_H__
#define __LEAVESLAYER_H__

#include <queue>

#include "Leaf.h"
#include "ofMain.h"
#include "ofxTimer.h"


class LeavesLayer {
public:
	LeavesLayer();
	~LeavesLayer();
  void start();
  void stop();
  void stopGrowing();
  void update();
  void draw();
  void reset();
  void addLeaf(Leaf* leaf);
  void detachLeaf(Leaf* leaf);
  float opacity;
  
  inline int numberOfLeaves() {
    return m_leaves.size();
  }
  void detachLeaves() {
    m_it = m_leaves.begin();
    while (m_it != m_leaves.end()) {
      (*m_it)->detach();
      ++m_it;
    }
  }
  bool m_running;
  std::list<Leaf*> m_leaves;
  
  void setWindForce(float force_); // 0-3
  
private:
  std::list<Leaf*>::iterator m_it;
  ofxTimer m_timer;  
  float m_windForce;
};

#endif // __LEAVESLAYER_H__
