//=====================================================================================================================
//
//   GridRaycaster.cpp
//
//   Implementation of class: GridRaycaster
//
//   Part of the TinyRT Raytracing Library.
//   Author: Joshua Barczak
//
//   Copyright 2008 Joshua Barczak.  All rights reserved.
//   See  Doc/LICENSE.txt for terms and conditions.
//
//=====================================================================================================================

#include "GridRaycaster.h"

//=====================================================================================================================
//
//         Constructors/Destructors
//
//=====================================================================================================================

GridRaycaster::GridRaycaster( TestMesh* pMesh ) : TestRaycaster( pMesh ), m_pGrid(0)
{
    m_pGrid = new UniformGrid<TestMesh>();
    m_pGrid->Build( pMesh, 100 );
}

GridRaycaster::~GridRaycaster()
{
    if( m_pGrid )
        delete m_pGrid;
}

//=====================================================================================================================
//
//            Public Methods
//
//=====================================================================================================================

void GridRaycaster::RaycastFirstHit( Ray& rRay, TriangleRayHit& rHitInfo )
{
    RaycastUniformGrid<MailboxType>( m_pGrid, GetMesh(), rRay, rHitInfo );
}

float GridRaycaster::ComputeCost( float fISectCost ) const
{
    return GetUniformGridSAHCost( fISectCost, m_pGrid );
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
