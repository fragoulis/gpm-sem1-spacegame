#include "Spacestation.h"
#include "SpacestationDisplayList.h"
#include "Texture.h"
#include "CollisionBBox.h"
#include "Logger.h"
#include "Config.h"

void Spacestation::setup()
{
    int iTileSize;
    int iNumOfTiles;
    string sTexture;

    Config cfg("config.txt");
    cfg.loadBlock("spacestation");

    cfg.getInt("tile_size"  ,&iTileSize);
    cfg.getInt("tiles"      ,&iNumOfTiles);
    cfg.getString("texture", sTexture);

    // The bounding box half size sides
    float fBBSide = iTileSize * iNumOfTiles * 0.5f;

    _LOG("Setting up spacestation...");
    // Initialize visual component
    setComponent( new SpacestationDisplayList );      
    // Initialize presetation component
    setComponent( new OCTexture( sTexture.c_str() ) );
    // Initialize collision component
    setComponent( new OCCollisionBBox( Vector3f(fBBSide,fBBSide,fBBSide) ) );
}