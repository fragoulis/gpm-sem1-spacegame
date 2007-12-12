#include <windows.h>
#include <gl/gl.h>
#include "PSScorchMarks.h"
#include "Particle.h"
#include "CollisionResponse.h"
#include "Orientation2d.h"
#include "Object.h"
#include "Tile3d.h"
#include "Config.h"
#include "Logger.h"
using tlib::Config;
using tlib::Logger;
using tlib::IOCCollisionResponse;
using tlib::Object;
using tlib::OCOrientation2D;

const float E_CORRECTION = .2f;

// ----------------------------------------------------------------------------
void PSScorchMarks::init()
{
    _LOG("Initializing scorch mark PS");

    // Open configuration file
    Config cfg("config.txt");
    cfg.loadBlock("scorch_marks");

    // Read particle number
    int iNumOfParticles;
    cfg.getInt("max_particles", &iNumOfParticles);

    // Initialize particle array
    m_Particles = new Particle[iNumOfParticles];

    // Read particle size and lifespan
    float fSize;
    int iLife;
    cfg.getFloat("size", &fSize);
    cfg.getInt("lifespan", &iLife);

    // At first, mark all particles as dead
    for( int i=0; i<iNumOfParticles; ++i ) {
        m_Particles[i].setSize( fSize );
        m_Particles[i].setStartLife( iLife );
        m_Emitter.getPDead().push_back( &m_Particles[i] );
    }

    // Read texture
    string sTexture;
    cfg.getString("texture", sTexture);
    setTexture( sTexture );

    // Initialize base class [compile's a display list for the particle]
    ParticleSystem::init( fSize );

    // This line is important, although marks' particle system time
    // is always 0[unlimited], we must explicitly start it, in order
    // to emit particles[scorch marks]
    m_Timer->start();

} // end init()

// ----------------------------------------------------------------------------
void PSScorchMarks::update()
{
    // Here we save the particles to be killed after the next update
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

        ++iter;
    } // end for( ... )

} // end update()

// ----------------------------------------------------------------------------
void PSScorchMarks::render() const
{   
    // Disable depth test
    glDepthMask( GL_FALSE );

    // Enable blending
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    // Disable lighting
    glDisable( GL_LIGHTING );

    // Enable texture
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, m_uiTexId );

    // Render all alive particles
    Particle *obj;
    ParticleList::const_iterator iter;
    for( iter = m_Emitter.getPAlive().begin();
         iter != m_Emitter.getPAlive().end();
         ++iter )
    {
        obj = *iter;
        // Set color
        float rgba[] = { 0.0f, 0.0f, 0.0f, obj->getLifeRatio() };
        glColor4fv( rgba );

        const Vector3f& vPos = obj->getPos();
        glPushMatrix();
        {
            glTranslatef( vPos.x(), vPos.y(), vPos.z() );

            // Rotate particle so that it faces the correct direction
            glRotatef( (float)obj->getRot().w(), 
                       (float)obj->getRot().x(),
                       (float)obj->getRot().y(), 
                       (float)obj->getRot().z() );

            // Draw particle
            glCallList( m_uiListId );
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
    //glEnable( GL_DEPTH_TEST );
    glDepthMask( GL_TRUE );

} // end render()

// ----------------------------------------------------------------------------
void PSScorchMarks::onSpawn( Particle *particle ) 
{
    // Correct position to be pos%35=0
    Vector3f vPos = m_vPos;
    

    // Correct position and orientation of the scorch mark
    Quati qRot( 0, 0, 0, 0 );
    if( (int)m_vDir.x() == (int)1.0f ) {
        qRot.wxyz( 90, 0, 1, 0 );
        vPos.addX( E_CORRECTION );
    }
    else if( (int)m_vDir.x() == -(int)1.0f ) {
        qRot.wxyz( 90, 0, -1, 0 );
        vPos.subX( E_CORRECTION );
    }
    else if( (int)m_vDir.y() == (int)1.0f ) {
        qRot.wxyz( 90, -1, 0, 0 );
        vPos.addY( E_CORRECTION );
    }
    else if( (int)m_vDir.y() == -(int)1.0f ) {
        qRot.wxyz( 90, 1, 0, 0 );
        vPos.subY( E_CORRECTION );
    }
    else if( (int)m_vDir.z() == -(int)1.0f ) {
        qRot.wxyz( 180, 0, 1, 0 );
        vPos.subZ( E_CORRECTION );
    } else {
        vPos.addZ( E_CORRECTION );
    }

    particle->setPos( vPos );
    particle->setRot( qRot );
    particle->resetLife();
}