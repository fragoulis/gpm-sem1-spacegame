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
    m_vPos.xyz( vfOffset );

    // Read part's dimensions
    float vfDim[3];
    cfg.getFloat("dim", vfDim, 3);

    // Initialize material component
    setComponent( new OCSimpleMaterial( 
        Color::black(),
        Color(vfColor),
        Color::null()) 
        );

    // Initialize visual component
    setComponent( new OCVisualBox( Vector3f( vfDim ) * 0.5f ) );

} // end init()