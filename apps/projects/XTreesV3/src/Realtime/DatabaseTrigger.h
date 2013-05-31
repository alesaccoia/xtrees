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

#ifndef __DATABASETRIGGER_H__
#define __DATABASETRIGGER_H__

#include "MessageTrigger.h"
#include <string>
#include <sstream>
#include <list>

class DatabaseTrigger :
  public MessageTrigger {  
public:
  DatabaseTrigger(XTree* tree, std::string keyword_, int capacity = 100);
  ~DatabaseTrigger();

  void fetch();
  
  float timeBetweenQueries() {
    return 0;
  }

  void setKeyword(std::string key_);
  
private: 

  std::vector <std::string> m_messages;
  std::vector <std::string> m_users;
  unsigned int m_size;
};


#endif // __DATABASETRIGGER_H__
