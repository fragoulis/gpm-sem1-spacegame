
#include "spaceshipkeyboard.h"
#include "spaceship.h"

void OCSpaceshipKeyboard::update() const
{
    Spaceship *ship = (Spaceship*)getOwner();

    if( getKey('z') ) {
        ship->speedUp();
    }
    else if( getKey('x') ) {
        ship->speedDown();
    } 
    else {
        ship->resetSpeed();
    }

    if( getKey('s') ) {
        ship->moveUp();
    }
    else if( getKey('w') ) {
        ship->moveDown();
    }

    if( getKey('d') ) {
        ship->moveRight();
    }
    else if( getKey('a') ) {
        ship->moveLeft();
    } else {
        ship->resetRoll();
    }

    //else if( getKey('f') ) getOwner()->fire();
    //else if( getKey('t') ) getOwner()->toggleCam();
    //else if( getKey('r') ) getOwner()->record();
}