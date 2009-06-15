//=====================================================================================================================
//
//   TestRaycaster.h
//
//   Definition of class: TestRaycaster
//
//   Part of the TinyRT Raytracing Library.
//   Author: Joshua Barczak
//
//   Copyright 2008 Joshua Barczak.  All rights reserved.
//   See  Doc/LICENSE.txt for terms and conditions.
//
//=====================================================================================================================

#ifndef _TRT_TESTRAYCASTER_H_
#define _TRT_TESTRAYCASTER_H_

#include "TestMesh.h"

#include "TinyRT.h"
using namespace TinyRT;


//=====================================================================================================================
/// \ingroup TinyRTTest
/// \brief A base class for a raycaster
//=====================================================================================================================
class TestRaycaster
{
public:


    inline TestRaycaster( TestMesh* pMesh ) : m_pMesh( pMesh )
    {
        pMesh->Optimize();
    }

    virtual ~TestRaycaster() {};

    inline TestMesh* GetMesh() const { return m_pMesh; };

    virtual void RaycastFirstHit( TinyRT::Ray& rRay, TinyRT::TriangleRayHit& rHitInfo ) = 0;

   
private:
    
    TestMesh* m_pMesh;
};



#endif // _TRT_TESTRAYCASTER_H_
