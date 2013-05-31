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

#include "LeavesLayer.h"
#include "Settings.h"
#include "MSACore.h"
#include "MSAFluidSolver.h"
#include "InteractiveAudio.h"


//----------------------

LeavesLayer::LeavesLayer() :
  m_running(false) {
  
  
  s_solver = new FluidSolver();
  s_windowSize = Vec2f(ofGetWidth(),ofGetHeight());
	s_invWindowSize = Vec2f( 1.0f / s_windowSize.x, 1.0f / s_windowSize.y );
  s_solver->setup(100, 100);
  s_solver->enableRGB(true).setFadeSpeed(0.01).setDeltaT(0.4).setVisc(0.0001).setColorDiffusion(0);
  
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

}

LeavesLayer::~LeavesLayer() {
  reset();
}

void LeavesLayer::start() {
  //if (!m_running) {
    //m_timer.setAlarm(1000);
    m_running = true;
  //}
}

void LeavesLayer::stop() {
  if (m_running) {
    m_timer.stop();
    m_running = false;
  }
}

void LeavesLayer::reset() {
  m_it = m_leaves.begin();
  while (m_it != m_leaves.end()) {
    delete *m_it;
    ++m_it;
  }
  m_leaves.clear();
  stop();
}

void LeavesLayer::update() {

  //always: gravity + wind
  s_solver->addConstantForce(Vec2f(0.f,0.000004f)); 
  s_solver->update();
  
  if (g_globalCounterSec % 3 == 0) {  
    m_it = m_leaves.begin();
    while (m_it != m_leaves.end()) {
      if ((*m_it)->age >= 1.f) {
        delete *m_it;
        *m_it = NULL;
      }
      ++m_it;
    }
    m_leaves.erase(std::remove_if(m_leaves.begin(), m_leaves.end(), std::bind2nd(std::equal_to<Leaf*>(), NULL)), m_leaves.end());
  }
  

  m_it = m_leaves.begin();
  while (m_it != m_leaves.end()) {
    (*m_it)->update();
    ++m_it;
  }
}

void LeavesLayer::draw() {
  m_it = m_leaves.begin();
  while (m_it != m_leaves.end()) {
    (*m_it)->draw();
    ++m_it;
  }
  
}

void LeavesLayer::addLeaf(Leaf* leaf) {
  if (!m_running)
    return;
  m_leaves.push_back(leaf);
  std::vector<float> msg;
  msg.push_back(2000);    // time
  msg.push_back(0); // panning  
  InteractiveAudio::instance()->sendFloatList(kNewLeaf, msg);
}

void LeavesLayer::setWindForce(float force_) {
  m_windForce = 0.000001f * force_;
}


void LeavesLayer::detachLeaf(Leaf* leaf) {
  std::list<Leaf*>::iterator it = std::find(m_leaves.begin(), m_leaves.end(), leaf);
  if (it != m_leaves.end()) {
    (*it)->detach();
  }
}

  