#include "PSSmallExplosion.h"
#include "Particle.h"
#include "gx/image.h"
#include "Movement.h"
#include "Object.h"
#include "Config.h"
#include "Logger.h"
using tlib::IOCMovement;
using tlib::Config;
using tlib::Logger;
using tlib::Object;

PSSmallExplosion::PSSmallExplosion()
{ setType( SmallExplosion ); }

// ----------------------------------------------------------------------------
void PSSmallExplosion::init( const Vector3f &vSysPos )
{
    _LOG("Initializing small explosion PS");

    // Save owner object's position
    m_vPos = vSysPos;

    // Open configuration file
    Config cfg("config.txt");
    cfg.loadBlock("small_explosion");

    // Read release time and count
    double dReleaseTime;
    int iReleaseCount;
    cfg.getDouble("release_time", &dReleaseTime);
    cfg.getInt("release_count", &iReleaseCount);
    m_Emitter.init( dReleaseTime, iReleaseCount );

    // Read particles' velocity, size and lifespan limits
    float fSize;
    cfg.getFloat("size", &fSize);
    cfg.getFloat("lifespan", m_fLifeSpan, 2);
    cfg.getFloat("velocity", m_fVelocity, 2);

    // Read particles' start and end color
    //float vfStartColor[4], vfEndColor[4];
    //cfg.getFloat("start_color", vfStartColor, 4);
    //cfg.getFloat("end_color", vfEndColor, 4);

    // Read system's lifespan
    cfg.getDouble("sys_lifespan", &m_dLifeSpan);

    // Read particle number
    int iNumOfParticles;
    cfg.getInt("max_particles", &iNumOfParticles);

    // Initialize particle array
    m_Particles = new Particle[iNumOfParticles];

    // At first, mark all particles as dead
    for( int i=0; i<iNumOfParticles; ++i ) {
        m_Emitter.getPDead().push_back( &m_Particles[i] );
    }

    ParticleSystem::init( fSize );

} // end init()

// ----------------------------------------------------------------------------
void PSSmallExplosion::update()
{
    // Only create new particles if emitter is active
    if( m_Emitter.isOn() ) {
        // If time is not right don't do anything
        if( m_Emitter.checkRelease() ) {
            spawn();
        }
    }

    // Here we save the particles to be killed after the next update
    ParticleList toKill;

    ParticleList::const_iterator iter;
    for( iter = m_Emitter.getPAlive().begin();
         iter != m_Emitter.getPAlive().end();
         ++iter )
    {
        Particle *obj = *iter;

        // Check if the particle has expired
        if( obj->hasExpired() )
        {
            toKill.push_back( obj );
            continue;
        }

        // Update particles' positions
        obj->updatePos();

    } // end for( ... )
    
    // Kill the temporary list
    kill( toKill );

} // end update()

// ----------------------------------------------------------------------------
void PSSmallExplosion::render() const
{   

    // Disable lighting
    glDisable( GL_LIGHTING );

    // Render all alive particles
    float matrix[16];
    ParticleList::const_iterator iter;
    for( iter = m_Emitter.getPAlive().begin();
         iter != m_Emitter.getPAlive().end();
         ++iter )
    {
        const Vector3f& vPos = (*iter)->getPos();

        // Set color
        float rgba[] = { 1.0f, 1.0f - (*iter)->getEnergy(), 0.0f, 1.0f };
        glColor4fv( rgba );

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
    
    // Enable lighting
    glEnable( GL_LIGHTING );

} // end render()

// ----------------------------------------------------------------------------
void PSSmallExplosion::onSpawn( Particle *particle ) 
{
    particle->setLifeSpan( tlib::randFloat( m_fLifeSpan[0], m_fLifeSpan[1] ) );
    particle->setPos( m_vPos );
    Vector3f vDir( tlib::randFloat(-1,1), tlib::randFloat(-1,1), tlib::randFloat(-1,1) );
    vDir.normalize();
    particle->setDir( vDir );
    particle->setVelocity( tlib::randFloat( m_fVelocity[0], m_fVelocity[1] ) );
}

// ----------------------------------------------------------------------------
void PSSmallExplosion::onKill( Particle *particle )
{}