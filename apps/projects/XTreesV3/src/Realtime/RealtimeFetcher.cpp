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


#include "RealtimeFetcher.h"

RealtimeFetcher::RealtimeFetcher() :
  m_running(false),
  m_paused(false) {
}

void RealtimeFetcher::start() {
  ofThread::startThread(false, false);
  m_paused = false;
  m_running = true;
}

void RealtimeFetcher::reset() {  
  triggers.clear();
}

void RealtimeFetcher::stop() {
  m_running = false;
  ofThread::waitForThread(true);
  for (int i = 0; i < triggers.size(); ++i ) {
    triggers[i]->clearMessages();
  }
}

void RealtimeFetcher::pause() {
  m_paused = true;
}

void RealtimeFetcher::restart() {
  m_paused = false;
}

void RealtimeFetcher::threadedFunction() {
  while(isThreadRunning()){
    if (!m_paused) {
      for (int i = 0; i < triggers.size(); ++i ) {
        triggers[i]->fetch();
        triggers[i]->cleanup();
        if (!m_running) {
          goto exitNow;
        }
      }
    }
    ofSleepMillis(200);
  }
exitNow:
  return;
}

void RealtimeFetcher::update() {
  if (!m_paused) {
    for (int i = 0; i < triggers.size(); ++i ) {
      triggers[i]->update();
    }
  }
}
