//=====================================================================================================================
//
//   TestUtils.h
//
//   Part of the TinyRT Raytracing Library.
//   Author: Joshua Barczak
//
//   Copyright 2008 Joshua Barczak.  All rights reserved.
//   See  Doc/LICENSE.txt for terms and conditions.
//
//=====================================================================================================================

#ifndef _TEST_UTILS_H_
#define _TEST_UTILS_H_

#include <stdlib.h>


inline float RandomFloat() { return (float) rand() / (float) RAND_MAX; };

inline float DegreeToRad( float fD ) { return fD * 3.1415629f / 180.0f; };

#endif
