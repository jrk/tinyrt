
//=====================================================================================================================
//
//   AABBTreeRaycaster.cpp
//
//   Implementation of class: AABBTreeRaycaster
//
//   Part of the TinyRT Raytracing Library.
//   Author: Joshua Barczak
//
//   Copyright 2008 Joshua Barczak.  All rights reserved.
//   See  Doc/LICENSE.txt for terms and conditions.
//
//=====================================================================================================================


#include "AABBTreeRaycaster.h"



void CheckBVH( std::vector<uint32>& rTriPresent, AABBTree<TestMesh>* pBVH, AABBTree<TestMesh>::Node* pNode )
{
    if( pNode->IsLeaf() )
    {
        uint32 nFirst;
        uint32 nLast;
        pBVH->GetNodeObjectRange( pNode, nFirst, nLast );
        for( ; nFirst != nLast; nFirst++ )
            rTriPresent[nFirst] = rTriPresent[nFirst]+1;

    }
    else
    {
        if( !pBVH->GetNodeBoundingVolume( pNode ).Contains( pBVH->GetNodeBoundingVolume( pBVH->GetLeftChild(pNode) ) ) )
            printf("ILLEGAL BVH\n");
        if( !pBVH->GetNodeBoundingVolume( pNode ).Contains( pBVH->GetNodeBoundingVolume( pBVH->GetRightChild(pNode) ) ) )
            printf("ILLEGAL BVH\n");

        CheckBVH( rTriPresent, pBVH, pBVH->GetLeftChild( pNode ) );
        CheckBVH( rTriPresent, pBVH, pBVH->GetRightChild( pNode ) );
    }
}



//=====================================================================================================================
//
//         Constructors/Destructors
//
//=====================================================================================================================

AABBTreeRaycaster::AABBTreeRaycaster( TestMesh* pMesh, AABBTree<TestMesh>* pBVH ) : TestRaycaster( pMesh ), m_pBVH(pBVH)
{
    // TEMPORARY DEBUG CODE:  Verify that all triangles made it into the BVH
    std::vector<uint32> triPresent;
    triPresent.insert( triPresent.end(), pMesh->GetObjectCount(), 0 );
    CheckBVH( triPresent, pBVH, pBVH->GetRoot() );

    for( uint32 i=0; i<pMesh->GetObjectCount(); i++ )
    {
        if( triPresent[i] != 1 )
        {
            printf("MISSING OR MULTIPLY REFERENCED TRIANGLE FOUND!!");
        }
    }
}

AABBTreeRaycaster::~AABBTreeRaycaster()
{
    delete m_pBVH; 
}

//=====================================================================================================================
//
//            Public Methods
//
//=====================================================================================================================

void AABBTreeRaycaster::RaycastFirstHit( Ray& rRay, TriangleRayHit& rHitInfo )
{
    static ScratchMemory memory;
    RaycastBVH( m_pBVH, GetMesh(), rRay, rHitInfo, m_pBVH->GetRoot(), memory );
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

