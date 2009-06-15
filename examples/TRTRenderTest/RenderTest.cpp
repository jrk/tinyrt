//=====================================================================================================================
//
//   RenderTest.cpp
//
//
//   Part of the TinyRT Raytracing Library.
//   Author: Joshua Barczak
//
//   Copyright 2008 Joshua Barczak.  All rights reserved.
//   See  Doc/LICENSE.txt for terms and conditions.
//
//=====================================================================================================================

#define _CRT_SECURE_NO_WARNINGS // make VC++ shut up about printf

#include "RenderTest.h"
#include "PPMImage.h"
#include "TestUtils.h"
#include "Timer.h"

//=====================================================================================================================
//
//         Constructors/Destructors
//
//=====================================================================================================================

//=====================================================================================================================
//=====================================================================================================================
RenderTest::RenderTest( TestRaycaster* pRC, ViewpointGenerator* pViews, const Options& rOptions )
: m_pViewpoints( pViews ), m_pRaycaster( pRC ), m_opts( rOptions )
{
};

RenderTest::~RenderTest()
{
}

//=====================================================================================================================
//
//            Public Methods
//
//=====================================================================================================================

void RenderTest::Run( )
{
    // make sure all tests are deterministic
    srand(0);

    int TILE_SIZE = m_opts.nTileSize;
    int SIZE = m_opts.nImageSize;
    PPMImage image(SIZE,SIZE);

    Timer tm;
    
    Vec3f vPosition;
    Vec3f vLookAt;
    float fFOV;

    uint32 nViewpoints = m_pViewpoints->GetViewpointCount();

    uint32 i=1;

    uint32 nTime=0;
    uint32 nFrames=0;

    while( m_pViewpoints->GetViewpoint( &vPosition, &vLookAt, &fFOV ) )
    {
        printf("Viewpoint %u of %u...", i++, nViewpoints );
        fflush( stdout );

        TinyRT::PerspectiveCamera cam( vPosition, vLookAt-vPosition, Vec3f(0,1,0), fFOV, 1 );


        tm.Reset();
        for( int y = 0; y<SIZE; y += TILE_SIZE )
        {
            for( int x = 0; x < SIZE; x += TILE_SIZE  )
            {
                for( int xi = x; xi < x + TILE_SIZE; xi++ )
                {
                    for( int yi = y; yi < y + TILE_SIZE; yi++ )
                    {
                        float s = (float) xi / (float) SIZE;
                        float t = (float) yi / (float) SIZE;

                        TinyRT::TriangleRayHit triHit;
                        triHit.nTriIdx = 0xffffffff;
                        triHit.vUVCoords = Vec2f(0,0);

                        
                        const Vec3f& vOrigin = cam.GetPosition();
                        Vec3f vDir = cam.GetRayDirectionNDC( Vec2f(s,t) );
                        
                        Ray ray( vOrigin, vDir );
                        m_pRaycaster->RaycastFirstHit( ray, triHit );

                        if( triHit.nTriIdx == 0xffffffff )
                        {
                            image.SetPixel( xi, yi, 0,0,0 );
                        }
                        else
                        {
                            // compute face normal and do N.V shading
                            TestMesh* pMesh = m_pRaycaster->GetMesh();
                            Vec3f v0 = pMesh->VertexPosition( pMesh->Index( triHit.nTriIdx, 0 ) );
                            Vec3f v1 = pMesh->VertexPosition( pMesh->Index( triHit.nTriIdx, 1 ) );
                            Vec3f v2 = pMesh->VertexPosition( pMesh->Index( triHit.nTriIdx, 2 ) );
                            Vec3f vNormal = Normalize3( Cross3( v2-v0, v1-v0 ) );
                            
                            Vec3f vHit = vOrigin + vDir*ray.MaxDistance();
                            Vec3f vV = Normalize3( vHit - vOrigin );

                            float f = fabs( Dot3( vV, vNormal ) );
                            image.SetPixel( xi, yi, f,f,f );
                        }
                    }
                }
            }
        }

        uint32 nThisFrameTime = tm.Tick();
        nTime += nThisFrameTime;
        nFrames++;

        printf("Render took: %u ms\n", nThisFrameTime );
        fflush( stdout );

        // dump images if asked
        if( m_opts.dumpFilePrefix != "" )
        {
            char filename[1000];
            sprintf( filename, "%s_%06d.ppm", m_opts.dumpFilePrefix.c_str(), i-1 );
            if( !image.SaveFile( filename ) )
            {
                printf("ERROR WRITING IMAGE TO: %s", filename );
            }
        }

        // check gold images if asked
        if( m_opts.goldImagePrefix != "" )
        {
            char goldFileName[1000];
            sprintf( goldFileName, "%s_%06d.ppm", m_opts.goldImagePrefix.c_str(), i-1 );
 
            PPMImage gold;
            if( !gold.LoadFile( goldFileName ) )
            {
                printf("UNABLE TO LOAD GOLD IMAGE FROM FILE: '%s'\n", goldFileName );
            }
            else
            {
                PPMImage diffImage;
                
                const int PIXEL_DIFF_THRESHOLD = 1; // maximum difference allowed between 8-bit pixel values
                const int PIXEL_DIFF_COUNT_THRESHOLD = 5;   // maximum number of erroneous pixels before complaining

                bool ok = true;

                unsigned int numDiffs = 0;
                for( int x=0; x<image.GetWidth() ; x++ )
                {
                    for(int y=0; y<image.GetHeight() ; y++ )
                    {
                        diffImage.SetPixel(x,y,0,0,0);
                        unsigned char goldBits[3];
                        unsigned char bits[3];
                        image.GetPixelBytes( x,y,bits );
                        gold.GetPixelBytes( x,y,goldBits );
                        
                        for(int k=0; k<3; k++ )
                        {
                            int gold = goldBits[k];
                            int result= bits[k];
                            if( abs( gold - result ) > PIXEL_DIFF_THRESHOLD )
                            {
                                numDiffs++;
                                if( numDiffs == PIXEL_DIFF_COUNT_THRESHOLD+1 )
                                {
                                    // too many erroneous pixels.  Complain later
                                    ok = false;
                                }
                                diffImage.SetPixel(x,y,1,1,1);
                                break;
                            }
                        }
                    }
                }

                // if images didn't match, complain, and dump out a diffed image
                if( !ok )
                {
                    printf("RESULT DOES NOT MATCH GOLD IMAGE: '%s'.  %u differing pixels\n", goldFileName, numDiffs );
                                
                    char diffFile[100];
                    if( m_opts.dumpFilePrefix != "" )
                    {
                        sprintf( diffFile, "%s_%06d_DIFF.ppm", m_opts.dumpFilePrefix.c_str(), i-1 );

                        if( !diffImage.SaveFile( diffFile ) )
                        {
                            printf("ERROR WRITING IMAGE TO: %s", diffFile );
                        }
                    }

                }
            }
        }
    }

    printf("Avg time: %ums\n", nTime / nFrames );

    m_pViewpoints->Reset();
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
