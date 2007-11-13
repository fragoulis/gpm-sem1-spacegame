#pragma once

namespace tlib { class Object; }

// Texture wall flags
// Each of these indicates for each tile which of its faces will be textured
#define TW_TOP    0x01
#define TW_BOTTOM 0x02
#define TW_FRONT  0x04
#define TW_BACK   0x08
#define TW_LEFT   0x10
#define TW_RIGHT  0x20

// Object container flags
// Each tile can have zero or one of the following objects
#define OBJ_LIGHT   1
#define OBJ_DOOR    2
#define OBJ_BLADE   3
#define OBJ_FIELD   4
#define OBJ_TRIGGER 5
#define OBJ_GUN     6
#define OBJ_OUTLET  7

/**
 * Holds information of where and how to draw each piece
 * of corridor for the spacestation
 */
class Tile3d
{
public:
    /**
     * Constructor
     */
    Tile3d( int pi, int pj, int pk, 
            int pcorrType, int pobjType );

    /**
     * Destructor
     */
    ~Tile3d();

    // The corridor type
    int corrType;

    // Pointer to a special object
    // The list of special objects consists of lights, barriers, 
    // triggers and defence guns
    tlib::Object *obj;

    // The index of the tile
    int i, j, k;

    /**
     * Creates a new object of the wanted type and assigns that
     * to the tile's object handle
     */
    void addObject( int objType );

}; // end of Tile3d class