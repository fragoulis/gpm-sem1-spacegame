#include <windows.h>
#include <gl/gl.h>
#include "PSCommon.h"
#include "Particle.h"
#include "PSTemplate.h"
#include "Object.h"
#include "Timer.h"
#include "Config.h"
#include "Logger.h"
#include "Random.h"
using namespace tlib;

// ----------------------------------------------------------------------------
void PSCommon::init( const PSTemplate &Template, const Vector3f &vSysPos )
{
    _LOG("Initializing common particle system");

    // Save owner object's position
    m_vPos = vSysPos;

    // Copy values from particle system template
    Template.copyTo( *this );

    // Initialize the rest
    m_Emitter.init( m_dReleaseTime, m_iReleaseCount );
    m_Timer->setDuration(m_dSysDuration);

    // At first, mark all particles as dead
    m_Particles = new Particle[m_iNumOfParticles];
    for( int i=0; i<m_iNumOfParticles; ++i ) {
        m_Emitter.getPDead().push_back( &m_Particles[i] );
    }

    ParticleSystem::init( m_fSize[0] );

} // end init()

// ----------------------------------------------------------------------------
void PSCommon::update()
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

        // Update particles' positions
        obj->update();
        obj->updateColor( m_vfColor, m_vfColorFactor );

        ++iter;
    } // end for( ... )

} // end update()

// ----------------------------------------------------------------------------
void PSCommon::render() const
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
void PSCommon::onSpawn( Particle *particle ) 
{
    particle->setColor( m_vfColor );
    particle->setSize( randFloat( m_fSize[0], m_fSize[1] ) );
    particle->setStartLife( randInt( m_iLifeSpan[0], m_iLifeSpan[1] ) );
    particle->setPos( m_vPos );

    Vector3f vDir( randFloat( m_vfDirection[0][0], m_vfDirection[0][1] ), 
                   randFloat( m_vfDirection[1][0], m_vfDirection[1][1] ), 
                   randFloat( m_vfDirection[2][0], m_vfDirection[2][1] ) );
    vDir.normalize();

    float fVel = randFloat( m_fVelocity[0], m_fVelocity[1] );
    particle->setVelocity( vDir * fVel );
}