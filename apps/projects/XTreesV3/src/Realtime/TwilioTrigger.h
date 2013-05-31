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


#ifndef __TWILIOTRIGGER_H__
#define __TWILIOTRIGGER_H__

#include "MessageTrigger.h"
#include <string>
#include <sstream>
#include <list>

#include "KplHttpRequest.h"
#include "tinyxml.h"

class TwilioTrigger :
  public MessageTrigger {  
public:
  TwilioTrigger(XTree* tree, std::string keyword_, int capacity = 100);
  ~TwilioTrigger();

  void fetch();
  
  float timeBetweenQueries() {
    return 0;
  }

  void setKeyword(std::string key_) {
    m_keyword = key_;
    m_msgBuffer.clear();
    m_requestUrl = "http://www.keplero.com/twilio/selectDeleteTwilio.php?q=" + m_keyword;
  }
private: 

  void retrieveMessages();

  KplHttpRequest m_http; 
  string m_requestUrl;
  unsigned long long m_lastId;
  static int s_instances;
  unsigned int counter;
};


#endif // __TWILIOTRIGGER_H__
