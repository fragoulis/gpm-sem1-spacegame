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
#define TW_X_ALIGNED ( TW_TOP | TW_BOTTOM | TW_FRONT | TW_BACK )
#define TW_Y_ALIGNED ( TW_LEFT | TW_RIGHT | TW_FRONT | TW_BACK )
#define TW_Z_ALIGNED ( TW_TOP | TW_BOTTOM | TW_LEFT | TW_RIGHT )

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
    // The index of the tile
    int i, j, k;

private:
    // Pointer to a special object
    // The list of special objects consists of lights, barriers, 
    // triggers and defence guns
    tlib::Object *m_oOccupant;

    int 
        // The corridor type
        m_iCorrType,

        // If tile occupant is an outlet, this will link the outlet
        // with a barrier
        m_iFriendId;

public:
    /**
     * Constructor
     */
    Tile3d();

    /**
     * Destructor
     */
    ~Tile3d();

    /**
     * Returns the occupier object
     */
    tlib::Object* getOccupant() const { return m_oOccupant; }

    /**
     * Getter/Setter for the tile type
     */
    int getType() const { return m_iCorrType; }
    void setType( int type ) { m_iCorrType = type; }

    /**
     * Getter/Setter for the friend id
     */
    int getFriendId() const { return m_iFriendId; }
    void setFriendId( int id ) { m_iFriendId = id; }

    /**
     * Creates a new object of the wanted type and assigns that
     * to the tile's object handle
     */
    void addObject( int objType );

private:
    // Helper functions
    void addDoor();
    void addOutlet();

}; // end of Tile3d class