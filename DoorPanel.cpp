#include "DoorPanel.h"
#include "LinearMovement.h"
#include "Config.h"
using tlib::OCLinearMovement;
using tlib::Config;
using tlib::Vector3f;

void DoorPanel::init( PanelId pid )
{
    Config cfg("config.txt");
    cfg.loadBlock("door_panel");

    // Read velocity
    float fVelocity;
    cfg.getFloat("velocity", &fVelocity);

    // Get panel's half dimensions
    float fvDim[3];
    cfg.getFloat("half_dim", fvDim, 3);

    // Position the panel relative to the owner door
    // and rotate it, both according to its type
    string sPanel;
    switch( pid ) {
        case Left:
            sPanel = "left_panel";
            getPos().xyz( -fvDim[0], 0.0f, fvDim[2] );
            break; 

        case Right:  
            sPanel = "right_panel"; 
            getPos().xyz( fvDim[0], 0.0f, fvDim[2] );
            getDir().toRotation( (float)M_PI, Vector3f::Right() );
            break;

        case Top:
            sPanel = "up_panel";
            getPos().xyz( 0.0f, fvDim[0], -fvDim[2] );
            getDir().toRotation( (float)M_PI_2, Vector3f::Front() );
            break;

        case Bottom: 
            sPanel = "down_panel";  
            getPos().xyz( 0.0f, -fvDim[0], -fvDim[2] );
            getDir().toRotation( 3.0f * (float)M_PI_2, Vector3f::Front() );
            break;
    } // end switch( )

    // Get panel's direction according to its type
    float fvDir[3];
    cfg.getFloat(sPanel, fvDir, 3);

    // Initialize movement component
    setComponent( new OCLinearMovement( fVelocity, Vector3f( fvDir ) ) );

} // end init()