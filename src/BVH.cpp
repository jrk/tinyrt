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



// Verifies that a class adheres to the BVH_C interface
template< class BVH_T >
static void CheckConceptSignature_BVH_C( BVH_T* p )
{
    typename BVH_T::ConstNodeHandle root = p->GetRoot();
    typename BVH_T::ConstNodeHandle l = p->GetLeftChild(root);
    typename BVH_T::ConstNodeHandle rc = p->GetRightChild(root);
    p->IsNodeLeaf( l );
    
    float t; 
    TinyRT::Ray ray( Vec3f(0,0,0), Vec3f(1,1,1) );
    p->RayNodeTest(l,ray,t);
    p->RayNodeTest(l,ray);

    typename BVH_T::obj_id first;
    typename BVH_T::obj_id last;
    p->GetNodeObjectRange( l, first, last );

    uint32 a = p->GetNodeSplitAxis(l);
    size_t n = p->GetStackDepth();
}

// Verifies that a class adheres to the AABBTree_C interface
template< class AABBTree_T >
static void CheckConceptSignature_AABBTree_C( AABBTree_T* p )
{
    CheckConceptSignature_BVH_C( p );

    // TODO
}



// Verify that the AABB tree class works correctly with any object set
static void ConceptCheckAABBTree( AABBTree< ObjectSet_C >* pAABBTree, Ray_C& rRay, HitInfo_C& rHitInfo, ObjectSet_C* pObjects )
{
    CheckConceptSignature_AABBTree_C( pAABBTree );

    ScratchMemory mem;
    RaycastBVH( pAABBTree, pObjects, rRay, rHitInfo, pAABBTree->GetRoot(), mem );

    ConstantCost< ObjectSet_C::obj_id > cost(0.7f);
    SahAABBTreeBuilder< ObjectSet_C, ConstantCost<ObjectSet_C::obj_id> > sah(cost);
    MedianCutAABBTreeBuilder< ObjectSet_C > bld;

    AABBTree<ObjectSet_C>* pTree;
    pTree->Build( pObjects, bld );
    pTree->Build( pObjects, sah );
}

// Verify that the AABB tree builders will work with any class implementing the AABBTree_C interface
static void ConceptCheckAABBTreeBuilders( AABBTree_C* pTree, ObjectSet_C* pObjs, QuadAABBTree_C* pQBVH )
{
    ConstantCost< ObjectSet_C::obj_id > cost(0.7f);
    SahAABBTreeBuilder< ObjectSet_C, ConstantCost<ObjectSet_C::obj_id> > sah(cost);
    
    MedianCutAABBTreeBuilder< ObjectSet_C > bld;

    bld.BuildTree( pObjs, pTree );
    sah.BuildTree( pObjs, pTree );
    sah.BuildQuadAABBTree( pObjs, pQBVH );
}

// Verify that the raycasting methods work correctly for their concepts
static void ConceptCheckBVHRaycast( Ray_C& rRay, HitInfo_C& rHitInfo )
{
    BVH_C* pBVH=0;
    ObjectSet_C* pObjects = 0;

    ScratchMemory mem;
    RaycastBVH( pBVH, pObjects, rRay, rHitInfo, pBVH->GetRoot(), mem );
}

