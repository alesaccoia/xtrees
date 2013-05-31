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


/*
 *
 *  Implements an asynchronous trigger, twitter, sms, or database.
 *  One of the decisions to be made is the strategy to follow when there's not enough to grow.
 *  There's a repository of old messages, or when it begins we store a padding of the latest x messages
 *  from the server or from the database. This can be policy-driven
 *
 *
 */

#ifndef MESSAGETRIGGER_HPP_
#define MESSAGETRIGGER_HPP_

#include <string>
#include <list>
#include <tr1/memory>

#include "ofMain.h"
#include "ofxTimer.h"

// TODO a shared pointer technique with reference counting,
// this is a lot of copying and the stuff could stay around.

enum MessageSource {
  SRC_TWITTER,
  SRC_TWILIO,
  SRC_DATABASE
};

struct MessageEvent {
  std::tr1::shared_ptr<ofPixels> image;
  std::string user;
  std::string message;
  std::string date; //unused
  std::string time;
  bool consumed;
  MessageSource src;
};

class XTree;

class MessageTrigger :
  public ofThread {
public:
  MessageTrigger(XTree* tree, std::string keyword_, int  capacity = 100);
  
  virtual ~MessageTrigger();
  
  virtual void start();  
  virtual void stop();  
  virtual void pause(); 
  virtual void restart();
  
  // done from the external thread
  void cleanup();
  void clearMessages();
  
  virtual void update();
  virtual void fetch() = 0;
  virtual float timeBetweenQueries() = 0;

  
  virtual void setKeyword(std::string key_);
  
  std::string getKeyword() const {
    return m_keyword;
  }

  ofEvent<std::tr1::shared_ptr<MessageEvent> > newMessageEvent;
  void threadedFunction();
  
  bool providesImages;  
  
protected:
  std::string m_keyword;
  bool m_running;
  bool m_paused;
  bool m_needsReset;
  int m_capacity;
public:
  std::list<std::tr1::shared_ptr<MessageEvent> > m_msgBuffer;
protected:
  std::list<std::tr1::shared_ptr<MessageEvent> >::iterator m_current;
  ofxTimer m_timer;
  XTree* m_tree;
  static std::vector<std::string> m_profanityFilter;
};

#endif // MESSAGETRIGGER_HPP_
