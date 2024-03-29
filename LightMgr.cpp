#include <cstdlib>
#include "LightMgr.h"
#include "GenericLight.h"
#include "Visual.h"
#include "ObjectMgr.h"
#include "Config.h"
#include "Logger.h"
using tlib::Logger;
using tlib::Config;
using tlib::IOCVisual;

LightMgr::LightMgr():
m_DistItems(0)
{
    // Init the list
    memset( m_vLights, 0, MAX_LIGHTS * sizeof(void*) );

    // Read culling distance from config file
    Config cfg( "config.txt" );
    cfg.loadBlock("point_light");
    cfg.getInt("update_frames", &m_iUpdateFrames);
}

LightMgr::~LightMgr()
{
    _LOG("Deleting light manager");
    //LightList::iterator iter;
    //for( iter = m_vList.begin(); 
    //     iter != m_vList.end(); 
    //     ++iter )
    //{
    //    delete *iter;
    //    *iter = 0;
    //}

    delete [] m_DistItems;
    m_DistItems = 0;
}

// ----------------------------------------------------------------------------
void LightMgr::init()
{
    // Allocate space for the array of distances between each light
    // and the spaceship
    m_DistItems = new _DistItem[ m_vList.size() ];
}

// ----------------------------------------------------------------------------
void LightMgr::render() const 
{
    for( int i=0; i<MAX_LIGHTS; ++i )
    {
        if( m_vLights[i] ) //&& m_vLights[i]->isOn() )
        {
            IOCVisual *cVis = (IOCVisual*)m_vLights[i]->getComponent("visual");
            if( cVis ) {
                cVis->render();
            }
            m_vLights[i]->getPos() += m_vLights[i]->getRenderOffset();
            m_vLights[i]->apply();
            m_vLights[i]->getPos() -= m_vLights[i]->getRenderOffset();
        }
    }
}

// ----------------------------------------------------------------------------
// 1App. Check all lights and see if they lie within a certain radius.
//       If they do, enable them.
// 
// 2App. Find all lights' manhattan distance to the ship and sort them
//       from closest to farest. Then enable the first MAX_LIGHTS
//
// ----------------------------------------------------------------------------
int compare( const void * a, const void * b )
{
    _DistItem *item1 = (_DistItem*)a;
    _DistItem *item2 = (_DistItem*)b;

    int ret = 0;
    if( item1->Md > item2->Md ) {
        ret = 1;
    }
    else if( item1->Md < item2->Md ) {
        ret = -1;
    }

    return ret;
}

#define PRINTDIST for(unsigned k=0;k<m_vList.size();++k) { \
    std::cout << "(Md," << m_DistItems[k].Md << ") (Index," << m_DistItems[k].index << ")" << std::endl;}

// ----------------------------------------------------------------------------
void LightMgr::update()
{
    // Get spaceship
    const Spaceship& oShip = ObjectMgr::Instance().getShip();
    //LightList::const_iterator iter;
    //for( iter = m_vList.begin(); iter != m_vList.end(); ++iter )
    //{
    //    GenericLight *oLight = *iter;
    //    
    //    // Find distance between the spaceship and the light
    //    const float fDistance = fabs((oLight->getPos() - oShip.getPos()).length());
    //    if( fDistance < 150 )
    //    {
    //        initLight( oLight );
    //        oLight->turnOn();
    //    } else {
    //        oLight->turnOff();
    //    }
    //}

    // ________________________________________________________________________
    // Dont update every frame
    static int iTimes = 0;
    if( 0 == iTimes )
    {
        // The following loop updates the distances between the lights and 
        // the spaceship
        GenericLight *oLight;
        int i = 0;
        LightList::const_iterator iter;
        for( iter = m_vList.begin(); iter != m_vList.end(); ++iter )
        {
            // Cache light
            oLight = *iter;

            // Find manhattan distance between the light source and the ship
            m_DistItems[i].Md = 
                fabs(oLight->getPos().x() - oShip.getPos().x()) + 
                fabs(oLight->getPos().y() - oShip.getPos().y()) + 
                fabs(oLight->getPos().z() - oShip.getPos().z());
            
            m_DistItems[i].index = i;

            // Advance index
            ++i;
        } // end for( ... )
        
        //std::cout << "Presort" << std::endl;
        //PRINTDIST

        // Sort the distances
        qsort( m_DistItems, m_vList.size(), sizeof(_DistItem), compare );
        
        //std::cout << "Aftersort" << std::endl;
        //PRINTDIST

        // Turn on the top MAX_LIGHTS lights
        for( int i=0; i<MAX_LIGHTS; ++i )
        {
            // Save this address here
            m_vLights[i] = m_vList[ m_DistItems[i].index ];
            // and give the light a proper id
            m_vLights[i]->setId( i+1 );
            m_vLights[i]->reset();
            m_vLights[i]->turnOn();
        }

        // Reset counter
        iTimes = m_iUpdateFrames;

    } else {
        // Advance counter
        --iTimes;
    }
}