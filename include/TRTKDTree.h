//=====================================================================================================================
//
//   TRTKDTree.h
//
//   Definition of class: TinyRT::KDTree
//
//   Part of the TinyRT raytracing library.
//   Author: Joshua Barczak
//   Copyright 2009 Joshua Barczak.  All rights reserved.
//
//   See  Doc/LICENSE.txt for terms and conditions.
//
//=====================================================================================================================

#ifndef _TRTKDTREE_H_
#define _TRTKDTREE_H_


namespace TinyRT
{

    //=====================================================================================================================
    /// \ingroup TinyRT
    /// \brief A basic KDTree implementation
    ///
    /// This class implements the KDTree_C concept
    /// \param ObjectSet_T Must implement the ObjectSet_C concept
    //=====================================================================================================================
    template< class ObjectSet_T >
    class KDTree
    {

    public:
                
        typedef ObjectSet_T ObjectSet;
        typedef typename ObjectSet_T::obj_id obj_id;
        typedef typename ObjectSet_T::obj_id obj_id;
        typedef const obj_id* LeafIterator; ///< Type that acts as an iterator over objects in a leaf

        /// A node in a KD-tree
        class Node
        {
        public:
            
            inline float GetSplitPosition() const { return m_fSplitPlane; };
            inline uint GetSplitAxis() const { return m_nSplitAxis; };
            inline bool IsLeaf() const { return m_nSplitAxis == 3; };
            inline uint GetObjectCount() const { return m_nObjCount; };
            inline uint GetFirstObject() const { return m_nLeftChildOrFirstObj; };
            inline uint GetChildren() const { return m_nLeftChildOrFirstObj; };
            inline Node* GetChildren() { return this + m_nLeftChildOrFirstObj; };

            inline void MakeInnerNode( uint nLeftChild, float fSplit, uint nSplitAxis )
            {
                m_fSplitPlane = fSplit;
                m_nSplitAxis = nSplitAxis;
                m_nLeftChildOrFirstObj = nLeftChild;
            }

            inline void MakeLeafNode( uint nFirstObj, uint nObjCount )
            {
                m_nObjCount = nObjCount;
                m_nLeftChildOrFirstObj = nFirstObj;
                m_nSplitAxis = 3;
            };


        private:
            union 
            {
                float  m_fSplitPlane;
                obj_id m_nObjCount; 
            };

             
            unsigned m_nLeftChildOrFirstObj : 30;     ///< Offset of first child (if an inner node), or index of first object (if leaf)
            unsigned m_nSplitAxis : 2;                ///< Split axis (a value of 3 indicates a leaf)
        };

        typedef uint NodeHandle;
        typedef uint ConstNodeHandle;

        inline KDTree( );

        /// Returns the root node of the tree
        inline ConstNodeHandle GetRoot() const                            { return 0; };

        /// Returns the i'th child of a particular (non-leaf) node.  0 is the left
        inline ConstNodeHandle GetChild( ConstNodeHandle r, size_t i ) const { TRT_ASSERT( r < m_nNodesInUse ); return m_pNodes[r].GetChildren() + i; };

        /// Returns the i'th child of a particular (non-leaf) node.  0 is the left
        inline ConstNodeHandle GetLeftChild( ConstNodeHandle r ) const { TRT_ASSERT( r < m_nNodesInUse ); return m_pNodes[r].GetChildren(); };

        /// Returns the i'th child of a particular (non-leaf) node.  0 is the left
        inline ConstNodeHandle GetRightChild( ConstNodeHandle r ) const { TRT_ASSERT( r < m_nNodesInUse ); return m_pNodes[r].GetChildren() + 1; };

        /// Returns the number of children of a particular node (always 0 or 2 for a KD tree)
        inline size_t GetChildCount( ConstNodeHandle r ) const            { TRT_ASSERT( r < m_nNodesInUse ); return ( m_pNodes[r].IsLeaf() ? 0 : 2 ) ; };
        
        /// Tests whether or not the given node is a leaf
        inline bool IsNodeLeaf( ConstNodeHandle r ) const                 { TRT_ASSERT( r < m_nNodesInUse ); return m_pNodes[r].IsLeaf(); };
        
        /// Returns the number of objects in a particular node
        inline size_t GetNodeObjectCount( ConstNodeHandle r ) const          { TRT_ASSERT( r < m_nNodesInUse ); return m_pNodes[r].GetObjectCount(); };
        


        /// Returns the root bounding box of the tree
        inline const AxisAlignedBox& GetBoundingBox() const { return m_aabb; };

        /// Returns the maximum depth of any node in the tree.  This is used to allocate stack space during traversal
        inline size_t GetStackDepth() const { return m_nStackDepth; };    
    
    
        /// Returns iterators over a leaf node's object list (may not be called on inner nodes)
        inline void GetNodeObjectList( ConstNodeHandle n, LeafIterator& rBegin, LeafIterator& rEnd ) const { 
            TRT_ASSERT( n < m_nNodesInUse &&IsNodeLeaf(n) );
            rBegin = &m_pObjectRefs[m_pNodes[n].GetFirstObject()];
            rEnd = rBegin + m_pNodes[n].GetObjectCount();
        };

        /// Returns the split axis (0,1,2 for a particular node)
        inline uint GetNodeSplitAxis( ConstNodeHandle r ) const { 
            TRT_ASSERT( r < m_nNodesInUse && !m_pNodes[r].IsLeaf() );
            return m_pNodes[r].GetSplitAxis(); 
        };

        /// Returns the split plane location for a particular node
        inline float GetNodeSplitPosition( ConstNodeHandle r ) const { 
            TRT_ASSERT( r < m_nNodesInUse && !m_pNodes[r].IsLeaf() );    
            return m_pNodes[r].GetSplitPosition(); 
        };

        /// Clears existing nodes in the tree and creates a new, single-node tree with the specified bounding box, returns the root node
        inline NodeHandle Initialize( const AxisAlignedBox& rRootAABB );

        /// Subdivides a particular node, creating two empty leaf children for it
        inline std::pair<NodeHandle,NodeHandle> MakeInnerNode( NodeHandle hNode, float fSplitPlane, uint nSplitAxis );

        /// Turns the specified node into a leaf containing the given set of object references.  Returns an object ref array which the caller must fill
        inline obj_id* MakeLeafNode( NodeHandle n, obj_id nObjCount ) ;

        /// Constructs a new KD tree
        template< class KDTreeBuilder_T >
        inline void Build( ObjectSet_T* pObjects, KDTreeBuilder_T& rBuilder );

        /// Returns the memory usage of the tree
        inline void GetMemoryUsage( size_t& rnBytesUsed, size_t& rnBytesAllocated ) const
        {
            rnBytesUsed = sizeof(KDTree) + m_nNodesInUse*sizeof(Node) + m_nObjectRefsInUse*sizeof(obj_id);
            rnBytesAllocated = sizeof(KDTree) + m_nObjectRefArraySize*sizeof(obj_id) + m_nNodeArraySize*sizeof(Node);
        }


        /// Sanity-checks the tree
        inline bool Validate( );

        /// Provides direct access to the nodes
        inline const Node* GetNodes() const { return &m_pNodes[0]; };

        /// Provides direct access to the object references
        inline const obj_id* GetObjectRefs() const { return &m_pObjectRefs[0]; };

        /// Returns the number of nodes
        inline size_t GetNodeCount() const { return m_nNodesInUse; };

        /// Returns the number of object references
        inline size_t GetObjectRefCount() const { return m_nObjectRefsInUse; };


    private:

        AxisAlignedBox m_aabb;
        size_t m_nStackDepth;

        ScopedArray<Node> m_pNodes;
        size_t m_nNodesInUse;
        size_t m_nNodeArraySize;

        ScopedArray<obj_id> m_pObjectRefs;
        size_t m_nObjectRefsInUse;
        size_t m_nObjectRefArraySize;

    };
    
}

#include "TRTKDTree.inl"

#endif // _TRTKDTREE_H_
