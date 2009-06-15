//=====================================================================================================================
//
//   TestMesh.h
//
//   Part of the TinyRT Raytracing Library.
//   Author: Joshua Barczak
//
//   Copyright 2008 Joshua Barczak.  All rights reserved.
//   See  Doc/LICENSE.txt for terms and conditions.
//
//=====================================================================================================================

#ifndef _TRT_TESTMESH_H_
#define _TRT_TESTMESH_H_

#include "TinyRT.h"
using namespace TinyRT;


//=====================================================================================================================
/// \brief Mesh loading class for the TRT test suite
///
//=====================================================================================================================
class TestMesh : public BasicMesh<Vec3f,uint32>
{
public:

    ~TestMesh();
    
    static TestMesh* LoadPly( const char* pFile, bool bFixVerts );

    void Optimize();

private:

    TestMesh( std::vector<Vec3f>* pPositions, std::vector<uint32>* pIndices );

    // you might think there's a perf hit during raytracing, but there isn't. 
    // BasicMesh has pointers directly into these vectors.  There is only a slight size overhead
    std::vector<Vec3f>*  m_pVertexArray;
    std::vector<uint32>* m_pIndexArray;
};


#endif // _TRT_TESTMESH_H_
