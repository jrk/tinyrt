//=====================================================================================================================
//
//   TRTBoxIntersect.h
//
//   Ray-box intersection routines.
//
//   Part of the TinyRT Raytracing Library.
//   Author: Joshua Barczak
//
//   Copyright 2008 Joshua Barczak.  All rights reserved.
//   See  Doc/LICENSE.txt for terms and conditions.
//
//=====================================================================================================================

#ifndef _TRT_BOXINTERSECT_H_
#define _TRT_BOXINTERSECT_H_


namespace TinyRT
{
    //=====================================================================================================================
    /// \ingroup TinyRT
    /// \brief Ray-box intersection test which returns entry distance.  
    /// \param rBBMin   The lower-left corner of the AABB
    /// \param rBBMax   The upper-right corner of the AABB
    /// \param rRay     The ray to be tested
    /// \param rTMin    If a hit is found, this receives the 'T' value at which the ray enters the box
    ///                     This will be negative if the ray origin is in the box
    /// \return True if the ray hits the box.  False otherwise.  Hits are only returned if the hit lies inside
    ///               of the ray's "valid region"
    //=====================================================================================================================
    template< class Ray_T >
    TRT_FORCEINLINE bool RayAABBTest( const Vec3f& rBBMin, const Vec3f& rBBMax, const Ray_T& rRay, float& rTMinOut )
    {
        const Vec3f& rOrigin = rRay.Origin();
        const Vec3f& rInvDir = rRay.InvDirection();

        float fTMin, fTMax;
        if( rInvDir.x < 0 )
        {
            fTMin = ( rBBMax.x - rOrigin.x ) * rInvDir.x;
            fTMax = ( rBBMin.x - rOrigin.x ) * rInvDir.x;
        }
        else
        {
            fTMin = ( rBBMin.x - rOrigin.x ) * rInvDir.x;
            fTMax = ( rBBMax.x - rOrigin.x ) * rInvDir.x;
        }

        if( rInvDir.y < 0 )
        {
            fTMin = std::max( fTMin, ( rBBMax.y - rOrigin.y ) * rInvDir.y );
            fTMax = std::min( fTMax, ( rBBMin.y - rOrigin.y ) * rInvDir.y );
        }
        else
        {
            fTMin = std::max( fTMin, ( rBBMin.y - rOrigin.y ) * rInvDir.y );
            fTMax = std::min( fTMax, ( rBBMax.y - rOrigin.y ) * rInvDir.y );
        }

        if( rInvDir.z < 0 )
        {
            fTMin = std::max( fTMin, ( rBBMax.z - rOrigin.z ) * rInvDir.z );
            fTMax = std::min( fTMax, ( rBBMin.z - rOrigin.z ) * rInvDir.z );
        }
        else
        {
            fTMin = std::max( fTMin, ( rBBMin.z - rOrigin.z ) * rInvDir.z );
            fTMax = std::min( fTMax, ( rBBMax.z - rOrigin.z ) * rInvDir.z );
        }

        if( fTMax < fTMin || !rRay.IsIntervalValid( fTMin, fTMax ) )
            return false;

        rTMinOut = fTMin;  // return hit distance
        return true;
    }

    //=====================================================================================================================
    /// \ingroup TinyRT
    /// \brief Ray-box intersection test which returns entry and exit distances  
    /// \param rBBMin   The lower-left corner of the AABB
    /// \param rBBMax   The upper-right corner of the AABB
    /// \param rRay     The ray to be tested
    /// \param rTMin    If a hit is found, this receives the 'T' value at which the ray enters the box 
    ///                     This will be negative if the ray origin is in the box
    /// \param rTMax    If a hit is found, this receives the 'T' value at which the ray exits the box
    /// \return True if the ray hits the box.  False otherwise.  Hits are only returned if the hit lies inside
    ///               of the ray's "valid region"
    //=====================================================================================================================
    template< class Ray_T >
    TRT_FORCEINLINE bool RayAABBTest( const Vec3f& rBBMin, const Vec3f& rBBMax, const Ray_T& rRay, float& rTMinOut, float& rTMaxOut )
    {
        const Vec3f& rOrigin = rRay.Origin();
        const Vec3f& rInvDir = rRay.InvDirection();

        float fTMin, fTMax;
        if( rInvDir.x < 0 )
        {
            fTMin = ( rBBMax.x - rOrigin.x ) * rInvDir.x;
            fTMax = ( rBBMin.x - rOrigin.x ) * rInvDir.x;
        }
        else
        {
            fTMin = ( rBBMin.x - rOrigin.x ) * rInvDir.x;
            fTMax = ( rBBMax.x - rOrigin.x ) * rInvDir.x;
        }

        if( rInvDir.y < 0 )
        {
            fTMin = std::max( fTMin, ( rBBMax.y - rOrigin.y ) * rInvDir.y );
            fTMax = std::min( fTMax, ( rBBMin.y - rOrigin.y ) * rInvDir.y );
        }
        else
        {
            fTMin = std::max( fTMin, ( rBBMin.y - rOrigin.y ) * rInvDir.y );
            fTMax = std::min( fTMax, ( rBBMax.y - rOrigin.y ) * rInvDir.y );
        }

        if( rInvDir.z < 0 )
        {
            fTMin = std::max( fTMin, ( rBBMax.z - rOrigin.z ) * rInvDir.z );
            fTMax = std::min( fTMax, ( rBBMin.z - rOrigin.z ) * rInvDir.z );
        }
        else
        {
            fTMin = std::max( fTMin, ( rBBMin.z - rOrigin.z ) * rInvDir.z );
            fTMax = std::min( fTMax, ( rBBMax.z - rOrigin.z ) * rInvDir.z );
        }

        if( fTMax < fTMin || !rRay.IsIntervalValid( fTMin, fTMax ) )
            return false;

        rTMinOut = fTMin;  // return hit interval
        rTMaxOut = fTMax;   
        return true;
    }

    //=====================================================================================================================
    /// \ingroup TinyRT
    /// Ray-box intersection test which returns NO hit distance
    /// \param rBBMin   The lower-left corner of the AABB
    /// \param rBBMax   The upper-right corner of the AABB
    /// \param rRay     The ray to be tested
    /// \param rTMin    If a hit is found, this receives the 'T' value at which the ray enters the box (it may be negative)
    /// \return True if the ray hits the box.  False otherwise.  Hits are only returned if the hit lies inside
    ///               of the ray's "valid region"
    //=====================================================================================================================
    template< class Ray_T >
    TRT_FORCEINLINE bool RayAABBTest( const Vec3f& rBBMin, const Vec3f& rBBMax, const Ray_T& rRay )
    {
        const Vec3f& rOrigin = rRay.Origin();
        const Vec3f& rInvDir = rRay.InvDirection();

        float fTMin, fTMax;
        if( rInvDir.x < 0 )
        {
            fTMin = ( rBBMax.x - rOrigin.x ) * rInvDir.x;
            fTMax = ( rBBMin.x - rOrigin.x ) * rInvDir.x;
        }
        else
        {
            fTMin = ( rBBMin.x - rOrigin.x ) * rInvDir.x;
            fTMax = ( rBBMax.x - rOrigin.x ) * rInvDir.x;
        }

        if( rInvDir.y < 0 )
        {
            fTMin = std::max( fTMin, ( rBBMax.y - rOrigin.y ) * rInvDir.y );
            fTMax = std::min( fTMax, ( rBBMin.y - rOrigin.y ) * rInvDir.y );
        }
        else
        {
            fTMin = std::max( fTMin, ( rBBMin.y - rOrigin.y ) * rInvDir.y );
            fTMax = std::min( fTMax, ( rBBMax.y - rOrigin.y ) * rInvDir.y );
        }

        if( rInvDir.z < 0 )
        {
            fTMin = std::max( fTMin, ( rBBMax.z - rOrigin.z ) * rInvDir.z );
            fTMax = std::min( fTMax, ( rBBMin.z - rOrigin.z ) * rInvDir.z );
        }
        else
        {
            fTMin = std::max( fTMin, ( rBBMin.z - rOrigin.z ) * rInvDir.z );
            fTMax = std::min( fTMax, ( rBBMax.z - rOrigin.z ) * rInvDir.z );
        }

        if( fTMax < fTMin || !rRay.IsIntervalValid( fTMin, fTMax ) )
            return false;

        return true;
    }


    //=====================================================================================================================
    /// \ingroup TinyRT
    /// \brief Performs an intersection test between a ray and a vectorized set of AABBs
    /// \param vAABB    Array containing sets of four AABB slabs.  The order is:  XMin,XMax, YMin,YMax, ZMin,ZMax
    /// \param rRay     The ray to be tested
    /// \return A four-bit mask indicating which AABBs were hit by the ray
    //=====================================================================================================================
    template< class Ray_T >
    TRT_FORCEINLINE int RayQuadAABBTest( const SimdVec4f vAABB[6], const Ray_T& rRay )
    {
        const Vec3f& rOrigin = rRay.Origin();
        const Vec3f& rInvDir = rRay.InvDirection();

        // intersect with X aligned slabs 
        SimdVec4f vDirInvX = SimdVec4f( rInvDir.x );
        SimdVec4f vTXIn  = (vAABB[0] - SimdVec4f( rOrigin[0] )) * vDirInvX;
        SimdVec4f vTXOut = (vAABB[1] - SimdVec4f( rOrigin[0] )) * vDirInvX;

        // account for negative directions (make sure tMin is < tmax)
        SimdVec4f vTMin = SimdVec4f::Min( vTXIn, vTXOut );
        SimdVec4f vTMax = SimdVec4f::Max( vTXIn, vTXOut );

        // Y aligned slabs
        SimdVec4f vDirInvY = SimdVec4f( rInvDir.y );
        SimdVec4f vTYIn  = (vAABB[2] - SimdVec4f( rOrigin[1] )) * vDirInvY;
        SimdVec4f vTYOut = (vAABB[3] - SimdVec4f( rOrigin[1] )) * vDirInvY;
        vTMin = SimdVec4f::Max( SimdVec4f::Min( vTYIn, vTYOut ), vTMin ); // we want to grab the largest min and smallest max
        vTMax = SimdVec4f::Min( SimdVec4f::Max( vTYIn, vTYOut ), vTMax );

        // Z aligned slabs
        SimdVec4f vDirInvZ = SimdVec4f( rInvDir.z );
        SimdVec4f vTZIn  = (vAABB[4] - SimdVec4f( rOrigin[2] )) * vDirInvZ;
        SimdVec4f vTZOut = (vAABB[5] - SimdVec4f( rOrigin[2] )) * vDirInvZ;
        vTMin = SimdVec4f::Max( SimdVec4f::Min( vTZIn, vTZOut ), vTMin );
        vTMax = SimdVec4f::Min( SimdVec4f::Max( vTZIn, vTZOut ), vTMax );

        // did we hit any of them? If so, return now
        return ( SimdVecf::Mask( (vTMin <= vTMax) & rRay.AreIntervalsValid( vTMin, vTMax ) ) );
    }

    
    //=====================================================================================================================
    /// \ingroup TinyRT
    ///
    /// \brief Performs an intersection test between a ray and a vectorized set of AABBs.  
    ///
    /// This version uses a precomputed array of direction signs to avoid additional SIMD instructions (at the expense of more address arithmetic).
    ///  
    /// \param vAABB        Array containing sets of four AABB slabs.  The order is:  XMin,XMax, YMin,YMax, ZMin,ZMax
    /// \param rRay         The ray to be tested
    /// \param nDirSigns    Array containing 1 if the corresponding direction component is negative, zero otherwise
    /// \return A four-bit mask indicating which AABBs were hit by the ray
    //=====================================================================================================================
    template< class Ray_T >
    TRT_FORCEINLINE int RayQuadAABBTest( const SimdVec4f vAABB[6], const Ray_T& rRay, const int nDirSigns[3] )
    {
        const Vec3f& rOrigin = rRay.Origin();
        const Vec3f& rInvDir = rRay.InvDirection();

        // intersect with X aligned slabs 
        SimdVec4f vDirInvX = SimdVec4f( rInvDir.x );
        SimdVec4f vTMin  = (vAABB[ 0 + nDirSigns[0] ] - SimdVec4f( rOrigin[0] )) * vDirInvX;
        SimdVec4f vTMax  = (vAABB[ 1 - nDirSigns[0] ] - SimdVec4f( rOrigin[0] )) * vDirInvX;

        // Y aligned slabs
        SimdVec4f vDirInvY = SimdVec4f( rInvDir.y );
        SimdVec4f vTYIn  = (vAABB[ 2 + nDirSigns[1] ] - SimdVec4f( rOrigin[1] )) * vDirInvY;
        SimdVec4f vTYOut = (vAABB[ 3 - nDirSigns[1] ] - SimdVec4f( rOrigin[1] )) * vDirInvY;
        vTMin = SimdVec4f::Max( vTYIn, vTMin ); // we want to grab the largest min and smallest max
        vTMax = SimdVec4f::Min( vTYOut, vTMax );

        // Z aligned slabs
        SimdVec4f vDirInvZ = SimdVec4f( rInvDir.z );
        SimdVec4f vTZIn  = (vAABB[ 4 + nDirSigns[2] ] - SimdVec4f( rOrigin[2] )) * vDirInvZ;
        SimdVec4f vTZOut = (vAABB[ 5 - nDirSigns[2] ] - SimdVec4f( rOrigin[2] )) * vDirInvZ;
        vTMin = SimdVec4f::Max( vTZIn, vTMin );
        vTMax = SimdVec4f::Min( vTZOut, vTMax );

        // did we hit any of them? If so, return now
        return ( SimdVecf::Mask( (vTMin <= vTMax) & rRay.AreIntervalsValid( vTMin, vTMax ) ) );
    }
}

#endif // _TRT_BOXINTERSECT_H_
