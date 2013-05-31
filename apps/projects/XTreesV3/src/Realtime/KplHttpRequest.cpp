/*
 *  KplHttpRequest.cpp
 *  ofxScreensaverNet
 *
 *  Created by Alessandro Saccoia on 2/7/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "KplHttpRequest.h"
#include "ofMain.h"


KplHttpRequest::KplHttpRequest() {

}

KplHttpRequest::~KplHttpRequest() {

}

#ifndef XTREES_IOS
bool KplHttpRequest::getUrlSync(std::string url_, char** data, int& nbytes) { 
  m_url = url_;
  bool toReturn = true;
  CFStringRef bodyString = CFSTR(""); // Usually used for POST data
  CFDataRef bodyData = CFStringCreateExternalRepresentation(kCFAllocatorDefault,
                                          bodyString, kCFStringEncodingUTF8, 0);
   
  CFStringRef url = CFStringCreateWithCString(kCFAllocatorDefault, m_url.c_str(), kCFStringEncodingUTF8);
  CFURLRef myURL = CFURLCreateWithString(kCFAllocatorDefault, url, NULL);
   
  CFStringRef requestMethod = CFSTR("GET");
  CFHTTPMessageRef myRequest =
      CFHTTPMessageCreateRequest(kCFAllocatorDefault, requestMethod, myURL,
                                 kCFHTTPVersion1_1);
   
  //CFDataRef bodyDataExt = CFStringCreateExternalRepresentation(kCFAllocatorDefault, bodyData, kCFStringEncodingUTF8, 0);
  CFHTTPMessageSetBody(myRequest, bodyData);

  
  CFReadStreamRef myReadStream = CFReadStreamCreateForHTTPRequest(kCFAllocatorDefault, myRequest);
 
  CFReadStreamOpen(myReadStream);  
  
  CFMutableDataRef body_data = CFDataCreateMutable(kCFAllocatorDefault, 0);
  
  CFHTTPMessageRef response;
  
  CFIndex status;
  
  if (!ReadStream(myReadStream, body_data)) {
    std::cout << "Error while reading stream" << endl;
    return false;
  }

  response = (CFHTTPMessageRef)CFReadStreamCopyProperty(myReadStream, kCFStreamPropertyHTTPResponseHeader);

  if (response == NULL) {
    std::cout << "Error obtaining HTTP response" << endl;
    toReturn = false;
    goto cleanup;
  }
  
  if (!CFHTTPMessageIsHeaderComplete(response)) {
    std::cout << "HTTP Header incomplete" << endl;
    toReturn = false;
    goto cleanup;
  }

  status = CFHTTPMessageGetResponseStatusCode(response);
  if (status != 200) {
    std::cout << "Unexpected HTTP response status: " << status << endl;
    toReturn = false;
    goto cleanup;
  }

  nbytes = CFDataGetLength(body_data);
  
  *data = new char[nbytes];
  memcpy((void*)*data, (const void*)CFDataGetBytePtr(body_data), nbytes);
cleanup:
  CFRelease(body_data);
  CFRelease(response);
  CFRelease(myReadStream);
  CFRelease(myRequest);
  CFRelease(myURL);
  CFRelease(url);
  CFRelease(bodyData);
  
  return toReturn;

}

bool KplHttpRequest::ReadStream(CFReadStreamRef stream, CFMutableDataRef data) {
  do {
    unsigned char buf[1024];
    CFStreamStatus stream_status = CFReadStreamGetStatus(stream);
    if (stream_status == kCFStreamStatusAtEnd) {
      return true;
    }
    if (stream_status != kCFStreamStatusOpen) {
      cerr << "Stream status: " << stream_status;
      return false;
    }
    CFIndex bytes_read = CFReadStreamRead(stream, buf, 1024);
    if (bytes_read < 0) {
      // TODO(allen): CFReadStremGetError to get error detail
      cerr << "Unable to read from stream";
      return false;
    } else if (bytes_read == 0) {
      return true;
    }
    CFDataAppendBytes(data, buf, bytes_read);
  } while (true);
}
#else

bool KplHttpRequest::getUrlSync(std::string url_, char** data, int& nbytes) { 
  return true;
}

#endif

