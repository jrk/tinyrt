//=====================================================================================================================
//
//   TRTAssert.h
//
//   Definition of class: TinyRT::Assert
//
//   Part of the TinyRT Raytracing Library.
//   Author: Joshua Barczak
//
//   Copyright 2008 Joshua Barczak.  All rights reserved.
//   See  Doc/LICENSE.txt for terms and conditions.
//
//=====================================================================================================================

#ifndef _TRT_ASSERT_H_
#define _TRT_ASSERT_H_

#include <assert.h>

#ifndef TRT_ASSERT

/// Library clients may #define TRT_ASSERT to substitute their own assertion mechanism 
#define TRT_ASSERT(x) assert(x); 
//#define TRT_ASSERT(x) if(!(x)) { printf("FOO: %s : %s", __FILE__, __LINE__ ); }

#endif

#endif // _TRT_ASSERT_H_
