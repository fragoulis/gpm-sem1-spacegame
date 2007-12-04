#include "BladePoll.h"
#include "SimpleMaterial.h"
#include "VisualBox.h"
#include "Config.h"
using tlib::OCSimpleMaterial;
using tlib::OCVisualBox;
using tlib::Config;
using tlib::Vector3f;

void BladePoll::init()
{
    Config cfg("config.txt");
    cfg.loadBlock("blade_poll");

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

    // Initialize material component
    OCSimpleMaterial *cMat = new OCSimpleMaterial;
    cMat->setAmbient( Color( vfColor ) );
    cMat->setDiffuse( Color( vfColor ) );
    setComponent( cMat );

    // Initialize visual component
    setComponent( new OCVisualBox( Vector3f( vfDim ) * 0.5f ) );

} // end init()