#pragma once
#include "Singleton.h"

// The maximum number of lights opengl can hold
static const int MAX_LIGHTS = 8;

class GenericLight;

/**
 * This class manages the light sources on the scene.
 * The class keeps a list of [up to eight] lights, the ones
 * that are actually turned on currenlty.
 *
 * Each time a light is turned on, it 'consults' this manager
 * and the manager returns an available light id.
 * If all lights are used, then the managers uses the culliing
 * distance variable to turn of every light outside that
 * perimeter, and if still there are no light ids available, it
 * reduces the culling distance virtually till it succeeds.
 */
class LightMgr : public Singleton<LightMgr>
{
    friend Singleton<LightMgr>;
private:
    // The array of lights
    GenericLight *m_vLights[MAX_LIGHTS];

public:
    /**
     * 
     */
    bool initLight( GenericLight *light );

private:
    /**
     * Constructor
     */
    LightMgr();

    /**
     * Destructor
     */
    ~LightMgr();

};