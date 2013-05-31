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


#include "Leaf.h"
#include "Settings.h"
#include "KepleroUtils.h"

#define HAS_STEM 1

static const float MOMENTUM = .5f; // nice values: .5f 
static const float ROTATION_MOMENTUM = .96f; // nice values: .5f 
static const float FLUID_FORCE = 0.1f;
static const float WIDTH = 150.f;
static const float DETACHINGSPEED = .01f;

static const float ATTACHANGLE = .15f;

static const float FADE_IN_LIFE = 0.3F;
static const int MIN_STEMLIFE = 100;
static const int MAX_STEMLIFE = 200;
static const int MIN_WIDTH = 100;
static const int MAX_WIDTH = 160;
static const float DETACHING_AGE = .7f;

/*
Story of the leaf:
When it is created, it's not attached by default, but it can be attached by the caller
If it is attached, it can be detached when the |wind speed| > DETACHINGSPEED
In this case, her bottomleft and topright corners become susceptible to the wind,
because they get a mass
The leaf has a life, that is decided randomly between LIFE_MIN and LIFE_MAX
The detaching speed is inversely proportional to the life, the color becomes darker
proportionally to the life.
Also the stem has a life.
When the leaf is detached, its stem's start to age, and gets shorter and shorter
The frame of the leaf becomes also smaller to accomodate the smaller texture.
During the update the rest position of the springs gets shrunk so that the
leaf gets smaller in size
*/

ofImage Leaf::leafImage;
float Leaf::leafImageW;
float Leaf::leafImageH;

Leaf::Leaf(ofVec3f attach, ofVec3f direction, ofVec3f normal) :
  alpha(.0f),
  life(.0f),
  stemLife(.0f),
  age(.0f),
  stemAge(.0f),
  isAttached(true),
  isJustBorn(true),
  ageBorn(0.f) {
  leftRight = ofRandomuf() < .5f ? -1 : 1;
  lifespan = ofRandom(std::min(0.f,(g_leavesLife - 10) * g_fps), g_leavesLife * g_fps);
  width = ofRandom(g_leavesMinWidth, g_leavesMaxWidth);
  m_image = g_leafImages[(size_t)ofRandom(g_leafImages.size())];
  init(attach, direction, normal);
}

// the leaf will be created on the plane identified
// by direction and normal
void Leaf::init(ofVec3f attach, ofVec3f direction, ofVec3f normal) {
  direction = direction.getNormalized(); // normalize
  direction.rotate(-leftRight * (20.f + 10.f * ofRandomuf()), ofVec3f(0,0,1));
  normal.z *= leftRight;
  ofVec3f otherAxis = direction.getPerpendicular(normal);

  /*
  dir
  |   /normal
  |  / 
  | /
  |/
  \----> otherAxis
  
  p1 p2
  p4 p3
  */
  Joint p1(attach + direction * width, 1, .96);
  //p1.pos.z += ofRandomf() * 2.f;
  Joint p2(attach + direction * width + otherAxis * width, 0., .96);
  //p2.pos.z += ofRandomf() * 2.f;
  Joint p3(attach + otherAxis * width, 1, .96);
  //p3.pos.z += ofRandomf() * 2.f;
  Joint p4(attach, 0, .96);
  joints.push_back  (p1);
  joints.push_back(p2);
  joints.push_back(p3);
  joints.push_back(p4);
  
  springs.push_back(Spring(&joints[0],&joints[1]));
  springs.push_back(Spring(&joints[1],&joints[2]));
  springs.push_back(Spring(&joints[2],&joints[3]));
  springs.push_back(Spring(&joints[3],&joints[0]));
  // try to mantain the shape of the leaf
  springs.push_back(Spring(&joints[0],&joints[2]));
  springs.push_back(Spring(&joints[1],&joints[3]));
  
  m_timer.setAlarm(ofRandom(5000, 10000));
}

void Leaf::move(ofVec3f attach, ofVec3f direction, ofVec3f normal) {
  direction = direction.getNormalized(); // normalize
  normal.z *= leftRight;
  ofVec3f otherAxis = direction.getPerpendicular(normal);
  joints[1].pos = ofVec3f(attach + direction * springs[0].rest_length + otherAxis * springs[0].rest_length);
  joints[3].pos = ofVec3f(attach);
}

void Leaf::update() {
#if HAS_STEM
  if (!isAttached) {
    age = std::min(++life/lifespan, 1.f);
//    stemAge = std::min(++stemLife/stemLifespan, 1.f);
    // shrink leaf
//    for (int i = 0; i < springs.size(); ++i)
//      springs[i].shrink(1.f - 0.5f * stemAge);
  } else if (isJustBorn) {
    ageBorn = std::min(++life/lifespan, 1.f); 
    if (ageBorn > FADE_IN_LIFE) {
      life = 0;
      isJustBorn = false;
    }
  }

#endif

  Vec2f absSpeed(0.f,0.f);
  for (int i=0; i<joints.size(); i++) {
    Vec2f positionIn2d(joints[i].pos.x, joints[i].pos.y);
    Vec2f solverVel = s_solver->getVelocityAtPos(positionIn2d * s_invWindowSize );
    // bounce of edges
    if( positionIn2d.x < 0 || positionIn2d.x > s_windowSize.x) {
      solverVel.x *= -10;
    }
    if( positionIn2d.y < 0) {
      solverVel.y *= -10;
    }
    if (positionIn2d.y > s_windowSize.y) {
      solverVel.y *= -10;
      joints[i].pos.y = s_windowSize.y;
    }

    joints[i].addForce(ofVec3f(solverVel.x * 100,solverVel.y * 100,0));
    absSpeed += solverVel;
  }

  //detach
 // if (isAttached && (absSpeed.length() > (DETACHINGSPEED/age) || age >  DETACHING_AGE)) {
 //   detach();
 // }

  //update springs
  for (int i=0; i<springs.size(); i++) {
    springs[i].update();
  }

  //update joints
  for (int i=0; i<joints.size(); i++) {
    joints[i].update();
  }

}

void Leaf::detach() {
  if (!isAttached) 
    return;
  for (int i=0; i<joints.size(); i++) {
    joints[i].mass = 1.f;
    joints[i].inverse_mass = 1.f;
  }
  isAttached = false;
#if HAS_STEM
  //set lifespan of the stem
//  stemLifespan = std::min(lifespan - life, ofRandom(MIN_STEMLIFE, MAX_STEMLIFE));
#endif
}

void Leaf::draw() {

//  for (int i = 0; i < 3; ++i) {
//    ofLine(joints[i].pos.x, joints[i].pos.y, joints[i+1].pos.x, joints[i+1].pos.y); 
//  }
//  ofLine(joints[3].pos.x, joints[3].pos.y, joints[0].pos.x, joints[0].pos.y); 
  
  float alpha = g_leavesOpacity;
  if (isAttached) {
    if (isJustBorn) {
      alpha = g_leavesOpacity * (ageBorn / FADE_IN_LIFE);
    }
  } else {
    alpha = g_leavesOpacity * (1.f - age);
  }

  
//  ofSetColor(kplToColor(g_leavesEndColor, age), 255.f * m_timer.getDiffN());
  ofSetColor(g_leavesEndColor, alpha);
  float halfStemAge = stemAge * .3f;
  
  glEnable(GL_DEPTH_TEST);  
  m_image->getTextureReference().bind();  
  
#ifndef XTREES_IOS

  glBegin(GL_QUADS);
      // Front Face
      glTexCoord2f(halfStemAge * m_image->width, .0f);      
      glVertex3f(joints[0].pos.x, joints[0].pos.y, 0);
      glTexCoord2f(m_image->width, .0f); 
      glVertex3f(joints[1].pos.x, joints[1].pos.y, 0);
      glTexCoord2f(m_image->width, m_image->height * (1.f - halfStemAge)); 
      glVertex3f(joints[2].pos.x, joints[2].pos.y, 0);
      glTexCoord2f(0.5f * halfStemAge * m_image->width, m_image->height * (1.f - halfStemAge)); 
      glVertex3f(joints[3].pos.x, joints[3].pos.y, 0);
  glEnd();   
#endif




  leafImage.getTextureReference().unbind();   
}

