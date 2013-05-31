/*
 *  LeavesMovement.cpp
 *  XTreesV3
 *
 *  Created by Alessandro Saccoia on 3/29/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "LeavesMovement.h"
#include "Settings.h"
#include "LeavesLayer.h"
#include "MSACore.h"
#include "MSAFluidSolver.h"

#define BORDERX 300
#define BORDERYT 50
#define BORDERYB 300

LeavesMovement::~LeavesMovement() {

}

Scattering::~Scattering() {

}

void Scattering::start() {
  duration = 30 * 10;
  int totalLeaves = g_leavesRows * g_leavesColumns;
  int incrementV = (ofGetHeight() - (BORDERYB + BORDERYT)) / (g_leavesRows - 1);
  int incrementH = (ofGetWidth() - 2 * BORDERX) / (g_leavesColumns - 1);
  m_joints.resize(totalLeaves * 2);
  m_springs.resize(totalLeaves * 2);
  for (int i = 0, x = BORDERX, y = BORDERYT; i < g_leavesRows; ++i, x = BORDERX) {
    for (int j = 0; j < g_leavesColumns; ++j) {
      m_joints[i * g_leavesColumns + j] = new Joint(ofVec3f(x-5,y,0), 0);
      m_joints[i * g_leavesColumns + j] = new Joint(ofVec3f(x+5,y,0), 0);
      //std::cout << i << ":" << j << "=" << x << " " << y << std::endl;
      x += incrementH;
    }
    y += incrementV;
  }
  layer->start();
  g_leavesActive = true;
  if (layer->numberOfLeaves() < g_leavesRows * g_leavesColumns) {
    return;
  }

  std::list<Leaf*>::iterator it = layer->m_leaves.begin();
  int i = 0;
  while (it != layer->m_leaves.end()) {
    m_springs[i] = new Spring(&((*it)->joints[0]), m_joints[i/2], .0008f);
    m_springs[i]->originalrest_length = m_springs[i]->rest_length = .0f;
    ++i;
    m_springs[i] = new Spring(&((*it)->joints[2]), m_joints[i/2], .0008f);
    m_springs[i]->originalrest_length = m_springs[i]->rest_length = .0f;
    ++i;
    (*it)->detach();
    ++it;
  }
}

void Scattering::update() {
  //update springs
  for (int i=0; i<m_springs.size(); i++) {
    m_springs[i]->update();
  }

  //update joints
//  for (int i=0; i<m_joints.size(); i++) {
//    m_joints[i]->update();
//  }
  
  completion += 1.f / duration;

}

// rowColumns

RowColumns::~RowColumns() {

}


void RowColumns::start() {
  duration = 30 * 10;
  int totalLeaves = g_leavesRows * g_leavesColumns;
  int incrementV = (ofGetHeight() - (BORDERYB + BORDERYT)) / (g_leavesRows - 1);
  int incrementH = (ofGetWidth() - 2 * BORDERX) / (g_leavesColumns - 1);
  m_joints.resize(totalLeaves * 2);
  m_springs.resize(totalLeaves * 2);
  for (int i = 0, x = BORDERX, y = BORDERYT; i < g_leavesRows; ++i, x = BORDERX) {
    for (int j = 0; j < g_leavesColumns; ++j) {
      m_joints[i * g_leavesColumns + j] = new Joint(ofVec3f(x-5,y,0), 0);
      m_joints[i * g_leavesColumns + j] = new Joint(ofVec3f(x+5,y,0), 0);
      //std::cout << i << ":" << j << "=" << x << " " << y << std::endl;
      x += incrementH;
    }
    y += incrementV;
  }
  layer->start();
  g_leavesActive = true;
  if (layer->numberOfLeaves() < g_leavesRows * g_leavesColumns) {
    return;
  }

  std::list<Leaf*>::iterator it = layer->m_leaves.begin();
  int i = 0;
  while (it != layer->m_leaves.end()) {
    m_springs[i] = new Spring(&((*it)->joints[0]), m_joints[i/2], .0008f);
    m_springs[i]->originalrest_length = m_springs[i]->rest_length = .0f;
    ++i;
    m_springs[i] = new Spring(&((*it)->joints[2]), m_joints[i/2], .0008f);
    m_springs[i]->originalrest_length = m_springs[i]->rest_length = .0f;
    ++i;
    (*it)->detach();
    ++it;
  }

}

void RowColumns::update() {
  //update springs
  for (int i=0; i<m_springs.size(); i++) {
    m_springs[i]->update();
  }

  //update joints
//  for (int i=0; i<m_joints.size(); i++) {
//    m_joints[i]->update();
//  }
  
  completion += 1.f / duration;
}

