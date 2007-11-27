#include "spaceshipkeyboard.h"
#include "spaceship.h"

void OCSpaceshipKeyboard::update() const
{
    Spaceship *ship = (Spaceship*)getOwner();

    if( getKey('a') ) {
        ship->speedUp();
    }
    else if( getKey('s') ) {
        ship->speedDown();
    } 
    else {
        ship->resetSpeed();
    }

    if( getKey('f') ) {
        ship->moveUp();
    }
    else if( getKey('r') ) {
        ship->moveDown();
    }

    if( getKey('g') ) {
        ship->moveRight();
    }
    else if( getKey('d') ) {
        ship->moveLeft();
    } else {
        ship->resetRoll();
    }

    if( getKey(' ') ) ship->fire();
    else ship->ceaseFire();

    //else if( getKey('t') ) getOwner()->toggleCam();
    //else if( getKey('r') ) getOwner()->record();
}