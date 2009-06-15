//=====================================================================================================================
//
//   TRTKDTree.inl
//
//   Implementation of class: TinyRT::KDTree
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
    //         Constructors/Destructors
    //
    //=====================================================================================================================
    
    template< class ObjectSet_T >
    KDTree<ObjectSet_T>::KDTree() :
        m_nNodesInUse(0), 
        m_nNodeArraySize(0), 
        m_nObjectRefsInUse(0), 
        m_nObjectRefArraySize(0)
    {
    }

    //=====================================================================================================================
    //
    //            Public Methods
    //
    //=====================================================================================================================
    
    //=====================================================================================================================
    //=====================================================================================================================
    template< class ObjectSet_T >
    typename KDTree<ObjectSet_T>::NodeHandle KDTree<ObjectSet_T>::Initialize( const AxisAlignedBox& rRootAABB )
    {
        if( m_nNodeArraySize < 1 )
        {
            m_pNodes.resize( 100, m_nNodeArraySize );
            m_nNodeArraySize = 100;
        }

        m_pNodes[0].MakeLeafNode(0,0);
        m_nNodesInUse = 1;
        m_nObjectRefsInUse = 0;
        m_aabb = rRootAABB;
        return 0;
    }

    //=====================================================================================================================
    //=====================================================================================================================
    template< class ObjectSet_T >
    std::pair< typename KDTree<ObjectSet_T>::NodeHandle, typename KDTree<ObjectSet_T>::NodeHandle > 
        KDTree<ObjectSet_T>::MakeInnerNode( NodeHandle hNode, float fSplitPlane, uint nSplitAxis )
    {
        m_nNodesInUse += 2;
        if( m_nNodeArraySize < m_nNodesInUse )
        {
            uint nNewSize = std::max( m_nNodeArraySize*2, m_nNodesInUse );
            m_pNodes.resize( nNewSize, m_nNodeArraySize );
            m_nNodeArraySize = nNewSize;
        }

        NodeHandle hLeft = m_nNodesInUse - 2;
        m_pNodes[hNode].MakeInnerNode( hLeft, fSplitPlane, nSplitAxis );

        return std::pair< NodeHandle, NodeHandle > ( hLeft, hLeft+1 );
    }

    //=====================================================================================================================
    //=====================================================================================================================
    template< class ObjectSet_T >
    typename KDTree<ObjectSet_T>::obj_id* KDTree<ObjectSet_T>::MakeLeafNode( typename KDTree<ObjectSet_T>::NodeHandle n, 
                                                                             typename KDTree<ObjectSet_T>::obj_id nObjCount )
    {
        uint nStart = m_nObjectRefsInUse;
        m_nObjectRefsInUse += nObjCount;

        if( m_nObjectRefArraySize < m_nObjectRefsInUse )
        {
            uint nNewSize = std::max( m_nObjectRefArraySize*2, m_nObjectRefsInUse );
            m_pObjectRefs.resize( nNewSize, m_nObjectRefArraySize );
            m_nObjectRefArraySize = nNewSize;
        }

        m_pNodes[n].MakeLeafNode( nStart, nObjCount );
        return &m_pObjectRefs[nStart];
    }

    //=====================================================================================================================
    //=====================================================================================================================
    template< class ObjectSet_T >
    template< class KDTreeBuilder_T >
    void KDTree<ObjectSet_T>::Build( ObjectSet_T* pObjects, KDTreeBuilder_T& rBuilder )
    {
        m_nStackDepth = rBuilder.BuildTree( pObjects, this );
    }

    //=====================================================================================================================
    //=====================================================================================================================
    template< class ObjectSet_T >
    bool KDTree<ObjectSet_T>::Validate()
    {
        for( uint i=0; i<m_nNodesInUse; i++ )
        {
            const Node* pN = &m_pNodes[i];
            if( pN->IsLeaf() )
            {
                if( pN->GetChildren() >= m_nNodesInUse-1 )
                    return false;
            }
            else
            {
                if( pN->GetObjectCount() > 0 && pN->GetFirstObject() + pN->GetObjectCount() >= m_nObjectRefsInUse )
                    return false;
            }
        }

        return true;
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
}

