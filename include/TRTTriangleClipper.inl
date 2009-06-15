//=====================================================================================================================
//
//   TRTTriangleClipper..inl
//
//   Definition of class: TinyRT::TriangleClipper
//
//   Part of the TinyRT raytracing library.
//   Author: Joshua Barczak
//   Copyright 2009 Joshua Barczak.  All rights reserved.
//
//   See  Doc/LICENSE.txt for terms and conditions.
//
//=====================================================================================================================


namespace TinyRT
{
    //=====================================================================================================================
    //
    //            Public Methods
    //
    //=====================================================================================================================
    
    //=====================================================================================================================
    /// Given an object and a subset of its AABB, this method clips the object by an axis-aligned plane, computes the 
    ///  AABBs of the two halves, and then returns their intersection with the input AABB
    ///
    /// \param pMesh         Mesh whose triangles are being clipped
    /// \param nObjectID     Id of the object being clipped
    /// \param rOldBB        The axis-aligned box of the portion of the object being clipped.  This box must intersect the clipping plane
    /// \param fPosition     Location of axis aligned split plane
    /// \param nAxis         Index of the axis to which the plane is aligned
    /// \param rLeftSideOut  Receives the AABB of the back (lower) portion 
    /// \param rRightSideOut Receives the AABB of the front (upper) portion
    //=====================================================================================================================
    template< class Mesh_T >
    void TriangleClipper<Mesh_T>::ClipObjectToAxisAlignedPlane( const Mesh_T* pMesh, 
                                                                face_id nObjectID, 
                                                                const AxisAlignedBox& rOldBB, 
                                                                float fPosition, 
                                                                uint nAxis,
                                                                AxisAlignedBox& rLeftSideOut, 
                                                                AxisAlignedBox& rRightSideOut )
    {
        // first, catch boundary cases:
        //  For this to be legal, the split plane must actually split the AABB into two non-degenerate pieces along the split axis
        //   This covers a variety of corner cases.  Including splitting flat objects along their flatness axis, and splitting
        //   an object where only one vertex lies inside the split plane.  Tree builders should not introduce these cases.
        TRT_ASSERT( rOldBB.Min()[nAxis] < fPosition && rOldBB.Max()[nAxis] > fPosition );

        // the algorithm is first to compute the intersections of two triangle edges with the split plane (there are exactly two)
        // We then compute the AABBs of the portions of the triangle on either side of the split, and intersect those with the
        //  input AABB

        // arrange the vertices in a consistent order
        const Vec3f& P0 = pMesh->VertexPosition( pMesh->Index( nObjectID, 0 ) );
        const Vec3f& P1 = pMesh->VertexPosition( pMesh->Index( nObjectID, 1 ) );
        const Vec3f& P2 = pMesh->VertexPosition( pMesh->Index( nObjectID, 2 ) );

        uint nMask = (( P0[nAxis] < fPosition ) ? 0 : 1 ) |
                     (( P1[nAxis] < fPosition ) ? 0 : 2 ) |
                     (( P2[nAxis] < fPosition ) ? 0 : 4 ) ;
        
        TRT_ASSERT( IsPowerOfTwo( (~nMask)&7 ) || // one vertex on the left, two on the right
                    IsPowerOfTwo( nMask )  ); // one on the right, two on the left

        bool bRightSide = IsPowerOfTwo(nMask);  // bRightSide means that there is one vertex on the right

        const float* pV0;
        const float* pV1;
        const float* pV2;
        switch( bRightSide ? nMask : ((~nMask)&7) )
        {
        case 1: 
            pV0 = &P0[0];
            pV1 = &P1[0];
            pV2 = &P2[0];
            break;
        case 2:
            pV0 = &P1[0];
            pV1 = &P0[0];
            pV2 = &P2[0];
            break;
        case 4:
            pV0 = &P2[0];
            pV1 = &P0[0];
            pV2 = &P1[0];
            break;
        }

       
        // vCut0 and vCut1 are the two locations where the triangle's edges intersect the split plane
        Vec3f vCut0, vCut1;
        
        float fT0 = ( fPosition - pV0[nAxis] ) / ( pV1[nAxis] - pV0[nAxis] );
        float fT1 = ( fPosition - pV0[nAxis] ) / ( pV2[nAxis] - pV0[nAxis] );     

        uint nA0 = (nAxis+1)%3;
        uint nA1 = (nAxis+2)%3;


        const Vec3f& rOldMin = rOldBB.Min();
        const Vec3f& rOldMax = rOldBB.Max();

        vCut0[nAxis] = fPosition;
        vCut1[nAxis] = fPosition;

        // these need to be clamped, as it is possible they will end up outside the input AABB segment
        //  Under the right circumstances, we can end up with a degenerate result
        //   (this can happen due to the joys of floating point arithmetic)
        vCut0[nA0] = Clamp( pV0[nA0] + (pV1[nA0] - pV0[nA0]) * fT0, rOldMin[nA0], rOldMax[nA0] ); 
        vCut1[nA0] = Clamp( pV0[nA0] + (pV2[nA0] - pV0[nA0]) * fT1, rOldMin[nA0], rOldMax[nA0] );
        vCut0[nA1] = Clamp( pV0[nA1] + (pV1[nA1] - pV0[nA1]) * fT0, rOldMin[nA1], rOldMax[nA1] );
        vCut1[nA1] = Clamp( pV0[nA1] + (pV2[nA1] - pV0[nA1]) * fT1, rOldMin[nA1], rOldMax[nA1] );


        // compute triangle AABBs on each side of the cut, and intersect them with the input AABB
        Vec3f vCutMin = Min3( vCut0, vCut1 );
        Vec3f vCutMax = Max3( vCut0, vCut1 );
        
        
        Vec3f& rLeftMin = rLeftSideOut.Min();
        Vec3f& rRightMin = rRightSideOut.Min();
        Vec3f& rLeftMax = rLeftSideOut.Max();
        Vec3f& rRightMax = rRightSideOut.Max();

        
        if( bRightSide )
        {
            for( int i=0; i<3; i++ )
            {
                // left side is AABB of two verts and two cut points
                rLeftMin[i] = std::max( rOldMin[i], std::min( vCutMin[i], std::min( pV1[i], pV2[i] ) ) );
                rLeftMax[i] = std::min( rOldMax[i], std::max( vCutMax[i], std::max( pV1[i], pV2[i] ) ) );

                // right side is AABB of one vert and two cut points
                rRightMin[i] = std::max( rOldMin[i], std::min( pV0[i], vCutMin[i] ) );
                rRightMax[i] = std::min( rOldMax[i], std::max( pV0[i], vCutMax[i] ) );
            }
        }
        else
        {
            for( int i=0; i<3; i++ )
            {
                // right side is AABB of two verts and two cut points
                rRightMin[i] = std::max( rOldMin[i], std::min( vCutMin[i], std::min( pV1[i], pV2[i] ) ) );
                rRightMax[i] = std::min( rOldMax[i], std::max( vCutMax[i], std::max( pV1[i], pV2[i] ) ) );

                // left side is AABB of one vert and two cut points
                rLeftMin[i] = std::max( rOldMin[i], std::min( pV0[i], vCutMin[i] ) );
                rLeftMax[i] = std::min( rOldMax[i], std::max( pV0[i], vCutMax[i] ) );
            }
        }

        TRT_ASSERT( rLeftSideOut.IsValid() && rRightSideOut.IsValid() );

    }


}

