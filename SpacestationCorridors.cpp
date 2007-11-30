#include "SpacestationCorridors.h"
#include "SpacestationCorridorsDisplayList.h"
#include "SingleTexture.h"
#include "SimpleMaterial.h"
#include "Logger.h"
#include "Config.h"

SpacestationCorridors::SpacestationCorridors() {}

void SpacestationCorridors::init()
{
    _LOG("Setting up spacestation corridors...");

    m_vPos.xyz( 0.0f, 0.0f, 0.0f );

    // Read texture filename from configuration file
    string sTexture;
    Config cfg("config.txt");
    cfg.loadBlock("corridor");
    cfg.getString("texture" ,sTexture);

    // Initialize display list component
    setComponent( new SpacestationCorridorsDisplayList );     
    // Initialize material component
    setComponent( new OCSimpleMaterial );
    // Initialize texture component
    setComponent( new OCSingleTexture( sTexture.c_str() ) );
}