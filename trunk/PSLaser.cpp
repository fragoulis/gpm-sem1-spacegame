#include <windows.h>
#include <gl/gl.h>
#include "PSLaser.h"
#include "Particle.h"
#include "ParticleSystemMgr.h"
#include "CollisionResponse.h"
#include "Orientation2d.h"
#include "PSScorchMarks.h"
#include "Tile3d.h"
#include "Config.h"
#include "Logger.h"
using tlib::Config;
using tlib::Logger;
using tlib::IOCCollisionResponse;
using tlib::OCOrientation2D;

// ----------------------------------------------------------------------------
void PSLaser::init( Object *oOwner,
                    float fEmitterOffset,
                    float vfLaserColor[] )
{
    _LOG("Initializing laser PS");

    // Save owner object pointer
    m_oOwner = oOwner;

    // Initialize position offset
    m_fOffset = fEmitterOffset;

    // Initialize particle color
    m_Color.Assign( vfLaserColor );

    // Open configuration file
    Config cfg("config.txt");
    cfg.loadBlock("laser");

    // Read release time and count
    double dReleaseTime;
    cfg.getDouble("release_time", &dReleaseTime);

    int iReleaseCount;
    cfg.getInt("release_count", &iReleaseCount);

    m_Emitter.init( dReleaseTime, iReleaseCount );

    // Read particle number
    int iNumOfParticles;
    cfg.getInt("max_particles", &iNumOfParticles);

    // Initialize particle array
    m_Particles = new Particle[iNumOfParticles];

    // Read particle size, velocity and lifespan
    float fSize, fVelocity;
    cfg.getFloat("size", &fSize);
    cfg.getFloat("velocity", &fVelocity);
    cfg.getDouble("lifespan", &m_dLifeSpan);

    // At first, mark all particles as dead
    for( int i=0; i<iNumOfParticles; ++i ) {
        m_Particles[i].setLifeSpan( m_dLifeSpan );
        m_Particles[i].setVelocity( fVelocity );
        m_Particles[i].setSize( fSize );
        m_Emitter.getPDead().push_back( &m_Particles[i] );
    }

    // Read texture
    string sTexture;
    cfg.getString("texture", sTexture);
    setTexture( sTexture );

    // Initialize base class [compile's a display list for the particle]
    ParticleSystem::init( fSize );

    // Initialize collidable's class
    PSCollidable::init();

    // Initialize the scorch marks' particle system
    m_Marks = PSManager::Instance().addScorchMark();

} // end init()

// ----------------------------------------------------------------------------
void PSLaser::update()
{
    // Only create new particles if emitter is active
    if( m_Emitter.isOn() ) {
        // If time is not right don't do anything
        if( m_Emitter.checkRelease() ) {
            // Get owner's orientation component
            OCOrientation2D * cOri = 
                (OCOrientation2D*)m_oOwner->getComponent("orientation");
            // We pass the object's position plus a small correction 
            // plus the position offset times the object's view vector
            m_vDir = cOri->getView();
            m_vPos = m_oOwner->getPos() + m_vDir * m_fOffset;
            spawn();
        }
    }

    // Here we save the particles to be killed after the next update
    ParticleList toKill;
    Particle *obj;
    ParticleList::const_iterator iter;
    for( iter = m_Emitter.getPAlive().begin();
         iter != m_Emitter.getPAlive().end();
         ++iter )
    {
        obj = *iter;

        // Check if the particle has expired
        if( obj->hasExpired() )
        {
            toKill.push_back( obj );
            continue;
        }

        // Update particles' positions
        obj->updatePos();

        // Check for collisions
        checkCollision( obj );

    } // end for( ... )
    
    // Kill the temporary list
    kill( toKill );

} // end update()

// ----------------------------------------------------------------------------
void PSLaser::render() const
{   
    // Disable depth test
    glDepthMask( GL_FALSE );

    // Enable blending
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    // Disable lighting
    glDisable( GL_LIGHTING );

    // Set color
    glColor4fv( m_Color.rgba() );

    // Enable texture
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, m_uiTexId );
        
    // Render all alive particles
    ParticleList::const_iterator iter;
    for( iter = m_Emitter.getPAlive().begin();
         iter != m_Emitter.getPAlive().end();
         ++iter )
    {
        const Vector3f& vPos = (*iter)->getPos();
        glPushMatrix();
        {
            glTranslatef( vPos.x(), vPos.y(), vPos.z() );
            glPushMatrix();
            {
                // Render as billboard
                float matrix[16];
                glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
                matrix[0] = matrix[5] = matrix[10] = matrix[11] = 1.0f;
                matrix[1] = matrix[2] = matrix[3] = matrix[4] =
                matrix[6] = matrix[7] = matrix[8] = matrix[9] = 0.0f;
                glLoadMatrixf(matrix);

                glCallList( m_uiListId );
            }
            glPopMatrix();
        }
        glPopMatrix();

    } // end for( ... )
    
    // Disable texturing
    glDisable( GL_TEXTURE_2D );

    // Enable lighting
    glEnable( GL_LIGHTING );

    // Disable blending
    glDisable( GL_BLEND );

    // Enable depth test 
    glDepthMask( GL_TRUE );

} // end render()

// ----------------------------------------------------------------------------
void PSLaser::onCollisionWithTiles( Particle *particle, 
                                    const Vector3f &vColDir,
                                    const Vector3f &vColPoint )
{
    particle->bounce( vColDir, 0.5f );
    particle->setLifeSpan(0.4f);

    // Set the scorch mark's direction
    m_Marks->setDir( vColDir );

    // Create a scorch mark on the point of collision
    m_Marks->setPos( vColPoint );
    m_Marks->spawn();
}

// ----------------------------------------------------------------------------
void PSLaser::onCollisionWithObjects( Particle *particle, 
                                      const Vector3f &vColDir,
                                      Object *oObj )
{
    particle->bounce( vColDir, 0.3f );
    particle->setLifeSpan(0.3f);

    // Call collision response for the object
    IOCCollisionResponse *cColRes = 
        (IOCCollisionResponse*)oObj->getComponent("collisionresponse");
    if( cColRes )
        cColRes->respond( vColDir );
}

// ----------------------------------------------------------------------------
void PSLaser::onSpawn( Particle *particle ) 
{
    particle->setPos( m_vPos );
    particle->setDir( m_vDir );
    particle->setLifeSpan( m_dLifeSpan );
}