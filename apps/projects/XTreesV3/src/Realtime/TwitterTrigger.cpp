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
 *  TwitterTrigger.cpp
 *  TwitterRetriever
 *
 *  Created by Alessandro Saccoia on 1/18/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 *  Algorithm:
 *  This class needs to be a thread because every call to getUrl is blocking
 *  even if it's callback based.
 *  When started, the threadedFunction does the call to getUrl. Note that the
 *  httpResponse method will be called from the same thread. The locking mechanism
 *  will then protect the list and not block the update callback that is called
 *  from the main thread and serves to consume the messages (timer based)
 *
 *
 */
 
 // TODO: do we need a watchdog to see if internet is down?
 // does m_http raise an exception i internet is broken?
 // TODO: check for bad values especially in ids

#include "TwitterTrigger.h"
#include <limits>
#include "KepleroUtils.h"
#include "XTree.h"

int TwitterTrigger::s_instances;


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
//----------------------------------------

TwitterTrigger::TwitterTrigger(XTree* tree, std::string keyword_, int capacity) :
  MessageTrigger(tree, keyword_, capacity),
  m_lastId(0) {
  setKeyword(keyword_);
  s_instances++;
}

TwitterTrigger::~TwitterTrigger() {
  //if (m_running) {
    //ofThread::stopThread();
  //}
}

void TwitterTrigger::stop() {
  MessageTrigger::stop();
  m_lastId = 0;
}



// TODO lots of string copying and operations (it's on the main thread)
// MOVE THIS PROCESSING TO THE THREADEDFUNCTION
//https://dev.twitter.com/docs/error-codes-responses

void TwitterTrigger::fetch() {
  if (m_lastId == 0) {
    m_requestUrl = "http://search.twitter.com/search.atom?q=" + m_keyword +"&since_id=" + ofToString(m_lastId) + "&rpp=" + ofToString(g_pastTweets);
  } else {
    m_requestUrl = "http://search.twitter.com/search.atom?q=" + m_keyword +"&since_id=" + ofToString(m_lastId) + "&rpp=100";
  }
  char *data;
  int nBytes;
  if (!m_http.getUrlSync(m_requestUrl, &data, nBytes)) {
    m_tree->realtimeStarving(true);
    return;
  }  
  TiXmlDocument xmlDoc;
  xmlDoc.Parse(data);
  delete data;
  if (xmlDoc.Error()) {
    ofLogError("Error in reading the xml file " + m_keyword);
    m_tree->realtimeStarving(true);
    return;
  }
  
  
  TiXmlNode * feedNode = xmlDoc.FirstChild("feed");
  if (feedNode == NULL) {
    ofLogError("Error in reading the xml node feed " + m_keyword);
    m_tree->realtimeStarving(true);
    return;
  }
  ofLog(OF_LOG_WARNING, "Tree %s is fetching realtime messages", m_keyword.c_str());
  unsigned long long id;
  std::tr1::shared_ptr<MessageEvent> theEvent;
  bool loadedOk = true;   
  
  if (!feedNode->ChildElementCount("entry")) {
    m_tree->realtimeStarving(true);
    return;
  }
  
  TiXmlNode* entryNode = feedNode->FirstChild("entry");  
  
  for( ;
			 entryNode && m_running;
			 entryNode = entryNode->NextSibling("entry"))
  {
    loadedOk = true;
    m_tree->realtimeStarving(false);
    try {
      id = extractId(entryNode->FirstChildElement("id")->GetText());
      if (strlen(entryNode->FirstChildElement("title")->GetText()) < 5) {
        throw -1;
      }
      theEvent.reset(new MessageEvent());
      theEvent->message = entryNode->FirstChildElement("title")->GetText();
      theEvent->user = entryNode->FirstChildElement("author")->FirstChildElement("name")->GetText();
      theEvent->time = extractTime(entryNode->FirstChildElement("published")->GetText());
      theEvent->consumed = false;
      theEvent->src = SRC_TWITTER;
      
      loadedOk &= theEvent->message.length() > 0;
      loadedOk &= theEvent->user.length() > 0;
      loadedOk &= theEvent->time.length() > 0;
      
      // remember that we can't acceess any opengl stuff from this thread, so a call to
//      theEvent->image = new ofImage("http://a1.twimg.com/profile_images/1758468996/tumblr_lnc0j7gj9P1qbgu2qo1_500_large_normal.png");
      // will crash. just load the raw Pixels from the url then.

      
      //http://a3.twimg.com/profile_images/1643824843/428741671_reasonably_small_normal.gif
      // these guys with animated gifs make this code crash:
      // they deserve the "Black Avatar" 
      
      std::string image_url( entryNode->LastChild("link")->ToElement()->Attribute("href"));
      
      if (!std::string::npos == image_url.find("normal")) {
        std::cout << "big image!";
      }
      
      if (g_showThumbs) {
        ofBuffer bData;
        char* data;
        int nBytes;
        if(m_http.getUrlSync(entryNode->LastChild("link")->ToElement()->Attribute("href"), &data, nBytes)) {
          bData.set(data, nBytes); //TODO remove uneseful copy
          delete data;
          theEvent->image.reset(new ofPixels);
          loadedOk &= ofLoadImage(*(theEvent->image.get()), bData);
        } else {
          loadedOk = false;
        }

      }
      if (id > m_lastId) {
        m_lastId = id;
      }    

      }

    catch (...) {
      loadedOk = false;

    }
    
    if (loadedOk && m_running) {
      // INSERT AT THE FRONT OF THE QUEUE         
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
        
    }

  }
}

unsigned long long TwitterTrigger::extractId(std::string fieldValue_) {
  std::vector<std::string> tokens = tokenize(fieldValue_,":");
  stringstream m_idReader(tokens[2]);
  unsigned long long id;
  m_idReader >> id;
  return id;
}

//<published>2012-01-18T18:14:55Z</published>
std::string TwitterTrigger::extractTime(std::string fieldValue_) {
  size_t posT, posZ;
  posT = fieldValue_.find("T");
  posZ = fieldValue_.find("Z");
  return fieldValue_.substr(posT + 1, posZ - posT - 1);
}



