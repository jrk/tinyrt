//=====================================================================================================================
//
//   QBVHRaycaster.h
//
//   Part of the TinyRT Raytracing Library.
//   Author: Joshua Barczak
//
//   Copyright 2008 Joshua Barczak.  All rights reserved.
//   See  Doc/LICENSE.txt for terms and conditions.
//
//=====================================================================================================================

#ifndef _TRT_HRAYCASTER_H_
#define _TRT_HRAYCASTER_H_

#include "TestRaycaster.h"


//=====================================================================================================================
/// \brief QBVH raycaster
//=====================================================================================================================
class QBVHRaycaster : public TestRaycaster
{
public:

    QBVHRaycaster( TestMesh* pMesh, QuadAABBTree<TestMesh>* pBVH );

    ~QBVHRaycaster( );

    virtual void RaycastFirstHit( TinyRT::Ray& rRay, TinyRT::TriangleRayHit& rHitInfo ) ;


private:
    QuadAABBTree< TestMesh >* m_pTree;
};



#endif // _TRT_HRAYCASTER_H_
