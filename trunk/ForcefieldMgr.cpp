#include <gl/glee.h>
#include "ForcefieldMgr.h"
#include "Forcefield.h"
#include "SimpleMaterial.h"
#include "Animation.h"
#include "SimpleMaterial.h"
#include "MultiTexture.h"
#include "VisualBox.h"
#include "ObjectMgr.h"
#include "Shader.h"
#include "Visual.h"
#include "Tile3d.h"
#include "Config.h"
#include "Logger.h"
using namespace tlib;

ForcefieldList ForcefieldMgr::m_vForcefields;

void ForcefieldMgr::init()
{
    _LOG("Initializing forcefield renderer...");
    Config cfg("config.txt");
    cfg.loadBlock("forcefield");

    // Read the 'warping' speed
    cfg.getFloat("warp_step", &m_fWarpStep);

    // Read vertex and pixel shader files
    string sVertex, sPixel;
    cfg.getString("vertex_shader", sVertex);
    cfg.getString("pixel_shader", sPixel);

    // Read forcefield's color
    float vfColor[4];
    cfg.getFloat("color", vfColor, 4);

    // Initialize material component
    OCSimpleMaterial *cMat = new OCSimpleMaterial;
    cMat->setAmbient( Color( vfColor ) );
    cMat->setDiffuse( Color( vfColor ) );
    setComponent( cMat );

    // Read textures for the forcefield
    string sColorMap, sNoiseMap;
    cfg.getString("color_map", sColorMap);
    cfg.getString("noise_map", sNoiseMap);

    // Initialize textures
    OCMultiTexture *cMTex = new OCMultiTexture( 2 );
    cMTex->set( 0, sColorMap.c_str() );
    cMTex->setName( 0, "colorMap" );
    cMTex->set( 1, sNoiseMap.c_str() );
    cMTex->setName( 1, "noiseMap" );
    setComponent( cMTex );

    // Read the visual box of the forcefield
    float vfBBox[3];
    cfg.getFloat("bbox", vfBBox, 3);
    
    // Initialize visual component
    OCVisualBox *cVBox = new OCVisualBox;
    setComponent( cVBox );
    cVBox->init( Vector3f( vfBBox ) * 0.5f );

    // Initialize shader object
    setComponent( new OCShader( ShaderMgr::FORCEFIELD_DOUBLE_TEX ) );
}

// ----------------------------------------------------------------------------
void ForcefieldMgr::update()
{
    IOCAnimation* cAnim;

    Forcefield *obj;
    ForcefieldList toKill;
    ForcefieldList::const_iterator iter;
    for( iter = m_vForcefields.begin();
         iter != m_vForcefields.end();
         iter++ )
    {
        obj = *iter;

        if( ObjectMgr::Instance().isCulled( obj ) ) {
            // If object is not active dont bother updating it
            // since it is culled
            continue;
        }

        cAnim = (IOCAnimation*)obj->getComponent("animation");
        // If object has finished its animation kill it
        if( cAnim->isDone() ) {
            toKill.push_back( obj );
            continue;
        }

        cAnim->update();
    }

    for( iter = toKill.begin(); 
         iter != toKill.end(); 
         ++iter )
    {
         remove( *iter );
    }

} // end update()

// ----------------------------------------------------------------------------
void ForcefieldMgr::render()
{
    // Declare a variable to hold some the change
    static float fTimer = 0.0f;

    // Start using the shader
    OCShader *cShader = (OCShader*)getComponent("shader");
    cShader->begin();

    // The a timer value to the shader
    glUniform1f(cShader->getUniform("timer"), fTimer);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE );

    // Enable material
    OCSimpleMaterial *cMat = (OCSimpleMaterial*)getComponent("material");

    // Enable textures
    IOCTexture *cText = (IOCTexture*)getComponent("texture");
    cText->apply();

    // Get visual component which will draw the door panels
    IOCVisual *cBox = (IOCVisual*)getComponent("visual");

    Forcefield *obj;
    ForcefieldList::const_iterator iter;
    for( iter = m_vForcefields.begin();
         iter != m_vForcefields.end();
         iter++ )
    {
        obj = *iter;

        // If it's not active don't render as it is culled
        if( !obj->isActive() ) continue;

        // Apply the same material by with individual alpha values
        cMat->getDiffuse().alpha( obj->getAlpha() );
        cMat->apply();

        // Draw the forcefield
        getPos().xyz( obj->getPos() );
        getDir().wxyz( obj->getDir() );
        cBox->render();

    } // end for( )

    // Alter the timer value
    float sign = m_fWarpStep;
    if( fTimer > 1000.0f )
        sign = -m_fWarpStep;
    else if( fTimer < -1000.0f )
        sign = m_fWarpStep;
    fTimer += sign;
    
    // Reset textures
    cText->reset();

    // Disable blending 
    glDisable(GL_BLEND);

    // Stop using the shader
    cShader->end();

} // end render()

// ----------------------------------------------------------------------------
void ForcefieldMgr::remove( Forcefield *value )
{
    _ASSERT(value!=0);

    // Remove it from the object list
    m_vForcefields.remove( value );

    // Delete object system from memory
    delete value;
    value = 0;
}

// ----------------------------------------------------------------------------
Forcefield* ForcefieldMgr::add( Tile3d *oTile )
{
    // Allocate object
    Forcefield *field = new Forcefield;

    // Set its position
    field->setPosFromIndex( oTile->ijk() );

    // Save it as this tile's occupant
    oTile->setOccupant( (Object*)field );

    // Push it to the list
    m_vForcefields.push_back( field );

    return field;
}