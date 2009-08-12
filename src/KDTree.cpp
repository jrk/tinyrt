//=====================================================================================================================
//
//   ConceptCheck.cpp
//
//   The code in this file does not actually do anything meaningful.  It exists only to perform compile-time checking. 
//    If this code compiles, it means that all of the classes provided by TinyRT implement their concept interfaces correctly
//
//   Part of the TinyRT Raytracing Library.
//   Author: Joshua Barczak
//
//   Copyright 2008 Joshua Barczak.  All rights reserved.
//   See  Doc/LICENSE.txt for terms and conditions.
//
//=====================================================================================================================



#include "..\\include\\TinyRT.h"
using namespace TinyRT;

#include "..\\include\\TRTConcepts.h"


// Verifies that a class adheres to the KDTree_C interface
template< class KDTree_T >
static void CheckConceptSignature_KDTree_C( KDTree_T* p )
{
    typename KDTree_T::ConstNodeHandle root = p->GetRoot();
    typename KDTree_T::ConstNodeHandle lc = p->GetLeftChild(root);
    typename KDTree_T::ConstNodeHandle rc = p->GetRightChild(root);
    
    p->IsNodeLeaf( lc );
    
    uint32 a = p->GetNodeSplitAxis(lc);
    float f = p->GetNodeSplitPosition(rc);
    size_t n = p->GetStackDepth();

    typename KDTree_T::LeafIterator itStart, itEnd;
    p->GetNodeObjectList( lc, itStart, itEnd );
 
}


// Verify that the KD tree class works correctly with any object set
static void ConceptCheckKDTree( KDTree<ObjectSet_C >* pKDTree,Ray_C& rRay, HitInfo_C& rHitInfo, ObjectSet_C* pObjects )
{
    CheckConceptSignature_KDTree_C( pKDTree );

    ScratchMemory mem;
    RaycastKDTree<Mailbox_C>( pKDTree, pObjects, rRay, rHitInfo, pKDTree->GetRoot(), mem );
}

// Verify that KD tree builders will work with any class implementing the KDTree_C interface
static void ConceptCheckKDTreeBuilders( KDTree_C* pTree, ObjectSet_C* pObjects )
{
    SahKDTreeBuilder< ObjectSet_C, Clipper_C > sah( 3.0f );
    sah.BuildTree( pObjects, pTree );
}


// Verify that the raycasting methods work correctly for their concepts
static void ConceptCheckRaycast( Ray_C& rRay, HitInfo_C& rHitInfo )
{
    KDTree_C* pKDTree= 0;
    Mailbox_C* pMailbox=0;
    ObjectSet_C* pObjects = 0;

    
    ScratchMemory mem;
    RaycastKDTree<Mailbox_C>( pKDTree, pObjects, rRay, rHitInfo, pKDTree->GetRoot(), mem );
}

