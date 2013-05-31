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


#include "MessageTrigger.h"
#include "KepleroUtils.h"
#include "Settings.h"


#define MAX_QUEUE_SIZE 100

// this is needed because on 64 bit we need to register the thread
#ifdef XTREES_SCREENSAVER
extern "C" {
void objc_registerThreadWithCollector();
}
#endif


MessageTrigger::MessageTrigger(XTree* tree, std::string keyword_, int capacity_) :
  m_tree(tree),
  m_keyword(keyword_),
  m_running(true),
  m_paused(false),
  m_needsReset(false),
  m_capacity(capacity_) {
  m_current = m_msgBuffer.begin();
//  std::cout << this << " created" << std::endl;
}

MessageTrigger::~MessageTrigger() {
  stop();
  ofThread::waitForThread(true);
}

void MessageTrigger::start() {
//  std::cout << this << " activate" << std::endl;
  m_running = true;
  m_paused = false;
  m_timer.setAlarm(10);
  ofThread::startThread(false, false);   // non blocking (uses trylock)
}


void MessageTrigger::stop() {
  if (m_running) {
    m_running = false;
    ofThread::stopThread();
  }
}

void MessageTrigger::pause() {
  m_paused = true;
}

void MessageTrigger::clearMessages() {
  m_msgBuffer.clear();
  m_current = m_msgBuffer.begin();
//  std::cout << this << " preactivate" << std::endl;
}

void MessageTrigger::restart() {
  m_paused = false;
//  std::cout << this << " preactivate" << std::endl;
}

// TODO lots of string copying and operations (it's on the main thread)
// MOVE THIS PROCESSING TO THE THREADEDFUNCTION
//https://dev.twitter.com/docs/error-codes-responses


void MessageTrigger::threadedFunction() {

#ifdef XTREES_SCREENSAVER
  if (m_lastId == 0) {
#if MAC_OS_X_VERSION_MAX_ALLOWED <= MAC_OS_X_VERSION_10_5
            // On Leopard and earlier there is no way objc_registerThreadWithCollector
            // function, so we do it manually.
            auto_zone_register_thread(auto_zone());
#else
            // On SnowLeopard and later we just call the thread registration function.
            objc_registerThreadWithCollector();
#endif
  }
#endif
  g_activeThreadsMutex.lock();
  ++g_activeThreads;
  g_activeThreadsMutex.unlock();
//  std::cout << this << " threadedFunction has entered" << std::endl; 
  while(isThreadRunning()){
    if(!m_paused){ 
      if (!m_msgBuffer.size()) {
        fetch();   // to be implemented by subclasses
      }
      cleanup();
//      float sleepingTime = timeBetweenQueries() / 20;
      for (int i = 0; i < 20 && m_running; ++i) {
        ofSleepMillis(30);
      }    
    } else {
      ofSleepMillis(200);
    }
  }
  m_msgBuffer.clear();
  m_current = m_msgBuffer.begin();  
  
  g_activeThreadsMutex.lock();
  --g_activeThreads;
  g_activeThreadsMutex.unlock();
//  std::cout << this << " threadedFunction has exited" << std::endl; 
}

// called from the local thread, blocks on the mutex of this trigger
void MessageTrigger::cleanup() {
  mutex.lock();
  if (m_msgBuffer.size() > m_capacity) {
    int i = m_msgBuffer.size() - m_capacity;
    std::list<std::tr1::shared_ptr<MessageEvent> >::iterator ite = m_msgBuffer.end();
    --ite;
    if (m_current == ite) {
      for (int j = i; j > 0 && m_current != m_msgBuffer.begin(); --j) {
        --m_current;
      }
    }
    while (m_current != ite && i-- > 0) {
      --ite;
      m_msgBuffer.pop_back();
    }
  }
  mutex.unlock();
}

void MessageTrigger::update() {
  // keyword has changed... wait to reset without blocking the thread
  // and do not produce anything until has been reset
  bool emitNewMessage = m_timer.alarm();
  
  /* once the alarm has been triggered we basically poll the mutex waiting the 
    other thread to sleep when the http request is over */
  
  if (emitNewMessage) {
    //std::cout << "size: " << m_msgBuffer.size() << std::endl;
    if (!m_paused) {
      if (mutex.tryLock()) {
        if (!m_msgBuffer.empty()) {      
          // TODO priority management now        
          // now the ownership is to the receiving thread :)
          // actually bullshit, but we are always on the main thread.
          // this just works on this assumption
          std::tr1::shared_ptr<MessageEvent> argument = *m_current;
          ofNotifyEvent(newMessageEvent, argument, this);
          m_current = m_msgBuffer.erase(m_current);
          if (!(m_current == m_msgBuffer.begin()))
            --m_current;  
        }
        mutex.unlock();        
      }
    }
    m_timer.set();
    m_timer.setAlarm(ofRandom(g_minFrequency, g_maxFrequency));
  }
}

void MessageTrigger::setKeyword (std::string key_) {    
  m_keyword = kplToUrlEncoding(key_);
  m_msgBuffer.clear();
}


