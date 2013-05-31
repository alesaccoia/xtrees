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


#include "TwilioTrigger.h"

#include <limits>
#include "KepleroUtils.h"



TwilioTrigger::TwilioTrigger(XTree* tree, std::string keyword_, int capacity) :
  MessageTrigger(tree, keyword_, capacity),
  m_lastId(0) {
  m_keyword = keyword_;
  m_msgBuffer.clear();
}

TwilioTrigger::~TwilioTrigger() {
  //if (m_running) {
  //  ofThread::waitForThread();
  //}
}


void TwilioTrigger::fetch() {
  if (!(counter++ % 30)) {
    return;
  }
  char *data;
  int nBytes;
  if (!m_http.getUrlSync(m_requestUrl, &data, nBytes)) {
    return;
  }
  TiXmlDocument xmlDoc;
  xmlDoc.Parse(data);
  delete data;
  if (xmlDoc.Error()) {
    ofLogError("Error in reading the xml file " + m_keyword);
    return;
  }
  
  TiXmlNode* entryNode;
  TiXmlNode * feedNode = xmlDoc.FirstChild("messages");
  if (feedNode == NULL) {
    ofLogError("Error in reading the xml node feed " + m_keyword);
    return;
  }
  unsigned long long id;
  bool loadedOk = true;   
  for( entryNode = feedNode->FirstChild("message");
			 entryNode;
			 entryNode = entryNode->NextSibling("message"))
  {
    loadedOk = true;
    std::tr1::shared_ptr<MessageEvent> theEvent;
    try {
      theEvent.reset(new MessageEvent());
      theEvent->message = entryNode->FirstChildElement("msg_data")->GetText();
      theEvent->user = "SMS";
      //theEvent->time = extractTime(entryNode->FirstChildElement("published")->GetText());
      theEvent->consumed = false;
      theEvent->src = SRC_TWILIO;
      loadedOk &= theEvent->message.length() > 0;
      loadedOk &= theEvent->user.length() > 0;
      //loadedOk &= theEvent->time.length() > 0;


    } catch (...) {
      loadedOk = false;

    }
    
    if (loadedOk) {
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
  ofSleepMillis(20);
}
