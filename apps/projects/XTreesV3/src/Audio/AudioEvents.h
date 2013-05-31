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


#ifndef AUDIOEVENTS_H_
#define AUDIOEVENTS_H_

enum AudioDestination {
  kStart,
  kSimulOn,
  kSimulOff,
  kMsgOn,
  kMsgOff,
  kLeavesOn,
  kLeavesOff,
  kNewLine,
  kLinesOn,
  kLinesOff,
  kBreath,
  kReadyToRegenerate,
  kRegen,
  kStop,
  kNewBranch,
  kNewLeaf,
  kActivateBirds,
  kActivateVinyl,
  kActivateDrone,
  kActivateRand1,
  kPlaceSeed,
  kEraseSeed,
  kBranchPolyphony,
  kEq100,
  kEq250,
  kEq700,
  kEq1500,
  kEq3000,
  kEq7000,
  kEq15000,
  kEqGain
};




#endif // AUDIOEVENTS_H_
