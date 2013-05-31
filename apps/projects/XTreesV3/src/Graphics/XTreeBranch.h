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


#ifndef XTREEBRANCHBRANCH_H
#define XTREEBRANCHBRANCH_H

#include "XTreeBranch.h"
#include <vector>
#include "Settings.h"
#include "ofMain.h"
#include "ofxTimer.h"
#include "GraphicUtilities.hpp"

class FlowersLayer;
class XTree;

class XTreeBranch {
  friend class FlowersLayer;
  friend class LinesLayer;
public:
  XTreeBranch(XTree* tree_, XTreeBranch* previous_, uint width_,  ofVec2f start_, ofVec2f end_);
  ~XTreeBranch();
  
  void update();
  
  void draw();
  
  bool canBeEvolved() {
    return !m_justActivated;
  }
  
  float getRotation() {
    return m_rotation;
  }
    
  virtual XTreeBranch* returnInactiveBranch();
  
  ofVec2f* currentEnd() {
    return &m_currentEnd;
  }
  
  ofVec2f end() {
    return m_end;
  }
  
  int getLevel() {
    return m_level;
  }
  
  Leaf* addLeaf();
  
private:
  XTree* m_tree;
  XTreeBranch* previous;
  unsigned int m_level;  
  unsigned int m_age;  
  std::vector<XTreeBranch *> children;
  std::vector<size_t> indices;
  bool m_active;
  float m_rotation;
  float m_growthAngle;
  
  uint m_length;
  uint m_width;
  
  ofVec2f m_start;
  ofVec2f m_end;
  
  ofVec2f m_cross;
  
  ofVec2f m_currentStart;
  ofVec2f m_currentEnd;
  
  ofImage* m_image;
  
  ofxTimer m_timer;
  
  ofVec2f m_currentStringPosition;
  char m_lengthString[6];
  
  bool m_justActivated;
  float m_growthTime;
  
  int m_numChildren;
  
  std::vector<int> m_positions;
  
  ofPoint m_startPosition;
  
  int m_numLeaves;

  std::vector<float> stemProportions;
  std::vector<Leaf*> leaves;
  std::vector<bool> usedLeaves;
  std::vector<float> leavesAngles;
  
  void addChild(XTreeBranch * node_) {
    children.push_back(node_);
    indices.push_back(indices.size());
  }
  
};

#endif
