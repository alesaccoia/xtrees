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


#include "KplHttpRequest.h"


KplHttpRequest::KplHttpRequest() {

}

KplHttpRequest::~KplHttpRequest() {

}

bool KplHttpRequest::getUrlSync(std::string url_, char** data, int& nbytes) {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  NSString *url = [[NSString stringWithUTF8String:url_.c_str()] stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
  NSData *rawdata = [NSData dataWithContentsOfURL:[NSURL URLWithString:url]];
  if(rawdata == nil || [rawdata length] == 0) {
    return false;
  }
  nbytes = [rawdata length];
  *data = (char*)malloc(nbytes * sizeof(char));
  memcpy((void*)*data, (const void*)[rawdata bytes], nbytes);
 [pool drain];
 return true;
}