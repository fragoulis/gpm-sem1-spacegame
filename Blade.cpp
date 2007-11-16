#include "Blade.h"
#include "SimpleMaterial.h"
#include "VisualBox.h"
#include "Movement.h"
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
    m_vPos.xyz( vfOffset );

    // Read part's dimensions
    float vfDim[3];
    cfg.getFloat("dim", vfDim, 3);

    // Read part's rotation factor
    cfg.getFloat("rot_factor", &m_fRotFactor);

    // Initialize material component
    setComponent( new OCSimpleMaterial( 
        Color::black(),
        Color(vfColor),
        Color::null()) 
        );

    // Initialize visual component
    setComponent( new OCVisualBox( Vector3f( vfDim ) * 0.5f ) );

} // end init()

void Blade::update()
{
    // Update object's orientation
    m_qRotFactor.toRotation( 
        m_fRotFactor * IOCMovement::DeltaTime(),
        Vector3f::Back() 
        );

    m_qDir = m_qRotFactor * m_qDir;
}

void Blade::slowDown() {
    if( m_fRotFactor < 0.1f ) {
        m_fRotFactor = 0.0f;
        return;
    }

    m_fRotFactor -= 4.0f * m_fRotFactor * IOCMovement::DeltaTime();
}