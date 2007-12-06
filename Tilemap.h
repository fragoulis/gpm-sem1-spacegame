#pragma once
#include "Singleton.h"
#include "Tile3d.h"
#include "Vector3.h"
#include <map>
using std::map;
using tlib::Vector3f;

typedef map< int, Tile3d* > TileAssocArray;

class Tilemap : public Singleton<Tilemap>
{
    friend Singleton<Tilemap>;

private:
    // The number of tiles along any axis of the spacestation
    int m_iNumOfTiles;

    // A single tile's size
    int m_iTileSize;
    
    // The array of tiles
    Tile3d *m_TileArray;

    // The number of tiles that the tile list holds
    int m_iTileArraySize;

    // The list of tiles mapped into an integer, used to quickly find 
    // a wanted tile.
    // The key is the index of the tile in an imaginary 3d cube
    // [key: i + j*tiles + k*tiles*tiles]
    TileAssocArray m_TileMap;

public:
    /**
     * Getters for the number of tiles and the tilesize
     */
    int getNumOfTiles() const { return m_iNumOfTiles; }
    int getTileSize() const { return m_iTileSize; }

    /**
     * Getter for the tile map structure
     */
    const TileAssocArray& getTileAssocArray() const {
        return m_TileMap;
    }

    /**
     * Assigns a tile's address to a certain index of the tile map
     */
    void setTile( int mapIndex, int tileIndex ) {
        m_TileMap[ mapIndex ] = &m_TileArray[ tileIndex ];
    }

    /**
     * Returns a pointer to a tile directly from the tile array
     */
    Tile3d* getTileByIndex( int tileIndex ) {
        return &m_TileArray[ tileIndex ];
    }

    /**
     * Returns the total count of tiles the tile array holds
     */
    int getArraySize() const { return m_iTileArraySize; }

    /**
     * Returns a pointer to a tile from the tile map
     * by passing a single index
     */
    Tile3d* getTile( int index ) {
        return m_TileMap[index];
    }

    /**
     * Returns a pointer to a tile from the tile map
     * by passing a 3d index
     */
    Tile3d* getTile( int i, int j, int k )
    { 
        int index = i + 
                    j * m_iNumOfTiles + 
                    k * m_iNumOfTiles * m_iNumOfTiles;

        return m_TileMap[index];
    }

    /**
     * Returns a pointer to a tile from the tile map
     * by passing a position vector and checking if that position is 
     * in fact within a tile
     */
    Tile3d* getTile( const Vector3f& vIn );

    /**
     * Allocates memory for the tiles' array
     */
    void create( int iNumOfTiles );

private:
    /**
     * Constructor
     */
    Tilemap();

    /**
     * Destructor
     */
    ~Tilemap();

};