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


#include "FlowersLayer.h"
#include "TreesManager.h"
#include "KepleroUtils.h"
#include "Settings.h"

namespace  {
  inline bool isDead(FlowersLayer::FlowersOrb& line) {
    return line.currentLife < 0.F;
  }
}



FlowersLayer::FlowersLayer(TreesManager* trees) :
  m_trees(trees),
  m_active(false) {
  
}

FlowersLayer::~FlowersLayer() {
  
}

void FlowersLayer::start() {
  m_active = true;
  m_timer.set();
  m_timer.setAlarm(ofRandom(g_flowersMin, g_flowersMax));
}

void FlowersLayer::stop() {
  m_lines.erase(std::remove_if(m_lines.begin(), m_lines.end(), isDead), m_lines.end());
  m_lines.clear();
  stopGrowing();
}

void FlowersLayer::stopGrowing() {
  m_active = false;
  m_timer.stop();
}

void FlowersLayer::update() {
  if (m_active) {
    if (m_timer.alarm() && m_trees->m_trees.size() > 0) {
      // search the trees for available branches
      int tree = kplToss(m_trees->m_trees.size());
      int maxLevel = m_trees->m_trees[tree]->getCurrentLevel();
      XTreeBranch* branch = m_trees->m_trees[tree]->m_trunk;
      if (branch != NULL && branch->canBeEvolved()) {
        while (maxLevel-- >= 0) {
          if (branch != NULL && branch->canBeEvolved()) {
            if (branch->children.size() > 0) {
              branch = branch->children[kplToss(branch->children.size())];
            } else {
              break;
            }
          }
        }
        
        
        ofPoint start(branch->m_currentEnd.x, branch->m_currentEnd.y);
        
        XTreeBranch* first = branch;
        
        //tree = kplToss(m_trees->m_trees.size());
        maxLevel = m_trees->m_trees[tree]->getCurrentLevel();
        branch = m_trees->m_trees[tree]->m_trunk;
        
        if (branch != NULL) {
          while (maxLevel-- >= 0) {
            if (first != branch && branch != NULL && branch->canBeEvolved()) {
              if (branch->children.size() > 0) {
                branch = branch->children[kplToss(branch->children.size())];
              } else {
                break;
              }
            }
          }
          ofPoint end(branch->m_currentEnd.x, branch->m_currentEnd.y);
          addLine(start,end);
        }
        
      }
      m_timer.set();
      m_timer.setAlarm(ofRandom(g_flowersMin, g_flowersMax));
    }
    
  }
  m_it = m_lines.begin();
  while (m_it != m_lines.end())
    (*m_it++).update();
  // garbage collection every from time to time
  if (g_globalCounterSec % 3 == 0) {    
    m_lines.erase(std::remove_if(m_lines.begin(), m_lines.end(), isDead), m_lines.end());
  }
}

void FlowersLayer::draw() {
  if (m_lines.size()) {
    m_it = m_lines.begin();
    ofSetLineWidth(g_crazyLineWidth);
    ofFill();
    while (m_it != m_lines.end()) {
      ofSetColor(g_flowersColor, m_it->opacity);
      ofPushMatrix();
      g_flowerImage.draw(m_it->start.x - m_it->circleWidth/2, m_it->start.y-m_it->circleWidth/2, m_it->circleWidth, m_it->circleWidth);
      g_flowerImage.draw(m_it->end.x - m_it->circleWidth/2, m_it->end.y-m_it->circleWidth/2, m_it->circleWidth, m_it->circleWidth);
      ofPopMatrix();
      ++m_it;
    }
  }
}

void FlowersLayer::addLine(ofPoint start_, ofPoint end_) {
  m_lines.push_back(FlowersOrb());
  m_lines[m_lines.size() - 1].opacity = ofRandom(50,255);
  m_lines[m_lines.size() - 1].start = start_;
  m_lines[m_lines.size() - 1].end = end_;
  m_lines[m_lines.size() - 1].life = ofRandom(200,410);
  m_lines[m_lines.size() - 1].currentLife = m_lines[m_lines.size() - 1].life;
  m_lines[m_lines.size() - 1].circleWidth = g_crazyCircleWidth;
}

void FlowersLayer::FlowersOrb::update() {
  currentLife -= 1.F;
  opacity = (currentLife / life) * 155.F;
  circleWidth = (1.F - (currentLife / life)) * 360.F;
}


