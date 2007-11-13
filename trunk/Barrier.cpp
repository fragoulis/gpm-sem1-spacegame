#include "Barrier.h"
#include "CollisionBBox.h"
#include "Config.h"
using tlib::OCCollisionBBox;
using tlib::Config;

Barrier::Barrier() {}
Barrier::Barrier( const Vector3f& vBBox )
{
    // Initialize a bounding box for all barriers
    setComponent( new OCCollisionBBox( vBBox ) );
}

void Barrier::setPosFromIndex( int ijk[] )
{
    // Open the ocnfiguration file to read the spacestation's 
    // number of tiles and tile size
    Config cfg("config.txt");
    cfg.loadBlock("spacestation");

    int iTileSize, iNumOfTiles;
    cfg.getInt("tile_size"  ,&iTileSize);
    cfg.getInt("tiles"      ,&iNumOfTiles);

    float fHalfNumOfTiles = (float)iNumOfTiles * 0.5f;

    // Setup the barriers position
    // The next gives us the position od the barrier and 
    // with some operations becomes simpler
    // [ iTileSize * i - ( iTileSize * iNumOfTiles / 2 ) + iTileSize / 2 ]
    m_vPos.xyz( 
        float(( ijk[0] - fHalfNumOfTiles + 0.5f ) * iTileSize),
        float(( ijk[1] - fHalfNumOfTiles + 0.5f ) * iTileSize),
        float(( -ijk[2] + fHalfNumOfTiles - 0.5f ) * iTileSize)
        );

} // end setPosFromIndex()