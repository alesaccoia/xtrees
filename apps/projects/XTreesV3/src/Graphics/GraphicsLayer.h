/*
 *  GraphicsLayer.h
 *  XTreesV3
 *
 *  Created by Alessandro Saccoia on 1/31/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GRAPHICSLAYER_H_
#define GRAPHICSLAYER_H_

class GraphicsLayer {
public:
  GraphicsLayer(TreesManager* trees);  
  ~GraphicsLayer();
  
  virtual void update() = 0;
  virtual void draw() = 0;
  virtual void preActivate() = 0;
  virtual void start() = 0;
  virtual void stop() = 0;
  void reset() = 0;
private:  
  bool m_active;
};

#endif //GRAPHICSLAYER_H_
