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
 *  TwitterTrigger.h
 *  TwitterRetriever
 *
 *  Created by Alessandro Saccoia on 1/18/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 *  Give the option of overriding or not the minimum time leaving the list to go to infinity... risky
 *  In this case we suppose that someone is monitoring the queue and if it gets too long can cut it!
 *
 *  If the minimum time is much less than the filling rate we'll end up out of memory or anyway it's a disaster
 *
 *  With the technique skipIfFull some messages will be removed to try to show the messages always within the specified time limits
 *  With the technique neverSkip we risk out of memory but every message will be displayed
 *  With the technique overrideMinimum we go much faster when there are many messages, otherwise we stay at a normal rate 
 */


#ifndef TwitterTrigger_H_
#define TwitterTrigger_H_

#include "MessageTrigger.h"
#include <string>
#include <sstream>
#include <list>

#include "KplHttpRequest.h"
#include "tinyxml.h"
#include "Settings.h"

class TwitterTrigger :
  public MessageTrigger {  
public:
  TwitterTrigger(XTree* tree, std::string keyword_, int capacity = 100);
  ~TwitterTrigger();

  void fetch();
  
  float timeBetweenQueries() {
    if(g_timedExhibit) {
      return ofClamp(1, g_showDuration, 60) * 60.F * 1000.F * s_instances / 150.F;
    } else {
      return 60.F * 60.F * 1000.F * s_instances / 150.F;
    }
  }
  void setKeyword(std::string key_) {
    MessageTrigger::setKeyword(key_);
    if (m_lastId == 0) {
      m_requestUrl = "http://search.twitter.com/search.atom?q=" + m_keyword +"&since_id=" + ofToString(m_lastId) + "&rpp=" + ofToString(g_pastTweets);
    } else {
      m_requestUrl = "http://search.twitter.com/search.atom?q=" + m_keyword +"&since_id=" + ofToString(m_lastId) + "&rpp=100";
    }
  }
private: 

  void retrieveMessages();

  void stop();
  
  // <id>tag:search.twitter.com,2005:159700307838894080</id>
  unsigned long long extractId(std::string fieldValue_);
  std::string extractTime(std::string fieldValue_);

  KplHttpRequest m_http;
  string m_requestUrl;
  unsigned long long m_lastId;
  static int s_instances;
};


#endif
