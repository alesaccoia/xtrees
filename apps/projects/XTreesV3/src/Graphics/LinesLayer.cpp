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

#include "LinesLayer.h"
#include "TreesManager.h"
#include "KepleroUtils.h"
#include "Settings.h"
#include "InteractiveAudio.h"

namespace  {
  inline bool isDead(LinesLayer::XtreeLine& line) {
    return line.currentLife < 0.F;
  }
}



LinesLayer::LinesLayer(std::tr1::shared_ptr<TreesManager> trees) :
  m_trees(trees),
  m_active(false),
  m_running(false) {
  
}

LinesLayer::~LinesLayer() {
  m_lines.clear();
}

void LinesLayer::start() {
  m_active = true;
  m_running = true;
  m_timer.set();
  m_timer.setAlarm(ofRandom(g_linesMin, g_linesMax));
}

void LinesLayer::stop() {
  //m_lines.clear();
  m_active = false;
  m_timer.stop();
}

void LinesLayer::update() {
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
          InteractiveAudio::instance()->sendBang(kNewLine);
        }
        
      }
      m_timer.set();
      m_timer.setAlarm(ofRandom(g_linesMin, g_linesMax));
    }    
  }
  m_it = m_lines.begin();
  while (m_it != m_lines.end())
    (*m_it++).update();
  m_lines.erase(std::remove_if(m_lines.begin(), m_lines.end(), isDead), m_lines.end());  
}

void LinesLayer::draw() {
  if (m_active) {
    m_it = m_lines.begin();
    ofSetLineWidth(g_crazyLineWidth);
    ofFill();
    while (m_it != m_lines.end()) {
      ofSetColor(g_flowersColor, m_it->opacity);
      ofPushMatrix();
      ofSetColor(g_flowersLineColor, m_it->opacity);
      ofLine(m_it->start.x, m_it->start.y, m_it->end.x, m_it->end.y);
//      ofCircle(m_it->start.x, m_it->start.y, m_it->circleWidth);
//      ofCircle(m_it->end.x, m_it->end.y, m_it->circleWidth);
      ofPopMatrix();
      ++m_it;
    }
  }
}

void LinesLayer::addLine(ofPoint start_, ofPoint end_) {
  m_lines.push_back(XtreeLine());
  m_lines[m_lines.size() - 1].opacity = ofRandom(50,255);
  m_lines[m_lines.size() - 1].start = start_;
  m_lines[m_lines.size() - 1].end = end_;
  m_lines[m_lines.size() - 1].life = ofRandom(200,410);
  m_lines[m_lines.size() - 1].currentLife = m_lines[m_lines.size() - 1].life;
}

void LinesLayer::XtreeLine::update() {
  currentLife -= 1.F;
  opacity = (currentLife / life) * 155.F;
}


