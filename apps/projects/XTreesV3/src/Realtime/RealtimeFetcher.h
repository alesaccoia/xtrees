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


#ifndef RealtimeFetcher_h_
#define RealtimeFetcher_h_

#include "ofMain.h"
#include "Singleton.hpp"
#include "MessageTrigger.h"

class RealtimeFetcher :
  public ofThread,
  public Singleton<RealtimeFetcher> {
public:  
  RealtimeFetcher();
  void start();  
  void stop();  
  void pause();
  void restart();
  void update();
  void reset();
   
  void threadedFunction();
  
  volatile bool m_running;
  bool m_paused;

  std::vector<MessageTrigger*> triggers;
};

#endif
