#include <windows.h>
#include <gl/gl.h>
#include "Spacecube.h"
#include "Config.h"
#include "SingleTexture.h"
#include "VisualBox.h"
using tlib::Vector3f;
using tlib::Config;
using tlib::OCSingleTexture;
using tlib::OCVisualBox;
using tlib::IOCVisual;

void Spacecube::init()
{
    // Set object's type
    setType( SPACECUBE );
    setPos( Vector3f( 0.0f, 0.0f, 0.0f ) );

    Config cfg("config.txt");
    cfg.loadBlock("spacecube");

    // Read textures
    string sColorMap, sColorMap2;
    cfg.getString("color_map", sColorMap);
    setComponent( new OCSingleTexture( sColorMap.c_str() ) );

    float fvDim[3];
    cfg.getFloat("bbox", fvDim, 3);
    
    // Initialize visual component
    OCVisualBox *cVBox = new OCVisualBox;
    setComponent( cVBox );
    cVBox->init( Vector3f( fvDim ) );
}

void Spacecube::render()
{
    glFrontFace( GL_CW );
    glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
    ((IOCVisual*)getComponent("visual"))->render();
    glFrontFace( GL_CCW );
}