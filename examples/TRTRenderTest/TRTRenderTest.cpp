//=====================================================================================================================
//
//   TRTRenderTest.cpp
//
//   Rendering test harness for TRT.  Performs simple raycasting against a mesh, using all of TRT's data structures
//
//   Part of the TinyRT Raytracing Library.
//   Author: Joshua Barczak
//
//   Copyright 2008 Joshua Barczak.  All rights reserved.
//   See  Doc/LICENSE.txt for terms and conditions.
//
//=====================================================================================================================


#include <iostream>

#include "TinyRT.h"

using namespace TinyRT;

#include "PPMImage.h"
#include "Timer.h"
#include "TestUtils.h"

#include "TestRaycaster.h"
#include "AABBTreeRaycaster.h"
#include "GridRaycaster.h"
#include "QBVHRaycaster.h"
#include "KDTreeRaycaster.h"

#include "RenderTest.h"

#include "BoundingBoxViewpointGenerator.h"


template< class Tree_T >
void PrintTreeStats( const Tree_T* pTree )
{
    TreeStatistics stats;
    GetTreeStatistics( stats, pTree );

    size_t nMemUsed, nMemAllocated;
    pTree->GetMemoryUsage( nMemUsed, nMemAllocated );
    nMemUsed /= 1024;
    nMemAllocated /= 1024;

    printf("====================================================\n");
    printf("              TREE STATS\n");
    printf("====================================================\n");
    printf("    Nodes: %u\n"
           "    Leafs: %u (%.2f%% empty)\n"
           "    Objects: %u\n"
           "    Objects (min/max/avg/avg_nonempty) %u / %u / %u / %u\n"
           "    Memory used/alloated(KB): %u / %u\n",
           stats.nNodes, stats.nLeafs, stats.nEmptyLeafs*100.0f / (float) stats.nLeafs, stats.nTotalObjects,
           stats.nMinObjectsPerLeaf, stats.nMaxObjectsPerLeaf, stats.nAvgObjectsPerLeaf, stats.nAvgObjectsPerNonEmptyLeaf,
           nMemUsed, nMemAllocated );
    printf("=====================================================\n");
}


void RandomRayTest( TestRaycaster* pCast, int nRays )
{
    srand(0);
    printf("RANDOM RAY TEST\n");

    AxisAlignedBox box;
    TestMesh* pMesh = pCast->GetMesh();
    pMesh->GetAABB( box );

    Timer tm;
    for( int i=0; i<nRays; i++ )
    {
        TinyRT::Vec3f vS1 = TinyRT::Vec3f( RandomFloat(), RandomFloat(), RandomFloat() );
        TinyRT::Vec3f vS2 = TinyRT::Vec3f( RandomFloat(), RandomFloat(), RandomFloat() );

        for(int j=0; j<3; j++ )
        {
            vS1[j] = Lerp( box.Min()[j], box.Max()[j], vS1[j] );
            vS2[j] = Lerp( box.Min()[j], box.Max()[j], vS2[j] );
        }

        TinyRT::Ray r( vS1, vS2-vS1 );
        TriangleRayHit hit;
        pCast->RaycastFirstHit( r, hit );
    }


    uint32 nTime = tm.Tick();
    printf("Time: %u.  Rays/s: %.2f\n", nTime, nRays / (nTime/1000.0f) );

}

template< class AABBTreeBuilder_T >
void DoBVHTest( TestMesh* pMesh, AABBTreeBuilder_T& builder, float fTriCost, ViewpointGenerator* pViews,
                RenderTest::Options& renderOpts )
{    
    Timer tm;
    AABBTree< TestMesh >* pBVH = new AABBTree<TestMesh>();
    pBVH->Build( pMesh, builder );
    printf("Build took: %u ms\n", tm.Tick() );

    PrintTreeStats( pBVH );

    float fCost = GetAABBTreeSAHCost( ConstantCost<uint32>(fTriCost), pBVH, pBVH->GetRoot() );
    printf("SAH cost: %f\n", fCost );

    AABBTreeRaycaster rc( pMesh, pBVH );
    RandomRayTest( &rc, 1000000 );

    RenderTest rt( &rc, pViews, renderOpts );
    rt.Run();
}



void DoQBVHTest( TestMesh* pMesh, float fTriCost, ViewpointGenerator* pViews, RenderTest::Options& renderOpts )
{
    Timer tm;
    SahAABBTreeBuilder< TestMesh > builder( fTriCost );

    QuadAABBTree< TestMesh >* pTree = new QuadAABBTree<TestMesh>;
    pTree->Build( pMesh, builder );
    printf("Build took: %u ms\n", tm.Tick() );

    PrintTreeStats( pTree );

    QBVHRaycaster rc( pMesh, pTree );
    RandomRayTest( &rc, 1000000 );

    RenderTest rt( &rc, pViews, renderOpts );
    rt.Run();
}



void TestBVH( TestMesh* pMesh, ViewpointGenerator* pViews, RenderTest::Options& renderOpts )
{
    float fTriCost = 1;

    printf("SAH QBVH\n");
    printf("=================\n");
    {
        DoQBVHTest( pMesh, 1, pViews, renderOpts );
    }

    printf("MEDIAN CUT BVH\n");
    printf("================\n");
    {
        MedianCutAABBTreeBuilder< TestMesh > builder(2);
        DoBVHTest( pMesh, builder, fTriCost, pViews, renderOpts );
    }

    printf("SAH BVH\n");
    printf("================\n");
    {
        SahAABBTreeBuilder<TestMesh, ConstantCost<uint32> > builder( fTriCost );

        DoBVHTest( pMesh, builder, fTriCost, pViews, renderOpts );
    }
    
}

void TestGrid( TestMesh* pMesh, ViewpointGenerator* pViews, RenderTest::Options& renderOpts )
{
    printf("UNIFORM GRID\n");
    printf("================\n");
    
    Timer tm;
    GridRaycaster rc( pMesh );
    float fElapsed = (float) tm.Tick();
    printf("Grid build took: %.2f\n", fElapsed );
    printf("Grid size: %u x %u x %u\n", rc.GetGrid()->GetCellCounts().x, rc.GetGrid()->GetCellCounts().y, rc.GetGrid()->GetCellCounts().z );
    printf("Cell count: %u\n", rc.GetGrid()->GetCellCounts().x * rc.GetGrid()->GetCellCounts().y * rc.GetGrid()->GetCellCounts().z );
    printf("SAH cost: %f\n", GetUniformGridSAHCost( 3.0f, rc.GetGrid() ) );

    RandomRayTest( &rc, 1000000 );

    RenderTest rt( &rc, pViews, renderOpts );
    rt.Run();
}

void TestKDTree( TestMesh* pMesh, ViewpointGenerator* pViews, RenderTest::Options& renderOpts )
{
    printf("KD-TREE \n");
    printf("================\n");

    static const float ISECT_COST = 3.0f;

    KDTree<TestMesh>* pTree = new KDTree<TestMesh>;
    SahKDTreeBuilder<TestMesh, TestMesh::Clipper > builder( ISECT_COST );
    
    Timer tm;
    
    pTree->Build( pMesh, builder );

    float fElapsed = (float) tm.Tick();
    printf("Build took: %.2f\n", fElapsed );

    PrintTreeStats( pTree );

    printf("SAH cost: %f\n", GetKDTreeSAHCost( ISECT_COST, pTree, pTree->GetRoot(), pTree->GetBoundingBox() ) );

    KDTreeRaycaster rc( pMesh, pTree );

    RandomRayTest( &rc, 1000000 );

    RenderTest rt( &rc, pViews, renderOpts );
    rt.Run();

}



/// Simple little mailboxing performance tester
template< class Mailbox_T, int TEST_VAL_COUNT >
long MailboxPerf(  )
{
    Mailbox_T mb;
    int nTestVals[TEST_VAL_COUNT];
    for( int i=0; i<TEST_VAL_COUNT; i++ )
        nTestVals[i] = rand();

    Timer tm;

    for(int i=0; i<10000000; i++ )
    {
        mb.CheckMailbox(nTestVals[i%TEST_VAL_COUNT]);
    }

    return tm.Tick();
}



template<int SIZE>
void MailboxPerfTest()
{
    printf("--------------\n");
    printf("SIZE:     %u\n", SIZE );
    printf("--------------\n");
    printf("Fifo:     %u\n", MailboxPerf< FifoMailbox<int,SIZE>,128 >() );
    printf("DM:       %u\n", MailboxPerf< DirectMapMailbox<int,SIZE>,128 >() );
    printf("SIMDFifo: %u\n", MailboxPerf< SimdFifoMailbox<SIZE>,128 >() );
}


int main()
{
    printf("Loading mesh\n");

    TestMesh* pMesh = TestMesh::LoadPly( "..\\models\\bunny.ply", true );
    if( !pMesh )
    {
        printf("ERROR LOADING MODEL\n");
        return 1;
    }

    RenderTest::Options renderOpts;
    renderOpts.dumpFilePrefix = "test";
    renderOpts.goldImagePrefix = "";//"goldimages\\bunny\\test";
    renderOpts.nImageSize = 256;
    renderOpts.nTileSize = 4;

    AxisAlignedBox meshBox;
    pMesh->GetAABB( meshBox );

    BoundingBoxViewpointGenerator views( meshBox, 10 );
    TestKDTree( pMesh, &views, renderOpts );
    TestBVH( pMesh, &views, renderOpts );
    TestGrid( pMesh, &views, renderOpts );



    delete pMesh;
    
    return 0;

}
