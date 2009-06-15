//=====================================================================================================================
//
//   TRTInstancing.h
//
//   Example illustrating instanced mesh rendering using TinyRT.  In this example we demonstrate instanced meshes,
//     with a seperate BVH tree built for each unique mesh.  The instances are nested inside a uniform grid.
//
//   These data structures were chosen to illustrate the ease with which various raytracing algorithms may be
//     composed together using TinyRT.
//   
//
//   Part of the TinyRT Raytracing Library.
//   Author: Joshua Barczak
//
//   Copyright 2009 Joshua Barczak.  All rights reserved.
//   See  Doc/LICENSE.txt for terms and conditions.
//
//=====================================================================================================================


#include "PPMImage.h"
#include "PlyLoad.h"
#include "TinyRT.h"
using namespace TinyRT;


typedef TinyRT::BasicMesh<Vec3f,unsigned int> Mesh;


// Some mesh data
struct MeshData
{
    std::vector< TinyRT::Vec3f > vertices;
    std::vector< unsigned int > indices;
    AxisAlignedBox meshAABB;
    TinyRT::AABBTree<Mesh> Tree;  // Tree built over the mesh

};

class MeshInstance;

// Information about a triangle/ray intersection
struct InstanceHitInfo
{
    const MeshInstance* pMeshInstance;
    TinyRT::TriangleRayHit triangleHit;
};


// An instance of a mesh
class MeshInstance
{
public:

    MeshInstance( MeshData& rData, const Vec3f& rPosition )
        : m_Mesh( &rData.vertices[0], &rData.indices[0], rData.vertices.size(), rData.indices.size()/3 ),
          m_aabb( rData.meshAABB.Min() + rPosition, rData.meshAABB.Max() + rPosition ),
          m_vPosition( rPosition ),
          m_pMeshData( &rData )
    {
    }

    void RayIntersect( TinyRT::Ray& rRay, InstanceHitInfo& rHitInfo ) const
    {
        // transform the ray
        TinyRT::Ray transformedRay( rRay );
        transformedRay.SetOrigin( rRay.Origin() - m_vPosition );

        TinyRT::TriangleRayHit hit;
        hit.nTriIdx = m_Mesh.GetObjectCount();

        // cast ray at mesh instance
        static TinyRT::ScratchMemory ScratchMemory;
        TinyRT::RaycastBVH( &m_pMeshData->Tree, 
                            &m_Mesh, 
                            transformedRay, 
                            hit, 
                            m_pMeshData->Tree.GetRoot(), 
                            ScratchMemory );

        if( hit.nTriIdx != m_Mesh.GetObjectCount() )
        {
            // we hit something in this instance, update hit info record
            rHitInfo.pMeshInstance = this;
            rHitInfo.triangleHit = hit;

            // update maximum distance in parent ray
            rRay.SetMaxDistance( transformedRay.MaxDistance() );
        }
    }
        
    inline const AxisAlignedBox& GetAABB() const { return m_aabb; };
    inline const Mesh* GetMesh() const { return &m_Mesh; };
        
private:

    MeshData* m_pMeshData;
    Mesh m_Mesh;                        // The mesh
    AxisAlignedBox m_aabb;              // AABB of the mesh instance (in world space)
    Vec3f m_vPosition;                  // for simplicity, this sample assumes only translation, but a full matrix is certainly doable
};


// A class encapsulating the set of instance mesh objects.  This class implements the TinyRT::ObjectSet_C concept.
class InstancedMeshObjectSet
{
public:

    typedef TinyRT::uint32 obj_id;

    InstancedMeshObjectSet( MeshInstance** pInstances, uint nInstances ) 
        : m_pInstances(pInstances), m_nInstances(nInstances)
    {
        m_grid.Build( this, 50.0f );
    }

    void RayIntersect( TinyRT::Ray& rRay, InstanceHitInfo& rHitInfo, obj_id nObject ) const
    {
        m_pInstances[nObject]->RayIntersect( rRay, rHitInfo );
    }

    void RayIntersect( TinyRT::Ray& rRay, InstanceHitInfo& rHitInfo, obj_id nObject, obj_id nCount ) const
    {
        for( obj_id i=0; i<nCount; i++ )
            RayIntersect( rRay, rHitInfo, nObject+i );
    }

    inline void RemapObjects( obj_id* pObjectRemap )
    {
        TinyRT::RemapArray( m_pInstances, m_nInstances, pObjectRemap );
    }

    void GetAABB( TinyRT::AxisAlignedBox& rBox )
    {
        if( m_nInstances > 0 )
        {
            rBox = m_pInstances[0]->GetAABB();
            for( uint i=1; i<m_nInstances; i++ )
            {
                rBox.Merge( m_pInstances[i]->GetAABB() );
            }
        }
    }

    inline void GetObjectAABB( obj_id nObject, TinyRT::AxisAlignedBox& rBoxOut )
    {
        rBoxOut = m_pInstances[nObject]->GetAABB();
    }

    inline obj_id GetObjectCount() const
    {
        return m_nInstances;
    }

    void CastRay( TinyRT::Ray& rRay, InstanceHitInfo& rRayHit ) const
    {
        TinyRT::RaycastUniformGrid< TinyRT::DirectMapMailbox<obj_id,8> >(  &m_grid, this, rRay, rRayHit );
    }

private:

    MeshInstance** m_pInstances;
    uint m_nInstances;

    TinyRT::UniformGrid< InstancedMeshObjectSet > m_grid;
};



void Render( const TinyRT::Vec3f& rPosition, const TinyRT::Vec3f& rLookAt, PPMImage& rImage, const InstancedMeshObjectSet& rObjects )
{
    PPMImage image;
    TinyRT::PerspectiveCamera cam( rPosition, rLookAt-rPosition, Vec3f(0,1,0), 60.0f, 1 );

    int nHeight = rImage.GetHeight();
    int nWidth  = rImage.GetWidth();
    
    for( int y = 0; y<nHeight; y++ )
    {
        printf("%u\r", y );
        for( int x = 0; x < nWidth; x++  )
        {
            float s = (float) x / (float) nWidth;
            float t = (float) y / (float) nHeight;

            const TinyRT::Vec3f& vOrigin = cam.GetPosition();
            TinyRT::Vec3f vDir = cam.GetRayDirectionNDC( TinyRT::Vec2f(s,t) );
            TinyRT::Ray ray( vOrigin, vDir );

            InstanceHitInfo hitInfo;
            hitInfo.pMeshInstance = 0;
            hitInfo.triangleHit.nTriIdx = 0xffffffff;
            hitInfo.triangleHit.vUVCoords = Vec2f(0,0);

            rObjects.CastRay( ray, hitInfo );

            if( !hitInfo.pMeshInstance )
            {
                rImage.SetPixel( x,y, 0,0,0 );
            }
            else
            {
                // compute face normal and do N.V shading
                const Mesh* pMesh = hitInfo.pMeshInstance->GetMesh();
                const TinyRT::Vec3f& v0 = pMesh->VertexPosition( pMesh->Index( hitInfo.triangleHit.nTriIdx, 0 ) );
                const TinyRT::Vec3f& v1 = pMesh->VertexPosition( pMesh->Index( hitInfo.triangleHit.nTriIdx, 1 ) );
                const TinyRT::Vec3f& v2 = pMesh->VertexPosition( pMesh->Index( hitInfo.triangleHit.nTriIdx, 2 ) );
                const TinyRT::Vec3f& vNormal = TinyRT::Normalize3( TinyRT::Cross3( v2-v0, v1-v0 ) );
                
                TinyRT::Vec3f vHit = vOrigin + vDir*ray.MaxDistance();
                TinyRT::Vec3f vV = TinyRT::Normalize3( vHit - vOrigin );

                float f = fabs( TinyRT::Dot3( vV, vNormal ) );
                rImage.SetPixel( x, y, f,f,f );
            }
        }
    }
}


bool LoadMesh( const char* pFileName, MeshData& rMeshData )
{
    if( !LoadPlyMesh( pFileName, rMeshData.vertices, rMeshData.indices, true ) )
        return false;

    TinyRT::SahAABBTreeBuilder< Mesh > TreeBuilder( 0.7f );
    Mesh tmpMesh( &rMeshData.vertices[0], &rMeshData.indices[0], rMeshData.vertices.size(), rMeshData.indices.size()/3 );
    
    rMeshData.Tree.Build( &tmpMesh, TreeBuilder );
    rMeshData.meshAABB.ComputeFromPoints( &rMeshData.vertices[0], rMeshData.vertices.size() );

    return true;
}

int main( )
{
    static const int MESH_TYPE_COUNT = 5;
    static const int INSTANCE_COUNT = 1000;
    const char* MESH_FILES[MESH_TYPE_COUNT] =
    {
        "..\\models\\bunny.ply",
        "..\\models\\horse.ply",
        "..\\models\\elephant.ply",        
        "..\\models\\toasters.ply",        
        "..\\models\\ben.ply",        
    
    };

    // load all meshes
    MeshData meshes[MESH_TYPE_COUNT];
    for( int i=0; i<MESH_TYPE_COUNT; i++ )
    {
        printf("Parsing mesh: '%s'\n", MESH_FILES[i] );
        if( !LoadMesh( MESH_FILES[i], meshes[i] ) )
        {
            printf("Failure loading mesh: '%s'\n", MESH_FILES[i] );
            return 1;
        }
    }

    // create mesh instances
    MeshInstance* pInstances[INSTANCE_COUNT];
    int nSize = (int) sqrt( (float) INSTANCE_COUNT );

    for( int i=0; i<INSTANCE_COUNT; i++ )
    {
        float x = rand() / (float)RAND_MAX;
        float y = rand() / (float)RAND_MAX;

        x *= nSize;
        y *= nSize;
        pInstances[i] = new MeshInstance( meshes[ rand()%MESH_TYPE_COUNT ], TinyRT::Vec3f(x,0,y) );
    }

    InstancedMeshObjectSet objects( &pInstances[0], INSTANCE_COUNT );

    // position the camera
    TinyRT::Vec3f vLookAt( nSize/2.0f, 0, nSize/2.0f );
    TinyRT::Vec3f vOrigin = vLookAt + Vec3f( 0, nSize/2.0f, (float) -nSize );
   
    // draw
    PPMImage image(512,512);
    Render( vOrigin, vLookAt, image, objects );
    image.SaveFile( "out.ppm");

    return 0;
}
