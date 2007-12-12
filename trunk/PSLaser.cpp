#include <windows.h>
#include <gl/gl.h>
#include "PSLaser.h"
#include "Particle.h"
#include "ParticleSystemMgr.h"
#include "CollisionResponse.h"
#include "Orientation2d.h"
#include "PSScorchMarks.h"
#include "Tile3d.h"
#include "Timer.h"
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
    int iLife;
    cfg.getFloat("size",     &fSize);
    cfg.getFloat("velocity", &fVelocity);
    cfg.getInt("lifespan",   &iLife);

    // At first, mark all particles as dead
    for( int i=0; i<iNumOfParticles; ++i ) {
        m_Particles[i].setStartLife( iLife );
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
    
    // This line is important, although laser's particle system time
    // is always 0[unlimited], we must explicitly start it, in order
    // to emit particles[lasers bolts]
    m_Timer->start();

} // end init()

// ----------------------------------------------------------------------------
void PSLaser::update()
{
    // Only create new particles if emitter is active
    if( m_Emitter.isOn() )
    {
        // If time is not right don't do anything
        if( !m_Emitter.getTimer()->isRunning() ) 
        {
            // Get owner's orientation component
            OCOrientation2D * cOri = 
                (OCOrientation2D*)m_oOwner->getComponent("orientation");
            
            // We pass the object's position plus a small correction 
            // plus the position offset times the object's view vector
            m_vDir = cOri->getView();
            m_vPos = m_oOwner->getPos() + m_vDir * m_fOffset;
            spawn();

            m_Emitter.getTimer()->start();
        }
    }

    Particle *obj;
    ParticleList::iterator iter;
    for( iter = m_Emitter.getPAlive().begin();
         iter != m_Emitter.getPAlive().end();
         )
    {
        obj = *iter;

        // Update the object's energy
        obj->updateLife();

        // Check if the particle has expired
        if( obj->getLife() < 0 )
        {
            iter = m_Emitter.getPAlive().erase(iter);
			m_Emitter.getPDead().push_back( obj );
            continue;
        }

        // Update particles' position
        obj->update();

        // Check for collisions
        checkCollision( obj );

        ++iter;
    } // end for( ... )

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
    float matrix[16];
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
                glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
                //matrix[0] = matrix[5] = matrix[10] = matrix[11] = 1.0f;
                matrix[0] = matrix[5] = matrix[10] = 1.0f;
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
                                    const Vector3f &vCollDir,
                                    const Vector3f &vCollPoint )
{
    particle->bounce( vCollDir, 0.5f );
    particle->setLife( particle->getLife()/5 );

    // Set the scorch mark's direction
    m_Marks->setDir( vCollDir );

    // Create a scorch mark on the point of collision
    m_Marks->setPos( vCollPoint );
    m_Marks->spawn();
}

// ----------------------------------------------------------------------------
void PSLaser::onCollisionWithObjects( Particle *particle, 
                                      const Vector3f &vCollDir,
                                      const Vector3f &vCollPoint,
                                      Object *oObj )
{
    particle->bounce( vCollDir, 0.3f );
    particle->setLife( particle->getLife()/5 );

    // Call collision response for the object
    IOCCollisionResponse *cColRes = 
        (IOCCollisionResponse*)oObj->getComponent("collisionresponse");
    if( cColRes ) {
        cColRes->setCollDir( vCollDir );
        cColRes->setCollPoint( vCollPoint );
        cColRes->respond();
    }
}

// ----------------------------------------------------------------------------
void PSLaser::onSpawn( Particle *particle ) 
{
    particle->setPos( m_vPos );
    particle->setDir( m_vDir );
    particle->resetLife();
}