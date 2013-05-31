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

#ifndef MATHUTILITIES_H_
#define MATHUTILITIES_H_

#include "ofMain.h"

inline ofPoint ofMidPoint(ofPoint& p1, ofPoint& p2) {
	return ofPoint((p1.x + p2.x) / 2., (p1.y + p2.y) / 2.);
}

// Returns a point that is dist_ pixels far in the direction direction_ in radians
// from the point pivot_ 
inline ofPoint ofPointFromPivot(ofPoint& pivot_, 
  double direction_,
  float dist_) {
  return ofPoint(pivot_.x + cos(direction_) * dist_,
    pivot_.y + sin(direction_) * dist_);
}
 
inline ofPoint ofPointFromPivot(ofVec2f& pivot_, 
  double direction_,
  float dist_) {
  return ofPoint(pivot_.x + cos(direction_) * dist_,
    pivot_.y + sin(direction_) * dist_);
}


// angle of line passing by 2 points
// (change order of the arguments for upside down side)
inline float ofAngle(ofPoint& p1, ofPoint& p2) {
  float x = p2.x - p1.x;
  float y = p2.y - p1.y;
  if (x < 0) {
    if (y < 0) {
      return 2 * M_PI + atan2(y,x); 
    }

  } else {
    if (y < 0) {
      return atan2(y,x);
    }
  }
  return atan2(y,x);
}


// left side perpendicular to the line between 2 points
// (change order of the arguments for right side)
inline float ofPerpendicular(ofPoint& p1, ofPoint& p2) {
  float coeff = ofAngle(p1,p2);
  return coeff - M_PI / 2.;
}

#endif