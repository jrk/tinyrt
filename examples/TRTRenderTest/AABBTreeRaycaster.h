//=====================================================================================================================
//
//   AABBTreeRaycaster.h
//
//   Part of the TinyRT Raytracing Library.
//   Author: Joshua Barczak
//
//   Copyright 2008 Joshua Barczak.  All rights reserved.
//   See  Doc/LICENSE.txt for terms and conditions.
//
//=====================================================================================================================

#ifndef _TRT_AABBTREERAYCASTER_H_
#define _TRT_AABBTREERAYCASTER_H_

#include "TinyRT.h"
#include "TestRaycaster.h"


//=====================================================================================================================
/// \brief AABB tree raycaster
//=====================================================================================================================
class AABBTreeRaycaster : public TestRaycaster
{
public:

    
    AABBTreeRaycaster( TestMesh* pMesh, AABBTree<TestMesh>* pBVH );

    ~AABBTreeRaycaster( );

    virtual void RaycastFirstHit( TinyRT::Ray& rRay, TinyRT::TriangleRayHit& rHitInfo ) ;

private:

    AABBTree<TestMesh>* m_pBVH;
};


#endif // _TRT_AABBTREERAYCASTER_H_
