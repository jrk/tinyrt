//=====================================================================================================================
//
//   PlyLoad.h
// 
//   Simple PLY mesh loader for TRT samples
//
//   Part of the TinyRT Raytracing Library.
//   Author: Joshua Barczak
//
//   Copyright 2009 Joshua Barczak.  All rights reserved.
//   See  Doc/LICENSE.txt for terms and conditions.
//
//=====================================================================================================================


#ifndef _PLY_LOAD_H_
#define _PLY_LOAD_H_

#include <vector>
#include "TRTTypes.h"

/// Loads a ply mesh and returns an array of vertices and indices
/// If bFixVerts is true, the mesh is scaled and translated so that its bottom is at y=0 and it fits in a unit box
bool LoadPlyMesh( const char* pMeshFileName, std::vector<TinyRT::Vec3f>& rVertices, std::vector<TinyRT::uint32>& rIndices, bool bFixVerts );




#endif
