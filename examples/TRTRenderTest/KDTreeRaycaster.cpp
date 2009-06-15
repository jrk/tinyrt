//=====================================================================================================================
//
//   KDTreeRaycaster..cpp
//
//
//   Part of the TinyRT raytracing library.
//   Author: Joshua Barczak
//   Copyright 2009 Joshua Barczak.  All rights reserved.
//
//   See  Doc/LICENSE.txt for terms and conditions.
//
//=====================================================================================================================

#include "KDTreeRaycaster.h"



//=====================================================================================================================
//
//         Constructors/Destructors
//
//=====================================================================================================================

KDTreeRaycaster::KDTreeRaycaster( TestMesh* pMesh, KDTree<TestMesh>* pTree ) : TestRaycaster(pMesh), m_pTree(pTree)
{
#ifdef _DEBUG
    pTree->Validate();
#endif
}

KDTreeRaycaster::~KDTreeRaycaster()
{
    delete m_pTree;
}

//=====================================================================================================================
//
//            Public Methods
//
//=====================================================================================================================

void KDTreeRaycaster::RaycastFirstHit( TinyRT::Ray& rRay, TinyRT::TriangleRayHit& rHitInfo )
{
    typedef DirectMapMailbox<TestMesh::obj_id> Mailbox_T;
    static ScratchMemory scratch;
    RaycastKDTree<Mailbox_T>( m_pTree, GetMesh(), rRay, rHitInfo, m_pTree->GetRoot(), scratch );
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

