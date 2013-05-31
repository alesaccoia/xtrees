/*
 *  KplHttpRequest.h
 *  ofxScreensaverNet
 *
 *  Created by Alessandro Saccoia on 2/7/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __KPLHTTPREQUEST_H__
#define __KPLHTTPREQUEST_H__

#include "ofEvents.h"

struct kplHttpRequestArgs {
  int status;
  std::string response;
};

class KplHttpRequest {
public:
	KplHttpRequest();
	~KplHttpRequest();

  // don't useme
  //void getUrl(std::string url);
  
  bool getUrlSync(std::string url_, char** data, int& nbytes);
  
  ofEvent<kplHttpRequestArgs> requestDone;

protected:
#if 0
  bool ReadStream(CFReadStreamRef stream, CFMutableDataRef data);
#endif
  std::string m_url;
  
};

#endif // __KPLHTTPREQUEST_H__
