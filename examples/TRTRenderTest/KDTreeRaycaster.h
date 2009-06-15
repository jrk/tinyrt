//=====================================================================================================================
//
//   KDTreeRaycaster..h
//
//   Part of the TinyRT raytracing library.
//   Author: Joshua Barczak
//   Copyright 2009 Joshua Barczak.  All rights reserved.
//
//   See  Doc/LICENSE.txt for terms and conditions.
//
//=====================================================================================================================

#ifndef _KDTREERAYCASTER_H_
#define _KDTREERAYCASTER_H_

#include "TinyRT.h"
#include "TestRaycaster.h"
using namespace TinyRT;

//=====================================================================================================================
/// \brief KD tree raycaster
//=====================================================================================================================
class KDTreeRaycaster : public TestRaycaster
{
public:
    KDTreeRaycaster( TestMesh* pMesh, KDTree<TestMesh>* pTree );

    ~KDTreeRaycaster( );

    virtual void RaycastFirstHit( TinyRT::Ray& rRay, TinyRT::TriangleRayHit& rHitInfo ) ;

private:

    KDTree<TestMesh>* m_pTree;
};


#endif // _KDTREERAYCASTER_H_
