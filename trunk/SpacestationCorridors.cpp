#include "SpacestationCorridors.h"
#include "SpacestationCorridorsDisplayList.h"
#include "SingleTexture.h"
#include "SimpleMaterial.h"
#include "Shader.h"
#include "Logger.h"
#include "Config.h"

SpacestationCorridors::SpacestationCorridors() {}

void SpacestationCorridors::init()
{
    _LOG("Setting up spacestation corridors...");

    getPos().xyz( 0.0f, 0.0f, 0.0f );

    // Read texture filename from configuration file
    string sTexture;
    Config cfg("config.txt");
    cfg.loadBlock("corridor");
    cfg.getString("texture" ,sTexture);

    // Initialize display list component
    setComponent( new SpacestationCorridorsDisplayList );     
    
    // Read corridor color attributes
    float vfAmbient[4], vfDiffuse[4], vfSpecular[4], fShine;
    cfg.getFloat("ambient",     vfAmbient,  4);
    cfg.getFloat("diffuse",     vfDiffuse,  4);
    cfg.getFloat("specular",    vfSpecular, 4);
    cfg.getFloat("shininess",   &fShine);

    // Initialize material component
    OCSimpleMaterial *cMat = new OCSimpleMaterial;
    cMat->setAmbient  ( Color( vfAmbient ) );
    cMat->setDiffuse  ( Color( vfDiffuse ) );
    cMat->setSpecular ( Color( vfSpecular ) );
    cMat->setShininess( fShine );
    setComponent( cMat );
    
    // Initialize texture component
    OCSingleTexture *cTex = new OCSingleTexture( sTexture.c_str() );
    cTex->setName("colorMap");
    setComponent( cTex );

    // Initialize shader object
    setComponent( new OCShader( ShaderMgr::POINT_AND_SPOT_LIGHT_SINGLE_TEX ) );
}