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

#ifndef __LEAF_H__
#define __LEAF_H__

#include "MSACore.h"
#include "MSAFluidSolver.h"
#include "Joint.h"
#include "Spring.h"
#include "ofxTimer.h"

using namespace MSA;

struct Leaf {
  Leaf(ofVec3f attach, ofVec3f direction, ofVec3f normal);
  void update();
  void init(ofVec3f attach, ofVec3f direction, ofVec3f normal);
  void move(ofVec3f attach, ofVec3f direction, ofVec3f normal);
  void draw();
  void detach();
  
  float alpha;
  
  vector<Joint> joints;
  vector<Spring> springs;
  
  bool isAttached;  
  bool isJustBorn;
  
  float leftRight;
  float lifespan;
  float life;
  float stemLife;
  float stemLifespan;
  float age;
  float stemAge;
  float width;
  float ageBorn;
  
  ofxTimer m_timer;
  
  ofImage* m_image;
  
  static ofImage leafImage;
  static float leafImageW;
  static float leafImageH;
};
 

#endif // __LEAF_H__
