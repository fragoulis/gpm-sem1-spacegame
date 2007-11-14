
#include <windows.h>
#include <gl/gl.h>
#include <fstream>
#include <assert.h>
#include "SpacestationCorridorsDisplayList.h"
#include "Outlet.h"
#include "Logger.h"
#include "Config.h"

using std::ifstream;
using tlib::Logger;

SpacestationCorridorsDisplayList::SpacestationCorridorsDisplayList():
    m_TileList(0)
{
    _LOG("Setting up spacestation corridors display list...");
    
    Config cfg("config.txt");
    cfg.loadBlock("spacestation");

    string filename;
    cfg.getInt("tile_size"  ,&m_iTileSize);
    cfg.getInt("tiles"      ,&m_iNumOfTiles);
    cfg.getString("mapfile" ,filename);

    // Read map from file
    if( !readMap( filename.c_str() ) ) {
        _LOG("Failed to read spacestation corridors map...");
    } else {
        // Call superclass function to generate the display list
        generate();
    }
}

// ----------------------------------------------------------------------------
SpacestationCorridorsDisplayList::~SpacestationCorridorsDisplayList()
{
    // Iterate and delete all tiles
    /*_Tilemap::iterator iter;
    for( iter = m_Tilemap.begin(); iter != m_Tilemap.end(); iter++ )
    {
        delete iter->second;
        iter->second = 0;
    }*/
    if( m_TileList )
        delete [] m_TileList;
    m_TileList = 0;
    m_Tilemap.clear();
}

// ----------------------------------------------------------------------------
bool SpacestationCorridorsDisplayList::readMap( const char *filename )
{
    ifstream fin( filename );
    if( !fin ) 
        return false;
    
    _LOG("Reading filemap " + string(filename) );

    // Read every line to get a hold of the number of tiles
    int iNumOfLines = 0;
    int i, j, k, corr_type, obj_type, friend_id;
    while( fin >> i >> j >> k >> corr_type >> obj_type >> friend_id )
    {
        // Tile's index must be between our boundaries
        if( i<0 || i>=m_iNumOfTiles || 
            j<0 || j>=m_iNumOfTiles ||
            k<0 || k>=m_iNumOfTiles ||
            corr_type == 0 ) {
            _LOG("Invalid tile on line " + toStr<size_t>(m_Tilemap.size()-1) );
            continue;
        }

        iNumOfLines++;
    }

    _LOG("Counted " + toStr<size_t>(iNumOfLines) + " tiles");

    // Return at the start of the file
    fin.clear();
    fin.seekg( 0, ios_base::beg );

    // Allocate continuous memory for all tiles
    m_TileList = new Tile3d[ iNumOfLines ];

    // Read info for every tile in the mapfile
    // We prefer to use a start to end for here sine we already have 
    // the end of the array :|
    for( int iCurrentTile = 0; iCurrentTile<iNumOfLines; ++iCurrentTile )
    {
        // Read tile attibutes
        fin >> i >> j >> k >> corr_type >> obj_type >> friend_id;
        
        // Create tile and push it to the list
        int index = i + 
                    j * m_iNumOfTiles + 
                    k * m_iNumOfTiles * m_iNumOfTiles;

        // Setup tile
        m_TileList[ iCurrentTile ].i = i;
        m_TileList[ iCurrentTile ].j = j;
        m_TileList[ iCurrentTile ].k = k;
        m_TileList[ iCurrentTile ].setType( corr_type );
        m_TileList[ iCurrentTile ].setFriendId( friend_id );
        m_TileList[ iCurrentTile ].addObject( obj_type );

        // Save address to associative map
        m_Tilemap[ index ] = &m_TileList[ iCurrentTile ];
    }
    
    // Third and final pass... a little quicker this time
    // We have to link outlets to barriers. To do that we pass the address
    // of the occupant object of the tile with index the same as the friend id
    // of to this tile's occupant barrier pointer :)))
    for( int iCurrentTile = 0; iCurrentTile<iNumOfLines; ++iCurrentTile )
    {
        // friend id is zero, then tile is not an outlet
        if( !m_TileList[iCurrentTile].getFriendId() ) continue;
        
        // Associate power outlet with a barrier
        Outlet *oOL = (Outlet*)m_TileList[iCurrentTile].getOccupant();
        int index = m_TileList[iCurrentTile].getFriendId();

        oOL->setBarrier( (Barrier*)m_Tilemap[index]->getOccupant() );
    }

    return true;
    
} // end readMap()

// ----------------------------------------------------------------------------
void SpacestationCorridorsDisplayList::buildObject() const 
{
    _LOG("Building spacestation corridors object");
    const float halfFace = (float)(m_iTileSize * m_iNumOfTiles) * 0.5f;
    // draw the dummy corridor paths
    glBegin(GL_QUADS);
    {
        float 
            // the lower, back, left corner of the tile
            startX, startY, startZ, 
            // the upper, front, right corner of the tile
            endX, endY, endZ,
            nTileReapeat = 0.5f;

        _Tilemap::const_iterator iter;
        for( iter = m_Tilemap.begin(); iter != m_Tilemap.end(); iter++ )
        {
            Tile3d *oTile = iter->second;

            // assign a starting point
            startX = -halfFace + m_iTileSize * oTile->i;
            startY = -halfFace + m_iTileSize * oTile->j;
            startZ =  halfFace - m_iTileSize * oTile->k;
            endX = startX + m_iTileSize;
            endY = startY + m_iTileSize;
            endZ = startZ - m_iTileSize;

            if( oTile->getType() & TW_FRONT )
            {
                // front face
                glNormal3f( 0.0f, 0.0f, 1.0f );
                glTexCoord2f( 0.0f, 0.0f );
                glVertex3f( startX, startY, endZ );
                glTexCoord2f( nTileReapeat, 0.0f );
                glVertex3f(   endX, startY, endZ );
                glTexCoord2f( nTileReapeat, nTileReapeat );
                glVertex3f(   endX,   endY, endZ );
                glTexCoord2f( 0.0f, nTileReapeat );
                glVertex3f( startX,   endY, endZ );
            } // end of front face

            if( oTile->getType() & TW_BACK )
            {
                // back face
                glNormal3f( 0.0f, 0.0f, -1.0f );
                glTexCoord2f( 0.0f, nTileReapeat );
                glVertex3f( startX,   endY, startZ );
                glTexCoord2f( nTileReapeat, nTileReapeat );
                glVertex3f(   endX,   endY, startZ );
                glTexCoord2f( nTileReapeat, 0.0f );
                glVertex3f(   endX, startY, startZ );
                glTexCoord2f( 0.0f, 0.0f );
                glVertex3f( startX, startY, startZ );
            } // end of back face

            if( oTile->getType() & TW_LEFT )
            {
                // left face
                glNormal3f( 1.0f, 0.0f, 0.0f );
                glTexCoord2f( 0.0f, 0.0f );
                glVertex3f( startX, startY, startZ );
                glTexCoord2f( 0.0f, nTileReapeat );
                glVertex3f( startX, startY, endZ );
                glTexCoord2f( nTileReapeat, nTileReapeat );
                glVertex3f( startX,   endY, endZ );
                glTexCoord2f( nTileReapeat, 0.0f );
                glVertex3f( startX,   endY, startZ );
            }

            if( oTile->getType() & TW_RIGHT )
            {
                // right face
                glNormal3f( -1.0f, 0.0f, 0.0f );
                glTexCoord2f( 0.0f, nTileReapeat );
                glVertex3f( endX, startY, endZ );
                glTexCoord2f( 0.0f, 0.0f );
                glVertex3f( endX, startY, startZ );
                glTexCoord2f( nTileReapeat, 0.0f );
                glVertex3f( endX,   endY, startZ );
                glTexCoord2f( nTileReapeat, nTileReapeat );
                glVertex3f( endX,   endY, endZ );
            }

            if( oTile->getType() & TW_TOP )
            {   
                // top face
                glNormal3f( 0.0f, -1.0f, 0.0f );
                glTexCoord2f( 0.0f, nTileReapeat );
                glVertex3f( startX, endY, endZ );
                glTexCoord2f( nTileReapeat, nTileReapeat );
                glVertex3f(   endX, endY, endZ );
                glTexCoord2f( nTileReapeat, 0.0f );
                glVertex3f(   endX, endY, startZ );
                glTexCoord2f( 0.0f, 0.0f );
                glVertex3f( startX, endY, startZ );
            }

            if( oTile->getType() & TW_BOTTOM )
            {    
                // bottom face
                glNormal3f( 0.0f, 1.0f, 0.0f );
                glTexCoord2f( 1.0f, 1.0f );
                glVertex3f(   endX, startY, endZ );
                glTexCoord2f( 0.5f, 1.0f );
                glVertex3f( startX, startY, endZ );
                glTexCoord2f( 0.50f, 0.5f );
                glVertex3f( startX, startY, startZ );
                glTexCoord2f( 1.0f, 0.5f );
                glVertex3f(   endX, startY, startZ );
            }
        } // end for( )
    }
    glEnd();

} // end buildObject()