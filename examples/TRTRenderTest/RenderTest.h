//=====================================================================================================================
//
//   RenderTest.h
//
//   Part of the TinyRT Raytracing Library.
//   Author: Joshua Barczak
//
//   Copyright 2008 Joshua Barczak.  All rights reserved.
//   See  Doc/LICENSE.txt for terms and conditions.
//
//=====================================================================================================================

#ifndef _RENDERTEST_H_
#define _RENDERTEST_H_

#include "TinyRT.h"
using namespace TinyRT;

#include "TestRaycaster.h"
#include "ViewpointGenerator.h"
#include <string>

//=====================================================================================================================
/// \brief Rendering test harness
//=====================================================================================================================
class RenderTest
{
public:

    struct Options
    {
        uint32 nImageSize;          ///< Size of images to render
        uint32 nTileSize;           ///< Organize pixels in NxN tiles
        std::string dumpFilePrefix; ///< Path and filename prefix for image files.  If non-empty, then images will be dumped
        std::string goldImagePrefix; ///< Path and filename prefix for 'gold' images.  If non-empty, gold image testing is done
    };

    RenderTest( TestRaycaster* pRC, ViewpointGenerator* pViews, const Options& rOpts );

    ~RenderTest();

    virtual void Run( );

private:
    
    Options m_opts;
    TestRaycaster* m_pRaycaster;
    ViewpointGenerator* m_pViewpoints;
};



#endif // _TRT_DERTEST_H_
