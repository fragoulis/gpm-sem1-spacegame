#include "Tilemap.h"
#include "Config.h"
#include "Logger.h"
using tlib::Logger;
using tlib::Config;

Tilemap::Tilemap():
m_TileArray(0)
{
    // Read tile size and number of tiles per side from the configuration file
    Config cfg("config.txt");
    cfg.loadBlock("spacestation");

    cfg.getInt("tile_size"  ,&m_iTileSize);
    cfg.getInt("tiles"      ,&m_iNumOfTiles);
}

Tilemap::~Tilemap()
{
    if( m_TileArray ) {
        delete [] m_TileArray;
        m_TileArray = 0;
        _LOG("Deleted tilemap array");
    }
    m_TileMap.clear();
}

// ----------------------------------------------------------------------------
void Tilemap::create( int iNumOfTiles ) {
    _ASSERT(iNumOfTiles>0);

    // Make sure that this is only initialized once
    if( !m_TileArray ) {
        m_TileArray = new Tile3d[ iNumOfTiles ];
        m_iTileArraySize = iNumOfTiles;
    }
}

// ----------------------------------------------------------------------------
Vector3f Tilemap::getTilePos( const Tile3d* oTile ) const
{
    const float fHalfTiles = (float)m_iNumOfTiles * 0.5f;

    Vector3f vIndex( 
        oTile->i() - fHalfTiles,
        oTile->j() - fHalfTiles,
        oTile->k() + fHalfTiles
        );
    
    return Vector3f(vIndex * (float)m_iTileSize);
}

// ----------------------------------------------------------------------------
Tile3d* Tilemap::getTile( const Vector3f& vIn ) const
{ 
    const float fHalfTiles = (float)m_iNumOfTiles * 0.5f;

    // Convert position vector to 'tile' coordinates
    Vector3f vIndex = vIn / (float)m_iTileSize;
    vIndex.x( vIndex.x() + fHalfTiles );
    vIndex.y( vIndex.y() + fHalfTiles );
    vIndex.z( vIndex.z() - fHalfTiles );
    
    return getTile( (int)floor((float)vIndex.x()), 
                    (int)floor((float)vIndex.y()),
                    (int)floor(-(float)vIndex.z()) );
}