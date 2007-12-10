#include <windows.h>
#include <gl/gl.h>
#include "PSBigExplosion.h"
#include "Particle.h"
#include "Object.h"
#include "Timer.h"
#include "Config.h"
#include "Logger.h"
#include "Random.h"
using tlib::Config;
using tlib::Logger;
using tlib::Object;

PSBigExplosion::PSBigExplosion()
{ setType( BigExplosion ); }

// ----------------------------------------------------------------------------
void PSBigExplosion::init( const Vector3f &vSysPos )
{
    _LOG("Initializing big explosion PS");

    // Save owner object's position
    m_vPos = vSysPos;

    // Open configuration file
    Config cfg("config.txt");
    cfg.loadBlock("big_explosion");

    // Read release time and count
    double dReleaseTime;
    int iReleaseCount;
    cfg.getDouble("release_time", &dReleaseTime);
    cfg.getInt("release_count", &iReleaseCount);
    m_Emitter.init( dReleaseTime, iReleaseCount );

    // Read particles' velocity, size and lifespan limits
    float fSize;
    cfg.getFloat("size", &fSize);
    cfg.getInt("lifespan", m_iLifeSpan, 2);
    cfg.getFloat("velocity", m_fVelocity, 2);

    // Read particles' start and end color
    float vfEndColor[4];
    cfg.getFloat("start_color", m_vfStartColor, 4);
    cfg.getFloat("end_color", vfEndColor, 4);

    for( int i=0; i<4; ++i )
        m_vfColorFactor[i] = vfEndColor[i] - m_vfStartColor[i];

    // Read system's lifespan
    double dLifeSpan;
    cfg.getDouble("sys_lifespan", &dLifeSpan);
    m_Timer->setDuration(dLifeSpan);

    // Read particle number
    int iNumOfParticles;
    cfg.getInt("max_particles", &iNumOfParticles);

    // Initialize particle array
    m_Particles = new Particle[iNumOfParticles];

    // At first, mark all particles as dead
    for( int i=0; i<iNumOfParticles; ++i ) {
        m_Emitter.getPDead().push_back( &m_Particles[i] );
    }

    // Read texture
    string sTexture;
    cfg.getString("texture", sTexture);
    setTexture( sTexture );

    ParticleSystem::init( fSize );

} // end init()

// ----------------------------------------------------------------------------
void PSBigExplosion::update()
{
    // Only create new particles if emitter is active
    if( m_Emitter.isOn() ) {
        // If time expired spawn
        if( !m_Emitter.getTimer()->isRunning() ) {
            spawn();
            // Start the timer again for the new spawn
            m_Emitter.getTimer()->start();
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

        // Update the object's energy
        obj->updateLife();

        // Check if the particle has expired
        if( obj->getLife() < 0 )
        {
            toKill.push_back( obj );
            continue;
        }

        // Update particles' positions
        obj->update();
        obj->updateColor( m_vfStartColor, m_vfColorFactor );

    } // end for( ... )
    
    // Kill the temporary list
    kill( toKill );

} // end update()

// ----------------------------------------------------------------------------
void PSBigExplosion::render() const
{   
    // Disable depth test
    glDepthMask( GL_FALSE );

    // Enable blending
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE );

    // Disable lighting
    glDisable( GL_LIGHTING );

    // Enable texture
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, m_uiTexId );

    // Render all alive particles
    float matrix[16];
    Particle *obj;
    ParticleList::const_iterator iter;
    for( iter = m_Emitter.getPAlive().begin();
         iter != m_Emitter.getPAlive().end();
         ++iter )
    {
        obj = *iter;

        // Set color
        glColor4fv( obj->getColor() );

        const Vector3f& vPos = obj->getPos();
        glPushMatrix();
        {
            glTranslatef( vPos.x(), vPos.y(), vPos.z() );
            glPushMatrix();
            {
                // Render as billboard
                glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
                matrix[0] = matrix[5] = matrix[10] = matrix[11] = 1.0f;
                matrix[1] = matrix[2] = matrix[ 3] = matrix[ 4] =
                matrix[6] = matrix[7] = matrix[ 8] = matrix[ 9] = 0.0f;
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
void PSBigExplosion::onSpawn( Particle *particle ) 
{
    particle->setStartLife( tlib::randInt( m_iLifeSpan[0], m_iLifeSpan[1] ) );
    particle->setPos( m_vPos );
        
    Vector3f vDir( tlib::randFloat( -0.1f, 0.1f ), 
                   tlib::randFloat(  0.0f, 1.0f ), 
                   tlib::randFloat( -1.0f, 1.0f ) );
    vDir.normalize();

    float fVel = tlib::randFloat( m_fVelocity[0], m_fVelocity[1] );
    particle->setVelocity( vDir * fVel );
}