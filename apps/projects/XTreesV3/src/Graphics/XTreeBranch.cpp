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


#include "XTreeBranch.h"
#include "XTree.h"
#ifndef XTREES_SCREENSAVER
#include "InteractiveAudio.h"
#endif
#include "Settings.h"
#include "MathUtilities.h"


XTreeBranch::XTreeBranch(XTree* tree_, XTreeBranch* previous_, uint width_, ofVec2f start_, ofVec2f end_) :
  m_tree(tree_),
  previous(previous_),
  m_age(0),
  m_active(false),
  m_start(start_),
  m_end(end_),
  m_currentStart(start_),
  m_currentEnd(start_),
  m_justActivated(true),
  m_growthTime(0.F),
  m_width(width_) {  
  m_length = ofDist(m_start.x, m_start.y, m_end.x, m_end.y);
  ofPoint thisStart(m_start.x, m_start.y, 0);
  ofPoint thisEnd(m_end.x, m_end.y, 0);
  m_rotation = ofAngle(thisStart, thisEnd);  
  m_startPosition.x = m_start.x + m_width/2 * sin(m_rotation);
  m_startPosition.y = m_start.y - m_width/2 * cos(m_rotation);
  this->m_level = (previous_ ? previous_->m_level + 1 : 0);
  m_image = g_images[(size_t)ofRandom(g_images.size())];
  m_growthTime = ofRandom(g_growthMin, g_growthMax);
  m_timer.set();
#ifndef XTREES_SCREENSAVER
  std::vector<float> msg;
  msg.push_back(m_growthTime);    // time
  msg.push_back(-45.f + 90.F * (float)m_start.x / (float)g_scene.width); // panning  
  InteractiveAudio::instance()->sendFloatList(kNewBranch, msg);
#endif
  m_timer.setAlarm(m_growthTime);  
  m_numChildren = 2 + (m_level + ofRandom(g_fertility)) / 3;
  m_positions.resize(m_numChildren);
  for (int i = 0; i < m_numChildren; ++i)
    m_positions[i] = i;
  std::random_shuffle(m_positions.begin(), m_positions.end());
  m_numLeaves = g_leavesFertility;
  stemProportions.resize(m_numLeaves);
  leaves.resize(m_numLeaves);
  usedLeaves.resize(m_numLeaves);
  leavesAngles.resize(m_numLeaves);
  for (int i = 0; i < m_numLeaves; ++i) {
    stemProportions[i] = ofRandom(m_length);
    leavesAngles[i] = .0f;// ofRandomuf() * M_PI/7.f;
    leaves[i] = NULL;
    usedLeaves[i] = false;
  }

}

XTreeBranch::~XTreeBranch() {
  std::vector<XTreeBranch *>::iterator it = children.begin();
  while (it != children.end()) {
    delete *it++;
  }
  /*
  for (int i = 0; i < m_numLeaves; ++i) {
    g_leavesLayer->detachLeaf(leaves[i]);
  }
  */
}

void XTreeBranch::update() {
  if(m_timer.alarm()) { // TODO so we don't check the timer every time
    if (!m_active) {
      m_justActivated = false;
      m_active = true;
      m_currentEnd = m_end;
      m_timer.stop();
    }
  } else if (m_justActivated ) {
    ofVec2f growth = m_end - m_start;
    m_currentEnd = m_start + growth.getScaled(m_length * pow((double)m_timer.getDiffN(), 0.5));
  }
  
  m_cross.x = m_currentEnd.x;
  m_cross.y = m_currentEnd.y;  

  std::vector<XTreeBranch *>::iterator it = children.begin();
  while (it != children.end()) {
    (*it++)->update();
  }
}

  
void XTreeBranch::draw() {
  ofPushMatrix();
  ofSetRectMode(OF_RECTMODE_CORNER);
  ofTranslate(m_startPosition.x, m_startPosition.y, .0f);
  ofRotateZ(m_rotation * 180.F / M_PI);
  ofSetColor(255, g_treeC.a);
  m_image->draw(0, 0, (m_currentEnd - m_start).length(), m_width);  
  ofTranslate((m_currentEnd - m_start).length(), .0F, .0f);
  ofPopMatrix();    
  
  std::vector<XTreeBranch *>::iterator it = children.begin();
  while (it != children.end()) {
    (*it++)->draw();
  }
  
  ofSetColor(g_jointC);
  
  if (!g_jointsAreCircles) {
    ofSetLineWidth(g_jointThickness);
    ofLine(m_cross.x - g_jointLength, m_cross.y - g_jointLength, m_cross.x + g_jointLength, m_cross.y + g_jointLength);
    ofLine(m_cross.x - g_jointLength, m_cross.y + g_jointLength, m_cross.x + g_jointLength, m_cross.y - g_jointLength);    
  } else {
    ofFill();
    ofCircle(m_cross.x, m_cross.y, g_jointLength);
  }

  ofSetLineWidth(1);
  ofSetColor(255);
  

}
  


XTreeBranch* XTreeBranch::returnInactiveBranch() {
  XTreeBranch * toReturn;
  ofPoint thisEnd(m_end.x, m_end.y, 0);
  if (m_active && m_level < g_maxLevel && g_scene.inside(thisEnd)) {
    if (children.size() < m_numChildren) {
      float direction = m_rotation;
      /*if (g_shapeV1) {
        int mult;
        if (ofRandomuf() > 0.5F) {
          mult = 1;
        } else {
          mult = -1;
        }
        direction += mult * ((g_minAngle + (g_maxAngle - g_minAngle) * ofRandomuf()) * M_PI / 180.F);
      } else { */
        float increment_span = (g_maxAngle - g_minAngle) * 2 / m_numChildren;
        int currentPosition = m_positions[children.size()];
        if (m_numChildren % 2 == 0) {
          if ( currentPosition < m_numChildren / 2) {
            direction += (-g_maxAngle + increment_span * currentPosition + increment_span * ofRandomuf()) * M_PI / 180.F;
          } else if (m_positions[children.size()] >= m_numChildren / 2) {
            direction += (g_minAngle + increment_span * (currentPosition - m_numChildren / 2) + increment_span * ofRandomuf()) * M_PI / 180.F;
          }
        } else {
          if ( currentPosition < m_numChildren / 2) {
            direction += (-g_maxAngle + increment_span * currentPosition + increment_span * ofRandomuf()) * M_PI / 180.F;
          } else if (m_positions[children.size()] > m_numChildren / 2) {
            direction += (g_minAngle + increment_span / 2 + increment_span * (currentPosition - m_numChildren / 2 - 1) + increment_span * ofRandomuf()) * M_PI / 180.F;
          } else { //central
            int mult;
            if (ofRandomuf() > 0.5F) {
              mult = 1;
            } else {
              mult = -1;
            }
            direction += mult * (g_minAngle + (increment_span / 2) * ofRandomuf()) * M_PI / 180.F;
          }
        //}
      }

      
      ofPoint thisStart(m_start.x, m_start.y, 0);      
      
      // todo parametrize randomicity
      float newLength =  m_length * g_lengthDecrease;
      newLength += ofRandomf() * (float)newLength / 10.F;
      newLength = ofClamp(newLength, 6, 1000);
      
      /*
      float newWidth = m_width * g_widthDecrease;
      newWidth += ofRandomf() * (float)newWidth / 10.F;
      newWidth = ofClamp(newWidth, 1, 1000);
      */
      ofVec2f end = ofPointFromPivot(thisEnd, direction, newLength);
      if (!g_scene.inside(end)) {
        return NULL;
      }
      ofVec2f attach = ofPointFromPivot(m_start, atan2(m_currentEnd.y - m_start.y , m_currentEnd.x - m_start.x), m_length * 0.95F);
      toReturn = new XTreeBranch(m_tree, this, std::max(m_width * g_widthDecrease, g_branchMinWidth), attach, end);
      addChild(toReturn);
      return toReturn;
    } else {
      std::random_shuffle(indices.begin(), indices.end());
      std::vector<size_t>::iterator it = indices.begin();
      while (it != indices.end()) {
        toReturn = (children[*(it++)])->returnInactiveBranch();
        return toReturn;
      }
    }
  } else if (m_justActivated) {
    return NULL;
  } else {
    return this;
  }

}

Leaf* XTreeBranch::addLeaf() {
  if (m_justActivated) {
    return NULL;
  }
  Leaf* leaf = NULL;
  
  std::random_shuffle(indices.begin(), indices.end());
  std::vector<size_t>::iterator it = indices.begin();
  while (it != indices.end()) {
    if (leaf = children[*(it++)]->addLeaf())
      return leaf;
  }

  if (previous == NULL) {
    return NULL;
  }
  for (int i = 0; i < m_numLeaves; ++i) {
    if (!usedLeaves[i]) {
      leaves[i] = new Leaf(
      ofVec3f(m_start.x + stemProportions[i] * cos(m_rotation),
      m_start.y + stemProportions[i] * sin(m_rotation),
      0), 
      ofVec3f(cos(m_rotation + leavesAngles[i]),sin(m_rotation + leavesAngles[i]), .0f), 
      ofVec3f(0,0,-1));
      usedLeaves[i] = true;
      leaves[i]->isAttached = true;
      return leaves[i];
    }
  }
  return NULL;
}


