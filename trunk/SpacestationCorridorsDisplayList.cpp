
#include <windows.h>
#include <gl/gl.h>
#include <fstream>
#include <assert.h>
#include "SpacestationCorridorsDisplayList.h"
#include "Logger.h"
#include "Config.h"

using std::ifstream;
using tlib::Logger;

SpacestationCorridorsDisplayList::SpacestationCorridorsDisplayList()
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

SpacestationCorridorsDisplayList::~SpacestationCorridorsDisplayList()
{
    // Iterate and delete all tiles
    _Tilemap::iterator iter;
    for( iter = m_Tilemap.begin(); iter != m_Tilemap.end(); iter++ )
    {
        delete iter->second;
        iter->second = 0;
    }
    m_Tilemap.clear();
}

bool SpacestationCorridorsDisplayList::readMap( const char *filename )
{
    ifstream fin( filename );
    if( !fin ) 
        return false;
    
    _LOG("Reading filemap " + string(filename) );

    // Read info for every tile in the mapfile
    // We are kind of forced to use a new for every tile
    int i, j, k, corr_type, obj_type;
    while( fin >> i >> j >> k >> corr_type >> obj_type )
    {
        // Tile's index must be between our boundaries
        if( i<0 || i>=m_iNumOfTiles || 
            j<0 || j>=m_iNumOfTiles ||
            k<0 || k>=m_iNumOfTiles ||
            corr_type == 0 ) {
            _LOG("Invalid tile on line " + toStr<size_t>(m_Tilemap.size()-1) );
            continue;
        }
            
        // Create tile and push it to the list
        int index = i + 
                    j * m_iNumOfTiles + 
                    k * m_iNumOfTiles * m_iNumOfTiles;
        m_Tilemap[ index ] = new Tile3d( i, j, k, corr_type, obj_type );
    }
    _LOG("Counted " + toStr<size_t>(m_Tilemap.size()) + " tiles");
    
    return true;
    
} // end readMap()

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

            if( oTile->corrType & TW_FRONT )
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

            if( oTile->corrType & TW_BACK )
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

            if( oTile->corrType & TW_LEFT )
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

            if( oTile->corrType & TW_RIGHT )
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

            if( oTile->corrType & TW_TOP )
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

            if( oTile->corrType & TW_BOTTOM )
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