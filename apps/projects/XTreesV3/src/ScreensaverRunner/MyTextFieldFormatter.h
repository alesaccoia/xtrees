//
//  MyTextFieldFormatter.h
//  XTreesV3_SS
//
//  Created by Alessandro Saccoia on 3/9/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface MyTextFieldFormatter : NSFormatter {
  int maxLength;
}
- (void)setMaximumLength:(int)len;
- (int)maximumLength;

@end