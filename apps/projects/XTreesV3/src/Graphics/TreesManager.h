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

 
#ifndef TREESMANAGER_H
#define TREESMANAGER_H

#include <string>
#include <vector>
#include <tr1/memory>

#include "ofMain.h"

#include "XTree.h"
#ifdef XTREES_APP
#include "textInput.h"
#endif
#include "Settings.h"
#include "TwitterLayer.h"

class FlowersLayer;
class Leaf;

class TreesManager {
  friend class FlowersLayer;
  friend class LinesLayer;
public:
  TreesManager(std::tr1::shared_ptr<TwitterLayer> twitterLayer_);
  ~TreesManager();
  
  void update();
  void draw();
  
  bool start();
  void reset();
  void stop();
  
  void stopGrowing();
  void restartGrowing();
  
  void regenerate();
  
  void saveSeeds();
  void loadSeeds();
  bool areTreesInFullMaturation();
  float averageMaturationLevel();
  
  void fadeImagesQuickly();
  
  Leaf* addLeaf();

#ifndef XTREES_SCREENSAVER
  void mousePressed(ofMouseEventArgs& args);
	void mouseMoved(ofMouseEventArgs& args);
	void mouseDragged(ofMouseEventArgs& args);
	void keyPressed(ofKeyEventArgs& args);
#endif
  // TODO private
  bool addTree(int x_, int y_, std::string keyword_ = "nature", float direction_ = -PI/2.F);
  
  bool visible;
private:


  std::vector<XTree*> m_trees;
  bool m_simulationRunning;
  std::tr1::shared_ptr<TwitterLayer> m_twitterLayer;
};

#endif

