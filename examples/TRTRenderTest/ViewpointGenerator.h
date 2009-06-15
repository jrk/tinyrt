//=====================================================================================================================
//
//   ViewpointGenerator.h
//
//   Definition of class: ViewpointGenerator
//
//   Part of the TinyRT Raytracing Library.
//   Author: Joshua Barczak
//
//   Copyright 2008 Joshua Barczak.  All rights reserved.
//   See  Doc/LICENSE.txt for terms and conditions.
//
//=====================================================================================================================

#ifndef _TRT_VIEWPOINTGENERATOR_H_
#define _TRT_VIEWPOINTGENERATOR_H_

#include "TinyRT.h"
using namespace TinyRT;


//=====================================================================================================================
/// \ingroup TinyRTTest
/// \brief A base class for an object which enumerates a series of viewpoints
//=====================================================================================================================
class ViewpointGenerator
{
public:

    /// Resets the viewpoints
    virtual void Reset() = 0;

    /// Returns the number of viewpoints remaining to be generated
    virtual uint32 GetViewpointCount() const = 0;

    /// Retrieves the next viewpoint
    /// \param pPosition    Receives the camera position
    /// \param pLookAt      Receives the camera's "look-at" point
    /// \param pFOV         Receives the camera field of view, in degrees
    /// \Return True if a viewpoint was created.  False if there are no more viewpoints to render
    virtual bool GetViewpoint( TinyRT::Vec3f* pPosition, TinyRT::Vec3f* pLookAt, float* pFOV ) = 0;
};


#endif // _TRT_VIEWPOINTGENERATOR_H_
