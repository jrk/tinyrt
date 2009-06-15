//=====================================================================================================================
//
//   TRTMultiBVHTraversal.h
//
//   Single-ray traversal through multi-branching BVH data structures
//
//   Part of the TinyRT Raytracing Library.
//   Author: Joshua Barczak
//
//   Copyright 2008 Joshua Barczak.  All rights reserved.
//   See  Doc/LICENSE.txt for terms and conditions.
//
//=====================================================================================================================

#ifndef _TRT_MULTIBVHTRAVERSAL_H_
#define _TRT_MULTIBVHTRAVERSAL_H_


namespace TinyRT
{
    //=====================================================================================================================
    /// \ingroup TinyRT
    /// \brief Searches for the first intersection between a ray and an object in an N-ary BVH
    /// \param MBVH_T       Must implement MBVH_C
    /// \param ObjectSet_T  Must implement ObjectSet_C
    /// \param HitInfo_T    Must implement HitInfo_C
    /// \param Ray_T        Must implement Ray_C
    //=====================================================================================================================
    template< typename MBVH_T, typename ObjectSet_T, typename HitInfo_T, typename Ray_T >
    void RaycastMultiBVH( const MBVH_T* pBVH, const ObjectSet_T* pObjects, Ray_T& rRay, HitInfo_T& rHitInfo, const typename MBVH_T::ConstNodeHandle pRoot, ScratchMemory& rScratch )
    {
        const Vec3f& rDir = rRay.Direction();
        int nDirSigns[4] = {
            rDir.x > 0 ? 0 : 1,
            rDir.y > 0 ? 0 : 1,
            rDir.z > 0 ? 0 : 1
        };
        nDirSigns[3] =  (nDirSigns[2] << 2) | (nDirSigns[1] << 1) | nDirSigns[0];


        size_t nStackSize = pBVH->GetStackDepth()*(MBVH_T::BRANCH_FACTOR);
        ScratchArray<MBVH_T::ConstNodeHandle> pStackMem( rScratch, nStackSize );
        MBVH_T::ConstNodeHandle* pStack = pStackMem;
        const MBVH_T::ConstNodeHandle* pStackBottom = pStack;
        (*pStack++) = pRoot;


        while( pStack != pStackBottom )
        {
            pStack--;
            MBVH_T::ConstNodeHandle pNode = *pStack;

            if( pBVH->IsNodeLeaf( pNode ) )
            {
                MBVH_T::obj_id nFirstObject;
                MBVH_T::obj_id nLastObject;
                pBVH->GetNodeObjectRange( pNode, nFirstObject, nLastObject );
                
                pObjects->RayIntersect( rRay, rHitInfo, nFirstObject, nLastObject );
            }
            else
            {
                pStack = pBVH->RayIntersectChildren( pNode, rRay, pStack, nDirSigns );
            }
        }
    }

}

#endif // _TRT_MULTIBVHTRAVERSAL_H_
