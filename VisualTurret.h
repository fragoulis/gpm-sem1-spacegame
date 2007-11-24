#pragma once
#include "VisualVertexArray.h"
using tlib::OCVisualVertexArray;
/**
 * 
 */
class VisualTurret : public OCVisualVertexArray
{
private:           


public:
    /**
     * Constructor
     */
    VisualTurret();

    /**
     * Returns the unique component ID
     */
    const string componentID() const { 
        return string("visualturret"); 
    }

    /**
     * Renders the turret
     */
    void render() const;

    /**
     * Creates the turret
     */
    bool create();

}; // end of VisualTurret class