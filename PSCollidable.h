#pragma once
#include "ParticleSystem.h"

class Tile3d;

class PSCollidable : public ParticleSystem
{
private:
    // The tile the particle is currently in
    Tile3d *m_CurTile;

    // The tile in which the particle used to be
    Tile3d *m_PrevTile;
    
    // The tile size
    int m_iTileSize;

    // The halh number of tiles per side
    int m_iHalfNumOfTiles;

    // A list of object types to avoid detecting collision with

public:
    /**
     * Constructor
     */
    PSCollidable();

    /**
     * Destructor
     */
    virtual ~PSCollidable();

    /**
     * 
     */
    const Tile3d *getTile() const { return m_CurTile; }
    const Tile3d *getPrevTile() const { return m_PrevTile; }

protected:
    /**
     * 
     */
    virtual void onCollisionWithTiles( 
        Particle *particle, 
        const Vector3f &vCollDir,
        const Vector3f &vCollPoint ) = 0;

    /**
     * 
     */
    virtual void onCollisionWithObjects(
        Particle *particle, 
        const Vector3f &vCollDir,
        const Vector3f &vCollPoint,
        Object *obj ) = 0;

    /**
     * 
     */
    void init();

    /**
     * 
     */
    void checkCollision( Particle *particle );

private:
    /**
     * 
     */
    bool checkCollisionWithTiles( 
        const Vector3f &vPos, 
        Vector3f &vCollDir,
        Vector3f &vCollPoint );

    /**
     * 
     */
    bool checkCollisionWithObject(
        Particle *particle,
        Vector3f &vCollDir,
        Vector3f &vCollPoint,
        Object *oObj
        );

    /**
     * 
     */
    bool readTile( Particle *particle );
    bool readPrevTile( Particle *particle );

    /**
     *
     */
    bool checkWithBox( 
        Particle *particle,
        const Vector3f& vPos, 
        const Vector3f& vBBox,
        Vector3f& vCollDir ) const;

    /**
     * 
     */
    bool checkWithSphere(
        Particle *particle,
        const Vector3f& vPos, 
        float fRadius,
        Vector3f& vCollDir,
        Vector3f& vCollPoint ) const;
};