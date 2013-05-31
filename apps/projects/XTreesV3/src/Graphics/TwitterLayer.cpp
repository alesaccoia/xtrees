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

#include "TwitterLayer.h"
#include "XTree.h"
#include <functional>

TwitterLayer::TwitterLayer() { 
  
}


TwitterLayer::~TwitterLayer() {
}

void TwitterLayer::addBaloon(XTree* tree_, std::tr1::shared_ptr<MessageEvent> args, ofVec2f* position_, float rotation) {
  TwitterBaloon* p = new TwitterBaloon(tree_, 
    args->message,
    args->user,
    args->time,
    args->image,
    position_,
    rotation,
    args->src);
  m_boxes.push_back(p);
  
  // here the life of the MessageEvent finally ends. Mind that 
  // the args->image memory will remain around, so it's the TwitterBaloon that needs to free it
}

//--------------------------------------------------------------
void TwitterLayer::update(){
  for(int i = 0; i < m_boxes.size(); ++i)
		m_boxes[i]->update();

  for(int i = 0; i < m_boxes.size(); ++i)
		if (m_boxes[i]->isDead()) {
     delete m_boxes[i];
     m_boxes[i] = NULL;
    }
  m_boxes.erase(std::remove_if(m_boxes.begin(), m_boxes.end(), std::bind2nd(std::equal_to<TwitterBaloon*>(), NULL)), m_boxes.end());
}

//--------------------------------------------------------------
void TwitterLayer::draw(){
  for(int i = 0; i < m_boxes.size(); ++i) {
		m_boxes[i]->draw();
	}
}

void TwitterLayer::stop() {
  for(int i = 0; i < m_boxes.size(); ++i)
    delete m_boxes[i];
  m_boxes.clear();
}



