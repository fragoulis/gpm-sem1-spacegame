#include "Spacestation.h"
#include "SpacestationDisplayList.h"
#include "SingleTexture.h"
#include "SimpleMaterial.h"
#include "CollisionBBox.h"
#include "Shader.h"
#include "Logger.h"
#include "Config.h"

void Spacestation::init()
{
    int iTileSize;
    int iNumOfTiles;
    string sTexture;

    getPos().xyz( 0.0f, 0.0f, 0.0f );

    Config cfg("config.txt");
    cfg.loadBlock("spacestation");

    cfg.getInt("tile_size"  ,&iTileSize);
    cfg.getInt("tiles"      ,&iNumOfTiles);
    cfg.getString("texture", sTexture);

    // The bounding box half size sides
    float fBBSide = iTileSize * iNumOfTiles * 0.5f;

    _LOG("Setting up spacestation...");

    // Initialize material component
    OCSimpleMaterial *cMat = new OCSimpleMaterial;
    cMat->setEmission( Color(0.7f,0.7f,0.7f,1.0f) );
    setComponent( cMat );

    // Initialize presetation component
    setComponent( new OCSingleTexture( sTexture.c_str() ) );
    
    // Initialize visual component
    setComponent( new SpacestationDisplayList );

    // Initialize collision component
    setComponent( new OCCollisionBBox( Vector3f(fBBSide,fBBSide,fBBSide) ) );
}