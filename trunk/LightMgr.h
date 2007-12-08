#pragma once
#include "Singleton.h"
#include <vector>
using std::vector;

// The maximum number of lights opengl can hold
static const int MAX_LIGHTS = 7;

class GenericLight;
typedef vector<GenericLight*> LightList;

/**
 * 
 */
struct _DistItem {
    float Md;
    int index;
};

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

    // A list of all the lights in the scene
    LightList m_vList;

    // Array of distances between the light's and the spaceship
    // We also need an extra index to connect the distance to a
    // light in the light vector container, that is why
    // we group them into a struct.
    _DistItem *m_DistItems;

    // The number of frames before updating the light distances
    int m_iUpdateFrames;

public:
    /**
     * Initializes the light manager
     */
    void init();

    /**
     * Adds a light to the list
     */
    void add( GenericLight *light ) {
        m_vList.push_back( light );
    }

    /**
     * Renders all lights
     */
    void render() const;

    /**
     * Updates the lights' status.
     * Checks the spaceship's position and tries to keep 
     * only the light's close to the spaceship lit
     */
    void update();

private:
    /**
     * Constructor
     */
    LightMgr();

    /**
     * Destructor
     */
    ~LightMgr();

}; // end of LightMgr class