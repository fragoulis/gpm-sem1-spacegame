#include "OutletAnimation.h"
#include "Object.h"
#include "SimpleMaterial.h"
#include "Config.h"
#include "Logger.h"
using namespace tlib;

OutletAnimation::OutletAnimation():
    OCTimerAnimation(true)
{
    Config cfg("config.txt");
    cfg.loadBlock("outlet");
    
    // Read the blinking interval
    int iBlinkInterval;
    cfg.getInt("blink", &iBlinkInterval );
    setDuration( iBlinkInterval );

    // Read the blink color
    float vfColor[4];
    cfg.getFloat("color", vfColor, 4);
    m_BlinkColor.Assign( vfColor );

    // Automaticaly start all outlets
    start();
}

// ----------------------------------------------------------------------------
void OutletAnimation::OnTimerChange( unsigned int uiCurrentTime,
                                     unsigned int uiDuration ) const
{
    // The following code resutls to a blinking power outlet
    // The color goes from red to white etc...
    OCSimpleMaterial *cMat = (OCSimpleMaterial*)getOwner()->getComponent("material");
    if( (float)uiCurrentTime < (float)uiDuration*0.5f ) {
        cMat->setDiffuse( Color::white() );
    } else {
        cMat->setDiffuse( m_BlinkColor );
    }
        
} // end onUpdate()

// ----------------------------------------------------------------------------
void OutletAnimation::onStart()
{
}

// ----------------------------------------------------------------------------
void OutletAnimation::onStop()
{
    OCSimpleMaterial *cMat = (OCSimpleMaterial*)getOwner()->getComponent("material");
    cMat->setDiffuse( Color::green() );
}