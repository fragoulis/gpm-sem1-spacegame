#include "spaceshipkeyboard.h"
#include "spaceship.h"

void SpaceshipKeyboard::update()
{
    Spaceship *ship = (Spaceship*)getOwner();

    if( getKey('a') ) {
		//ship->speed(1.0f);
        ship->speedUp();
    }
    else if( getKey('s') ) {
        //ship->speed(-1.0f);
		ship->speedDown();
    } 
    else {
        ship->resetSpeed();
    }

    if( getKey('f') ) {
		//ship->move( 1.0f, 0.0f );
        ship->moveUp();
    }
    else if( getKey('r') ) {
        //ship->move( -1.0f, 0.0f );
		ship->moveDown();
    }

    if( getKey('g') ) {
		//ship->move( 0.0f, 1.0f );
        ship->moveRight();
    }
    else if( getKey('d') ) {
		//ship->move( 0.0f, -1.0f );
        ship->moveLeft();
    } else {
        ship->resetRoll();
    }

    if( getKey(' ') ) ship->fire();
    else ship->ceaseFire();

    //else if( getKey('t') ) getOwner()->toggleCam();
    //else if( getKey('r') ) getOwner()->record();
}