#include "SpacestationCorridors.h"
#include "SpacestationCorridorsDisplayList.h"
#include "Texture.h"
#include "Logger.h"
#include "Config.h"

SpacestationCorridors::SpacestationCorridors() {}

void SpacestationCorridors::setup()
{
    _LOG("Setting up spacestation corridors...");

    Config cfg("config.txt");
    cfg.loadBlock("corridor");

    string sTexture;
    cfg.getString("texture" ,sTexture);

    setComponent( new SpacestationCorridorsDisplayList );      
    setComponent( new OCTexture( sTexture.c_str() ) );
}