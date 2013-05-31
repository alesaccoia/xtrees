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

 
#ifndef TWITTERLAYER_H_
#define TWITTERLAYER_H_


#include "TwitterBaloon.h"
#include <tr1/memory>

#ifndef XTREES_IOS
#include "ofxFTGLFont.h"
#endif
#include "MessageTrigger.h"
class XTree;

class TwitterLayer {
public:
  TwitterLayer();
  ~TwitterLayer();
  void update();
  void draw();
  void addBaloon(XTree* tree_, std::tr1::shared_ptr<MessageEvent> args, ofVec2f* position_, float rotation_);
  void stop();
  inline int numberOfBaloons() {
    return m_boxes.size();
  }
private:
  std::vector	<TwitterBaloon*> m_boxes;
};



#endif
