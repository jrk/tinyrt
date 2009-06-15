//=====================================================================================================================
//
//   BoundingBoxViewpointGenerator.h
//
//   Part of the TinyRT Raytracing Library.
//   Author: Joshua Barczak
//
//   Copyright 2008 Joshua Barczak.  All rights reserved.
//   See  Doc/LICENSE.txt for terms and conditions.
//
//=====================================================================================================================

#ifndef _TRT_BOUNDINGBOXVIEWPOINTGENERATOR_H_
#define _TRT_BOUNDINGBOXVIEWPOINTGENERATOR_H_

#include "ViewpointGenerator.h"

//=====================================================================================================================
/// \brief Generates a series of random viewpoints within the a bounding box
//=====================================================================================================================
class BoundingBoxViewpointGenerator : public ViewpointGenerator
{
public:

    BoundingBoxViewpointGenerator( const TinyRT::AxisAlignedBox& rBox, TinyRT::uint32 nViews );

    virtual void Reset() { m_nViewsLeft = m_nViews; };
    
    virtual uint32 GetViewpointCount() const { return m_nViewsLeft; };

    virtual bool GetViewpoint( TinyRT::Vec3f* pPosition, TinyRT::Vec3f* pLookAt, float* pFOV )  ;

private:

    TinyRT::AxisAlignedBox m_box;
    uint32 m_nViewsLeft;
    uint32 m_nViews;
};


#endif // _TRT_BOUNDINGBOXVIEWPOINTGENERATOR_H_
