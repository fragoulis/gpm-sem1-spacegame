
#include <iostream>
#include "../../config.h"

using namespace std;

// ---- What we want to save in the configuration file
// Number of lives for the spaceship (N)
// Number of hits the shield can withstand (N)
// Number of hits a defence gun can withstand (N)
// Number of hits before the reactor explodes (N)

// Number of tiles of one spacestation's side
// Size of an individual tile
// Pathname of the spacestation mapfile

// Pathname of the spaceship model
// Pathname of the outer texture of the spacestation
// Pathname of the inner corridor textures

// Initial position of the spaceship
// Initial rotation of the spaceship

/**
 * We open a/the configuration file by using a hardcoded string
 * and afterwards we use the file to withdraw information about our
 * programme.
 */
void main()
{    
    tlib::Config cfg( "config.txt" );

    /**
     * We search by keyname, ex. spaceship_shield_hits, and return
     * by given type
     */

    // So, if we want to read the number of lives the spaceship has this 
    // should be sufficient
    int iSpaceshipLives;
    cfg.getInt("spaceship_lives", &iSpaceshipLives );
    cout << iSpaceshipLives << endl;

    char iSpacestationTiles[50];
    cfg.getString("spacestation_texture", iSpacestationTiles );
    cout << iSpacestationTiles << endl;
    
    cfg.close();
    char k = ' ';
    while( k != 'q' )
    {
        cfg.open( "config.txt" );

        float vSpaceshipInitRot[4];
        cfg.getFloat("spaceship_init_rot", vSpaceshipInitRot, 4 );
        cout << vSpaceshipInitRot[0] << " "
             << vSpaceshipInitRot[1] << " "
             << vSpaceshipInitRot[2] << " "
             << vSpaceshipInitRot[3] << endl;
    
        cfg.close();
        cin >> k;
    }
}