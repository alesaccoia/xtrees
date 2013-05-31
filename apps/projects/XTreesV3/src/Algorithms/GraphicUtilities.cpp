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

#include "GraphicUtilities.hpp"


#include "ofMain.h"
#ifdef TARGET_WIN32
	#define GLUT_BUILDING_LIB
	#include "glut.h"
#endif
#ifdef TARGET_OSX
  #ifndef XTREES_SCREENSAVER
    #include "../../../libs/glut/lib/osx/GLUT.framework/Versions/A/Headers/glut.h"
  #endif
	#include <Carbon/Carbon.h>
#endif
#ifdef TARGET_LINUX
	#include <GL/glut.h>
#endif


// TODO : a better way that preserves brightness
// http://stackoverflow.com/questions/7521058/colorize-grayscale-image

void kplColorize(ofImage* from, ofImage* to,  ofColor& color_) {
  ofColor now;
  ofColor then;
  float lightness;
  for (int x = 0; x < from->getWidth(); ++x) {
      for (int y = 0; y < from->getHeight(); ++y) {
        now = from->getColor(x, y);
        lightness = now.getLightness()/255.F;
        then.a = now.a;
        then.r = color_.r * lightness;
        then.g = color_.g * lightness;
        then.b = color_.b * lightness;
        to->setColor(x, y, then);
      }
  }
  to->update();
}

void kplSetMaxAlpha(ofImage* fromTo) {
  ofColor now;
  for (int x = 0; x < fromTo->getWidth(); ++x) {
      for (int y = 0; y < fromTo->getHeight(); ++y) {
        now = fromTo->getColor(x, y);
        if (now.a != 0) {
          now.a = std::max(now.a + 126, 255);
        }        
        fromTo->setColor(x, y, now);
      }
  }
  fromTo->update();
}

float kplNormalizeAngle(float angle_) {
  angle_ = (float) fmod((double)angle_, 2. * PI);
  if (angle_ < 0)
    return 2. * M_PI + angle_;
  return angle_;
}










