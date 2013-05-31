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


#include "DatabaseTrigger.h"

#include <limits>
#include "KepleroUtils.h"
#include "XTree.h"

// taken straight from the ofxXmlSettings implementation file
//----------------------------------------
// a pretty useful tokenization system:
static vector<string> tokenize(const string & str, const string & delim)
{
  vector<string> tokens;

  size_t p0 = 0, p1 = string::npos;
  while(p0 != string::npos)
  {
    p1 = str.find_first_of(delim, p0);
    if(p1 != p0)
    {
      string token = str.substr(p0, p1 - p0);
      tokens.push_back(token);
    }
    p0 = str.find_first_not_of(delim, p1);
  }
  return tokens;
}

DatabaseTrigger::DatabaseTrigger(XTree* tree, std::string keyword_, int capacity) :
  MessageTrigger(tree, keyword_, capacity),
  m_size(0) {
  setKeyword(keyword_);
}

DatabaseTrigger::~DatabaseTrigger() {
  //if (m_running) {
   // ofThread::stopThread();
  //}
}


void DatabaseTrigger::fetch() {
  if (m_size > 0 && m_tree->isRealtmeStarving()) {
    ofLog(OF_LOG_WARNING, "Offline database access for tree %s", m_keyword.c_str());
    int pos = kplToss(m_size);
    std::tr1::shared_ptr<MessageEvent> theEvent(new MessageEvent());
    theEvent->user = m_users[pos];
    theEvent->message = m_messages[pos];
    theEvent->consumed = false;
    theEvent->src = SRC_DATABASE;
    while (!lock()) {
      // trylock polling
      ofSleepMillis(10);
    }
    m_msgBuffer.push_front(theEvent);      
    if (m_current == m_msgBuffer.end()) {
      m_current = m_msgBuffer.begin();
    }
    unlock();
    ofSleepMillis(50);      
  } else {
    ofSleepMillis(1000);
  }
}


void DatabaseTrigger::setKeyword(std::string key_) {
  m_keyword = key_;
  m_size = 0;
  m_users.clear();
  m_messages.clear();
  std::string line;
  std::ifstream myfile ("../../../data/archive.txt");
  if (myfile.is_open())
  {
    while ( myfile.good() )
    {
      getline (myfile,line);
      int found = line.find(m_keyword);
      if (found!=string::npos) {
        std::vector<std::string> tokens = tokenize(line,"#");
        if (tokens.size() == 2 ) {
          m_users.push_back(tokens[0]);
          m_messages.push_back(tokens[1]);
          ++m_size;
        }
      }
    }
    myfile.close();
  } else {
    ofLog(OF_LOG_ERROR, "Tree %s cannot open offline archive", m_keyword.c_str());
  }
  
}
