#include "Blade.h"
#include "SimpleMaterial.h"
#include "VisualBox.h"
#include "Clock.h"
#include "Config.h"
using namespace tlib;

void Blade::init()
{
    Config cfg("config.txt");
    cfg.loadBlock("blade");

    // Read part's color
    float vfColor[4];
    cfg.getFloat("color", vfColor, 4);

    // Read part's position relative to system center
    float vfOffset[3];
    cfg.getFloat("offset", vfOffset, 3);
    getPos().xyz( vfOffset );

    // Read part's dimensions
    float vfDim[3];
    cfg.getFloat("dim", vfDim, 3);

    // Read part's rotation factor
    cfg.getFloat("rot_factor", &m_fRotFactor);

    // Initialize material component
    OCSimpleMaterial *cMat = new OCSimpleMaterial;
    cMat->setAmbient( Color( vfColor ) );
    cMat->setDiffuse( Color( vfColor ) );
    setComponent( cMat );

    // Initialize visual component
    setComponent( new OCVisualBox( Vector3f( vfDim ) ) );

} // end init()

void Blade::update()
{
    // Update object's orientation
    m_qRotFactor.toRotation( 
        m_fRotFactor * Clock::Instance().getDeltaTime(),
        Vector3f::Back() 
        );

    setDir( m_qRotFactor * getDir() );
}

void Blade::slowDown() {
    if( m_fRotFactor < 0.1f ) {
        m_fRotFactor = 0.0f;
        return;
    }

    m_fRotFactor -= 4.0f * m_fRotFactor * Clock::Instance().getDeltaTime();
}