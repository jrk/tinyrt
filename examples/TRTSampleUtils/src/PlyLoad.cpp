

#include "rply.h"  // ply parsing library, courtesy of diego

#include "TinyRT.h"
using namespace TinyRT;
typedef TinyRT::uint32 UINT;

#include <assert.h>
#include <string>
#include <float.h>


static Vec3f* s_pVertices = 0;

static UINT* s_nStripIndices = 0;   // used if the mesh is stored as triangle strips
UINT s_nStripLength = 0;            // used if mesh is stored as a triangle list

std::vector<UINT> s_triList;

std::vector<UINT> s_faceColors;

int VertexCallback( p_ply_argument  argument )
{
    // figure out which vertex component this is
    long nVertexComponent;
    ply_get_argument_user_data( argument, 0, &nVertexComponent );

    // figure out this vertex's index
    long nIndex;
    ply_get_argument_element( argument, NULL, &nIndex );
    
    // get the value
    double value = ply_get_argument_value( argument );
    
    // write it
    s_pVertices[ nIndex ][ nVertexComponent ] = (float) value;

    return 1;
}


int FaceCallback( p_ply_argument argument )
{
    long length, value_index;
    ply_get_argument_property(argument, NULL, &length, &value_index);

    // this is the beginning of the list
    // the meshes I'm trying to deal with are stored as triangle strips for some asinine reason
    if( value_index == -1 )
    {
        // allocate strip array now
        s_nStripIndices = new UINT[ length ];
        s_nStripLength = length;
    }
    else
    {
        s_nStripIndices[ value_index ] = (UINT) ply_get_argument_value( argument );
    }

    return 1;
}

int FaceListCallback( p_ply_argument argument )
{
    long length, value_index;
    ply_get_argument_property(argument, NULL, &length, &value_index);

    // this is the beginning of the list
    if( value_index != -1 )
    {
        s_triList.push_back( (UINT) ply_get_argument_value( argument ) );
    }

    return 1;
}

int ColorCallback( p_ply_argument  argument )
{
    // figure out which vertex component this is
    long nColorComponent;
    ply_get_argument_user_data( argument, 0, &nColorComponent );

    // figure out this vertex's index
    long nIndex;
    ply_get_argument_element( argument, NULL, &nIndex );
    
    // get the value
    double value = ply_get_argument_value( argument );
    UINT nValue = (UINT) value;

    // write it
    if( nIndex >= (long) s_faceColors.size() )
        s_faceColors.resize( nIndex+1 );


    UINT nMask = 0xff << 8*( nColorComponent+1 );
    s_faceColors[ nIndex ] &= (~nMask);   // clear bits for this color channel (make sure they're initialized to 0)
    s_faceColors[ nIndex ] |= ( nValue << 8*(nColorComponent+1) ) & nMask; // set bits for this color channel

    return 1;
}


void FlattenStrips( std::vector<UINT>& triList )
{
    triList.push_back( s_nStripIndices[0] );
    triList.push_back( s_nStripIndices[1] );
    triList.push_back( s_nStripIndices[2] );

    UINT nFaces=1;
    for( UINT i=3; i<s_nStripLength; i++ )
    {
        if( s_nStripIndices[i] == 0xffffffff )
        {
            // cut strip
            i++;
            if( i < s_nStripLength )
            {
                triList.push_back( s_nStripIndices[i] );
                triList.push_back( s_nStripIndices[i+1] );
                triList.push_back( s_nStripIndices[i+2] );
                i+= 2; // the loop adds one more to i
            
                nFaces=1;
            }
        }
        else
        {
            // to get normals to be sane, we need to flip face winding every other face
            if( nFaces % 2 )
            {
                // odd
                triList.push_back( s_nStripIndices[ i-2 ] );
                triList.push_back( s_nStripIndices[ i ] );
                triList.push_back( s_nStripIndices[ i-1 ] );
            }
            else
            {
                // even
                triList.push_back( s_nStripIndices[ i-2 ] );
                triList.push_back( s_nStripIndices[ i-1 ] );
                triList.push_back( s_nStripIndices[ i ] );
            }

            nFaces++;
        }
                
    }
}



// convert the mesh vertices into the form that Josh likes
void FixVertices( Vec3f* pVertices, UINT nVertices )
{
    if( nVertices == 0 )
        return;

    // compute mesh bounding box
    TinyRT::AxisAlignedBox box( pVertices, nVertices );
    TinyRT::Vec3f& minPt = box.Min();
    TinyRT::Vec3f& maxPt = box.Max();

    // push vertices up so that lowest point of mesh is at y=0
    float dy = -minPt.y;
    for( UINT i=0; i<nVertices; i++ )
    {
        pVertices[i].y += dy;
    }
    minPt.y=0;
    maxPt.y += dy;

    // center X and Z at 0
    Vec3f center = (maxPt + minPt) / 2.0f;
    for( UINT i=0; i<nVertices; i++ )
    {
        pVertices[i].x -= center.x;
        pVertices[i].y -= center.z;
    }

    // normalize vertices so that they fit in a unit-length bounding box
    Vec3f size = maxPt-minPt;
    UINT nMaxComp=0;
    for( UINT i=0; i<3; i++ )
    {
        if( size[i] > size[nMaxComp] )
            nMaxComp = i;
    }

    for( UINT i=0; i<nVertices; i++ )
        pVertices[i] /= size[nMaxComp];
}


bool LoadPlyMesh( const char* pMeshFileName, std::vector<Vec3f>& rVertices, std::vector<UINT>& rIndices, bool bFix )
{
    p_ply ply = ply_open(pMeshFileName, NULL);
    
    if (!ply) 
        return false;

    if (!ply_read_header(ply)) 
        return false;

    UINT nvertices = ply_set_read_cb(ply, "vertex", "x", &VertexCallback, NULL, 0);
    ply_set_read_cb( ply, "vertex", "y", &VertexCallback, NULL, 1);
    ply_set_read_cb( ply, "vertex", "z", &VertexCallback, NULL, 2);
    s_pVertices = new Vec3f[ nvertices ];

    ply_set_read_cb(ply, "tristrips", "vertex_indices", FaceCallback, NULL, 0);
    ply_set_read_cb(ply, "face", "vertex_indices", FaceListCallback, NULL, 0 );

    bool ok = ply_read(ply) != 0;
    ply_close(ply);

    if( ok )
    {
        // convert tri strips to tri lists (the one true mesh representation)
        std::vector<uint32> triList;

        if( s_nStripIndices )
        {
            FlattenStrips( triList );
        }
        else
        {
            triList.insert( triList.begin(), s_triList.begin(), s_triList.end() );
        }
        
        rVertices.reserve( nvertices );
        for( UINT i=0; i<nvertices; i++ )
            rVertices.push_back( Vec3f( s_pVertices[i] ) );
        
        rIndices = triList;
    }
    
    delete[] s_pVertices;
    delete[] s_nStripIndices;
    s_nStripIndices=0;
    s_pVertices=0;
    s_triList.clear();
    s_nStripLength = 0;


    if( bFix )
        FixVertices( &rVertices[0], rVertices.size() );

    return ok;
}