//
//  MyTextFieldFormatter.m
//  XTreesV3_SS
//
//  Created by Alessandro Saccoia on 3/9/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import "MyTextFieldFormatter.h"



@implementation MyTextFieldFormatter

- (id)init {
  [super init];
  maxLength = 20;
  return self;
}

- (void)setMaximumLength:(int)len {
  maxLength = len;
}

- (int)maximumLength {
  return maxLength;
}

- (NSString *)stringForObjectValue:(id)object {
  return (NSString *)object;
}

- (BOOL)getObjectValue:(id *)object forString:(NSString *)string errorDescription:(NSString **)error {
  *object = string;
  return YES;
}

- (BOOL)isPartialStringValid:(NSString **)partialStringPtr
   proposedSelectedRange:(NSRangePointer)proposedSelRangePtr
          originalString:(NSString *)origString
   originalSelectedRange:(NSRange)origSelRange
        errorDescription:(NSString **)error {
    if ([*partialStringPtr length] > maxLength) {
        return NO;
    }
    return YES;
}

- (NSAttributedString *)attributedStringForObjectValue:(id)anObject withDefaultAttributes:(NSDictionary *)attributes {
  return nil;
}

@end