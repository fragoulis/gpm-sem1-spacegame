#include "LightMgr.h"
#include "GenericLight.h"
#include "Config.h"
using tlib::Config;

LightMgr::LightMgr()
{
    // Init the list
    memset( m_vLights, 0, MAX_LIGHTS * sizeof(void*) );

    // Read culling distance from config file
    Config cfg( "config.txt" );
    cfg.loadBlock("global");
    //cfg.getFloat("cull_distance", &m_fCullDistance);
}

LightMgr::~LightMgr()
{}

// ----------------------------------------------------------------------------
bool LightMgr::initLight( GenericLight *light ) 
{        
    // Search through the list to find an available light id
    bool bFound = false;
    for( int i=0; i<MAX_LIGHTS && !bFound; ++i )
    {
        // If pointer is null, light is free
        if( !m_vLights[i] ) 
        {
            // Save this address here
            m_vLights[i] = light;
            // and give the light a proper id
            light->setId( i );
            light->reset();
            bFound = true;
        } 
    }

    return bFound;
}