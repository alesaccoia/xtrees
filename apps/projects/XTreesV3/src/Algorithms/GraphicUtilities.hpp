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


 
#ifndef XTREEPROJECT_GRAPHICSUTILITY_HPP
#define XTREEPROJECT_GRAPHICSUTILITY_HPP

#include "ofMain.h"

// todo: works just with 4 channels, that is for outr particular case
// row major order
// columnEnd is not included!
//#ifndef XTREES_IOS
//inline void CopyPixels(PixelType * dst, const PixelType * src, int columnStart, int columnEnd, int rowStart, int rowEnd, int srcColumns, int srcRows) {
//  size_t nrColumns = columnEnd - columnStart;
//  size_t amountPerRow = nrColumns * 4;
//  for (int i = rowStart; i < rowEnd; ++i)
//    memcpy((void*)(dst + i * nrColumns * 4), (void*)(src + ((i * srcColumns + columnStart) * 4)), amountPerRow);  
//}
//#endif

// can be the same image
void kplColorize(ofImage* from, ofImage* to, ofColor& color_);

void kplSetMaxAlpha(ofImage* fromTo);

float kplNormalizeAngle(float angle_);

#endif