//=====================================================================================================================
//
//   GridRaycaster.h
//
//   Part of the TinyRT Raytracing Library.
//   Author: Joshua Barczak
//
//   Copyright 2008 Joshua Barczak.  All rights reserved.
//   See  Doc/LICENSE.txt for terms and conditions.
//
//=====================================================================================================================

#ifndef _TRT_GRIDRAYCASTER_H_
#define _TRT_GRIDRAYCASTER_H_

#include "TestRaycaster.h"


//=====================================================================================================================
/// \ingroup TinyRTTest
/// \brief 
//=====================================================================================================================
class GridRaycaster : public TestRaycaster
{
public:

    GridRaycaster( TestMesh* pMesh );

    virtual ~GridRaycaster();

    virtual void RaycastFirstHit( TinyRT::Ray& rRay, TinyRT::TriangleRayHit& rHitInfo ) ;
    
    virtual float ComputeCost( float fISectCost ) const ;

    inline const UniformGrid<TestMesh>* GetGrid() const { return m_pGrid; };

private:

    UniformGrid<TestMesh>* m_pGrid;

    //typedef NullMailbox MailboxType;
    typedef DirectMapMailbox<uint32,16> MailboxType;
    //typedef FifoMailbox<uint32,8> MailboxType;
    //typedef SimdFifoMailbox<8> MailboxType;

    //FifoMailbox<uint32,8> m_mb;
    //DirectMapMailbox<uint32,8> m_mb;
    //SimdFifoMailbox<8> m_mb;
};


#endif // _TRT_GRIDRAYCASTER_H_
