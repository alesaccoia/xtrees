/*
 *  LeavesMovement.h
 *  XTreesV3
 *
 *  Created by Alessandro Saccoia on 3/29/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __LEAVESMOVEMENT_H__
#define __LEAVESMOVEMENT_H__

#include <vector>
#include "ofMain.h"
#include "Joint.h"
#include "Spring.h"

class LeavesLayer;

class LeavesMovement {
public:
	LeavesMovement(LeavesLayer* layer_) :
    layer(layer_),
    completion(0.f) {
    
  }
	virtual ~LeavesMovement() = 0;
  virtual void start() = 0;
  virtual void update() = 0;
  inline bool isCompleted() {
    return completion >= 1.f;
  }
  LeavesLayer* layer;
  float completion;
  float duration;
  bool m_automode;
};

class Scattering :
  public LeavesMovement {
public:
  Scattering(LeavesLayer* layer_) :
    LeavesMovement(layer_) {
    duration = 30 * 10;
  }
  
	~Scattering();
  void start();
  void update();
  std::vector<Joint*> m_joints;
  std::vector<Spring*> m_springs;
};

class RowColumns :
  public LeavesMovement {
public:
  RowColumns(LeavesLayer* layer_) :
    LeavesMovement(layer_) {
    duration = 50;
  }
  
	~RowColumns();
  void start();
  void update();
  std::vector<Joint*> m_joints;
  std::vector<Spring*> m_springs;
};


#endif // __LEAVESMOVEMENT_H__
