#include <windows.h>
#include <gl/gl.h>
#include <fstream>
#include <assert.h>
#include "SpacestationCorridorsDisplayList.h"
#include "Tilemap.h"
#include "Outlet.h"
#include "Logger.h"
#include "Config.h"

using std::ifstream;
using tlib::Logger;

SpacestationCorridorsDisplayList::SpacestationCorridorsDisplayList()
{
    _LOG("Setting up spacestation corridors display list...");
    
    // Read mapfile location from configuration file
    Config cfg("config.txt");
    cfg.loadBlock("spacestation");

    string filename;
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
{}

// ----------------------------------------------------------------------------
bool SpacestationCorridorsDisplayList::readMap( const char *filename )
{
    ifstream fin( filename );
    if( !fin ) 
        return false;
    
    _LOG("Reading filemap " + string(filename) );

    // Read every line to get a hold of the number of tiles
    const int iNumOfTiles = Tilemap::Instance().getNumOfTiles();
    int 
        iNumOfLines = 0,
        i, j, k, corr_type, obj_type, friend_id;
    while( fin >> i >> j >> k >> corr_type >> obj_type >> friend_id )
    {
        // Tile's index must be between our boundaries
        if( i<0 || i>=iNumOfTiles || 
            j<0 || j>=iNumOfTiles ||
            k<0 || k>=iNumOfTiles ||
            corr_type == 0 ) 
        {
            _LOG( "Invalid tile on line " + 
                  toStr<size_t>(Tilemap::Instance().getTileArray().size()-1) );

            continue;
        }

        iNumOfLines++;
    }

    _LOG("Counted " + toStr<size_t>(iNumOfLines) + " tiles");

    // Return at the start of the file
    fin.clear();
    fin.seekg( 0, ios_base::beg );

    // Allocate continuous memory for all tiles
    //m_TileList = new Tile3d[ iNumOfLines ];
    Tilemap::Instance().create( iNumOfLines );

    // Read info for every tile in the mapfile
    // We prefer to use a start to end for here sine we already have 
    // the end of the array :|
    for( int iCurrentTile = 0; iCurrentTile<iNumOfLines; ++iCurrentTile )
    {
        // Read tile attibutes
        fin >> i >> j >> k >> corr_type >> obj_type >> friend_id;
        
        // Create tile and push it to the list
        int index = i + 
                    j * iNumOfTiles + 
                    k * iNumOfTiles * iNumOfTiles;

        // Setup tile
        Tile3d *tile = Tilemap::Instance().getTileByIndex( iCurrentTile );
        tile->ijk( i, j, k );
        tile->setType( corr_type );
        tile->setFriendId( friend_id );
        tile->addObject( obj_type );

        // Save address to associative map
        Tilemap::Instance().setTile( index, iCurrentTile );
    }
    
    // Third and final pass... a little quicker this time
    // We have to link outlets to barriers. To do that we pass the address
    // of the occupant object of the tile with index the same as the friend id
    // of this tile's occupant barrier pointer :)))
    for( int iCurrentTile = 0; iCurrentTile<iNumOfLines; ++iCurrentTile )
    {
        Tile3d *tile = Tilemap::Instance().getTileByIndex(iCurrentTile);

        // If friend id is zero, then tile is not an outlet
        if( !tile->getFriendId() ) continue;
        
        // ---- Associate power outlet with a barrier
        // Get this tile's occupier, which by now we know is an Outlet
        Outlet *oOL = (Outlet*)tile->getOccupant();

        // This tile's friend id is the index of the tile which holds the 
        // outlet's barrier
        int index = tile->getFriendId();

        // Get the occupier of that tile
        Object *oOccupant = Tilemap::Instance().getTile(index)->getOccupant();

        // And pass it as this outlet's barrier
        oOL->setBarrier( (Barrier*)oOccupant );
    }

    return true;
    
} // end readMap()

// ----------------------------------------------------------------------------
void SpacestationCorridorsDisplayList::buildObject() const 
{
    _LOG("Building spacestation corridors object");

    const int iTileSize = Tilemap::Instance().getTileSize();
    const float halfFace = (float)(iTileSize * 
                                   Tilemap::Instance().getNumOfTiles()) * 0.5f;

    // Draw the dummy corridor paths
    glBegin(GL_QUADS);
    {
        float 
            // the lower, back, left corner of the tile
            startX, startY, startZ, 
            // the upper, front, right corner of the tile
            endX, endY, endZ,
            nTileReapeat = 0.5f;

        TileArray::const_iterator iter;
        for( iter = Tilemap::Instance().getTileArray().begin(); 
             iter != Tilemap::Instance().getTileArray().end(); 
             ++iter )
        {
            // Shorthand tile
            Tile3d *oTile = iter->second;

            // Assign a starting point
            startX = -halfFace + iTileSize * oTile->i();
            startY = -halfFace + iTileSize * oTile->j();
            startZ =  halfFace - iTileSize * oTile->k();
            endX = startX + iTileSize;
            endY = startY + iTileSize;
            endZ = startZ - iTileSize;

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