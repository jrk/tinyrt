//=====================================================================================================================
//
//   BoundingBoxViewpointGenerator.cpp
//
//   Implementation of class: TinyRT::ndingBoxViewpointGenerator
//
//   Part of the TinyRT Raytracing Library.
//   Author: Joshua Barczak
//
//   Copyright 2008 Joshua Barczak.  All rights reserved.
//   See  Doc/LICENSE.txt for terms and conditions.
//
//=====================================================================================================================

#include "TinyRT.h"
#include "BoundingBoxViewpointGenerator.h"
#include "TestUtils.h"


//=====================================================================================================================
//
//         Constructors/Destructors
//
//=====================================================================================================================

//=====================================================================================================================
//=====================================================================================================================
BoundingBoxViewpointGenerator::BoundingBoxViewpointGenerator( const TinyRT::AxisAlignedBox& rBox, uint32 nViews ) 
: m_box( rBox ), m_nViews(nViews), m_nViewsLeft(nViews)
{
}

//=====================================================================================================================
//
//            Public Methods
//
//=====================================================================================================================

//=====================================================================================================================
//=====================================================================================================================
bool BoundingBoxViewpointGenerator::GetViewpoint( TinyRT::Vec3f* pPosition, TinyRT::Vec3f* pLookAt, float* pFOV ) 
{
    if( m_nViewsLeft == 0 )
        return false;

    TinyRT::Vec3f vS1 = TinyRT::Vec3f( RandomFloat(), RandomFloat(), RandomFloat() );
    TinyRT::Vec3f vS2 = TinyRT::Vec3f( RandomFloat(), RandomFloat(), RandomFloat() );

    *pPosition = m_box.Min() + (m_box.Max()-m_box.Min())*vS1;
    *pLookAt   = m_box.Min() + (m_box.Max()-m_box.Min())*vS2;
    
    *pFOV = 60.0f;

    m_nViewsLeft--;
    return true;
}

//=====================================================================================================================
//
//           Protected Methods
//
//=====================================================================================================================

//=====================================================================================================================
//
//            Private Methods
//
//=====================================================================================================================

