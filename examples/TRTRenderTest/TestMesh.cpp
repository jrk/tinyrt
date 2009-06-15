//=====================================================================================================================
//
//   TestMesh.cpp
//
//   Implementation of class: TestMesh
//
//   Part of the TinyRT Raytracing Library.
//   Author: Joshua Barczak
//
//   Copyright 2008 Joshua Barczak.  All rights reserved.
//   See  Doc/LICENSE.txt for terms and conditions.
//
//=====================================================================================================================

#include "TestMesh.h"
#include "PlyLoad.h"


/// \brief Reorders vertices in a mesh in order to optimize memory locality for the vertex data
template< class Vertex_T, class Index_T >
void OptimizeVertexOrder( Vertex_T* pVB, Index_T* pIB, uint nVertices, uint nIndices )
{
    Index_T nMaxVert = std::numeric_limits<Index_T>::max();

    // make an array to hold the new index of each vertex
    ScopedArray<Index_T> pNewVertexIDs( new Index_T[nVertices] );
    for( uint i=0; i<nVertices; i++ )
        pNewVertexIDs[i] = nMaxVert;
    
    // walk the mesh, and assign new IDs to each vertex as they appear
    Index_T nVertexID=0;
    for( uint i=0; i<nIndices; i++ )
    {
        Index_T nIndex = pIB[i];
        if( pNewVertexIDs[ nIndex ] == nMaxVert )
        {
            pNewVertexIDs[ nIndex ] = nVertexID;
            nVertexID++;
        }

        // remap the index buffer as we go
        pIB[i] = pNewVertexIDs[nIndex];
    }

    // copy the vertices into their new positions
    ScopedArray<Vertex_T> pVerts( new Vertex_T[nVertices] );
    for( uint i=0; i<nVertices; i++ )
        pVerts[ pNewVertexIDs[i] ] = pVB[i];

    // now copy back to original VB
    memcpy( pVB, pVerts, sizeof(Vertex_T)*nVertices );
}


//=====================================================================================================================
//
//         Constructors/Destructors
//
//=====================================================================================================================

TestMesh::TestMesh(  std::vector<Vec3f>* pPositions, std::vector<uint32>* pIndices )
: m_pVertexArray( pPositions ), m_pIndexArray(pIndices),
  BasicMesh<Vec3f,uint32>( &pPositions->at(0), &pIndices->at(0), pPositions->size(), pIndices->size()/3 )
{
}

TestMesh::~TestMesh()
{
    delete m_pVertexArray;
    delete m_pIndexArray;
}

//=====================================================================================================================
//
//            Public Methods
//
//=====================================================================================================================

TestMesh* TestMesh::LoadPly( const char* pFile, bool bFixVerts )
{
    std::vector<Vec3f>* pVerts    = new std::vector<Vec3f>();
    std::vector<uint32>* pIndices = new std::vector<uint32>();

    if( !LoadPlyMesh( pFile, *pVerts, *pIndices, bFixVerts ) )
    {
        delete pVerts;
        delete pIndices;
        return NULL;
    }

    return new TestMesh( pVerts, pIndices );
}


void TestMesh::Optimize()
{
    std::vector<uint32> ibCopy = *m_pIndexArray;
    OptimizeVertexOrder( &m_pVertexArray->at(0), &(m_pIndexArray->at(0)), m_pVertexArray->size(), m_pIndexArray->size() );
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
