//=====================================================================================================================
//
//   TRTMath.h
//
//   Definition of class: TinyRT::Math
//
//   Part of the TinyRT Raytracing Library.
//   Author: Joshua Barczak
//
//   Copyright 2008 Joshua Barczak.  All rights reserved.
//   See  Doc/LICENSE.txt for terms and conditions.
//
//=====================================================================================================================

#ifndef _TRT_MATH_H_
#define _TRT_MATH_H_

#include <math.h>

namespace TinyRT
{

#ifdef __GNUC__
    inline float sqrtf( float f ) { return sqrt(f); };
#endif

    /// \ingroup TinyRT
    template< class uint_t > inline bool IsPowerOfTwo( uint_t x )
    {
        return ( (x > 0) && ((x & (x - 1)) == 0) );
    }

    // ---------------------------------------------------------------------------------------------------
    // Length
    // ---------------------------------------------------------------------------------------------------

    //=====================================================================================================================
    /// \ingroup TinyRT
    //=====================================================================================================================
    template<class Scalar> inline Scalar Length3( const Scalar* v1 )  
    { 
        return sqrtf( v1[0]*v1[0] + v1[1]*v1[1] + v1[2]*v1[2] );
    };

    //=====================================================================================================================
    /// \ingroup TinyRT
    //=====================================================================================================================
    template< class Vec3f_T >
    inline float Length3( const Vec3f_T& a )
    {
        return sqrtf( a[0]*a[0] + a[1]*a[1] + a[2]*a[2] );
    };


    //=====================================================================================================================
    /// \ingroup TinyRT
    //=====================================================================================================================
    template<class Scalar> inline Scalar Length3Sq( const Scalar* v1 )  
    { 
        return ( v1[0]*v1[0] + v1[1]*v1[1] + v1[2]*v1[2] );
    };

    //=====================================================================================================================
    /// \ingroup TinyRT
    //=====================================================================================================================
    template< class Vec3f_T >
    inline float Length3Sq( const Vec3f_T& a )
    {
        return ( a[0]*a[0] + a[1]*a[1] + a[2]*a[2] );
    };

    // ---------------------------------------------------------------------------------------------------
    // Dot product
    // ---------------------------------------------------------------------------------------------------


    //=====================================================================================================================
    /// \ingroup TinyRT
    //=====================================================================================================================
    template<class Scalar> inline Scalar Dot3( const Scalar* v1, const Scalar* v2 ) 
    { 
        return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
    };


    //=====================================================================================================================
    /// \ingroup TinyRT
    //=====================================================================================================================
    template< class Scalar >
    inline Scalar Dot3( const Vec3<Scalar>& a, const Vec3<Scalar>& b )
    {
        return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
    };

    // ---------------------------------------------------------------------------------------------------
    //  Normalization
    // ---------------------------------------------------------------------------------------------------


    //=====================================================================================================================
    /// \ingroup TinyRT
    //=====================================================================================================================
    template< class Vec3_T >
    inline Vec3_T Normalize3( const Vec3_T& rV )
    {
        return rV * ( 1.0f / Length3(rV) );
    };

    //=====================================================================================================================
    /// \ingroup TinyRT
    //=====================================================================================================================
    template< class Scalar >
    inline void Normalize3( const Scalar v[3], Scalar vOut[3] )
    {
        Scalar vNrm = 1.0f / Length3(v);
        vOut[0] = v*vNrm;
        vOut[1] = v*vNrm;
        vOut[2] = v*vNrm;
    }

    //=====================================================================================================================
    /// \ingroup TinyRT
    //=====================================================================================================================
    template< class Scalar >
    inline void Normalize3( Scalar v[3] )
    {
        Scalar vNrm = 1.0f / Length3(v);
        v[0] *= v;
        v[1] *= v;
        v[2] *= v;
    }

    // ---------------------------------------------------------------------------------------------------
    //   Cross Product
    // ---------------------------------------------------------------------------------------------------


    //=====================================================================================================================
    /// \ingroup TinyRT
    //=====================================================================================================================
    template< class Vec3_T_1, class Vec3_T_2 >
    inline Vec3_T_1 Cross3( const Vec3_T_1& v1, const Vec3_T_2& v2 )
    {
        return Vec3_T_1( (v1.y*v2.z) - (v1.z*v2.y),
                         (v1.z*v2.x) - (v1.x*v2.z),
                         (v1.x*v2.y) - (v1.y*v2.x) );
    };


    //=====================================================================================================================
    /// \ingroup TinyRT
    //=====================================================================================================================
    template< class Scalar >
    inline void Cross3( const Scalar v1[3], const Scalar v2[3], Scalar vOut[3] )
    {
        vOut[0] =  (v1[1]*v2[2]) - (v1[2]*v2[1]);
        vOut[1] =  (v1[2]*v2[0]) - (v1[0]*v2[2]);
        vOut[2] =  (v1[0]*v2[1]) - (v1[1]*v2[0]);
    };

    // ---------------------------------------------------------------------------------------------------
    //   Interpolation
    // ---------------------------------------------------------------------------------------------------

    /// \ingroup TinyRT
    template< class Scalar1, class Scalar2, class Scalar3 >
    inline Scalar1 Lerp( const Scalar1& a, const Scalar2& b, const Scalar3& t )
    {
        return a + (b-a)*t;
    }

    /// \ingroup TinyRT
    template< class Vec3_T_1, class Vec3_T_2, class Scalar >
    inline Vec3_T_1 Lerp3( const Vec3_T_1& v1, const Vec3_T_2& v2, const Scalar& t )
    {
        return Vec3_T_1( Lerp( v1[0], v2[0], t ),
                         Lerp( v1[1], v2[1], t ),
                         Lerp( v1[2], v2[2], t ) );
    };

    /// \ingroup TinyRT
    template< class Vec3_T_1, class Vec3_T_2, class Scalar >
    inline void Lerp3( const Vec3_T_1& v1, const Vec3_T_2& v2, const Scalar& t, Vec3_T_1& rOut )
    {
        rOut[0] = Lerp( v1[0], v2[0], t );
        rOut[1] = Lerp( v1[1], v2[1], t );
        rOut[2] = Lerp( v1[2], v2[2], t );
    };

    // ---------------------------------------------------------------------------------------------------
    //   Minima/Maxima
    // ---------------------------------------------------------------------------------------------------

    template< class Scalar_T >
    inline Scalar_T Clamp( Scalar_T x, Scalar_T xMin, Scalar_T xMax )
    {
        TRT_ASSERT( xMax >= xMin );
        if( x <= xMin ) return xMin;
        if( x >= xMax ) return xMax;
        return x;
    }

    //=====================================================================================================================
    /// \ingroup TinyRT
    /// Computes the component-wise minima of a pair of vectors
    //=====================================================================================================================
    template< class Vec3_T_1, class Vec3_T_2 >
    inline Vec3_T_1 Min3( const Vec3_T_1& a, const Vec3_T_2& b )
    {
        return Vec3_T_1( std::min(a.x,b.x), std::min(a.y,b.y), std::min(a.z,b.z) );
    }

    //=====================================================================================================================
    /// \ingroup TinyRT
    /// Computes the component-wise maxima of a pair of vectors
    //=====================================================================================================================
    template< class Vec3_T_1, class Vec3_T_2 >
    inline Vec3_T_1 Max3( const Vec3_T_1& a, const Vec3_T_2& b  )
    {
        return Vec3_T_1( std::max(a.x,b.x), std::max(a.y,b.y), std::max(a.z,b.z) );
    }

    //=====================================================================================================================
    /// \ingroup TinyRT
    /// Computes the min and max of a pair of vectors.
    /// \param a    Vector whose components are set to min(a,b)
    /// \parma b    Vector whose components are set to max(a,b)
    //=====================================================================================================================
    template< class Vec3_T_1, class Vec3_T_2 >
    inline void MinMax3( Vec3_T_1& a, Vec3_T_2& b )
    {
        if( a[0] > b[0] ) std::swap( a[0], b[0] );
        if( a[1] > b[1] ) std::swap( a[1], b[1] );
        if( a[2] > b[2] ) std::swap( a[2], b[2] );
    }


    // ---------------------------------------------------------------------------------------------------
    //   Reflection/refraction
    // ---------------------------------------------------------------------------------------------------

    //=====================================================================================================================
    /// \ingroup TinyRT
    /// Computes the reflection vector
    /// \param N   Surface normal
    /// \param I   Incident vector for which reflection direction is computed
    /// \return Reflection of -I about N
    //=====================================================================================================================
    template< class Vec3_T >
    inline Vec3_T Reflect( const Vec3_T& N, const Vec3_T& I )
    {
        return I - N * (2*Dot3(N,I));
    }

    //=====================================================================================================================
    /// \ingroup TinyRT
    /// Computes the reflection vector
    /// \param N   Surface normal
    /// \param I   Incident vector for which reflection direction is computed
    /// \param R   Receives the reflected vector
    //=====================================================================================================================
    template< class Vec3_T >
    inline void Reflect( const Vec3_T& N, const Vec3_T& I, Vec3_T& R )
    {
        float f = 2.0f*Dot3(N,I);
        R[0] = I[0] - N[0]*f ;
        R[1] = I[1] - N[1]*f ;
        R[2] = I[2] - N[2]*f ;
    }

    //=====================================================================================================================
    /// \param I    Incident vector to refract
    /// \param N    Surface normal, assumed to point in the opposite direction as I
    /// \param eta  Relative index of refraction at the boundary (outside IOR / inside IOR)
    /// \param R    Receives the refracted direction (unnormalized)
    /// \return True if refraction occurs, false if total internal reflection occurs.  
    ///              R is not modified in the event of total internal reflection
    //=====================================================================================================================
    template< class Vec3_T >
    inline bool Refract( const Vec3_T& I, const Vec3_T& N, float eta, Vec3_T& R )
    {
        float vdotn = Dot3(I,N);

        float B = (eta*eta)*(1.0f - vdotn*vdotn);
        if( B > 1.0 )
        {
            // total internal reflection
            return false;
        }
        else
        {
            float f = (-vdotn*eta - sqrtf(1.0f-B));
            R[0] = N[0]*f + I[0]*eta;
            R[1] = N[1]*f + I[1]*eta;
            R[2] = N[2]*f + I[2]*eta;
            return true;
        }
    }


}

#endif // _TRT_MATH_H_
