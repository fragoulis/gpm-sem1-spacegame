#include "ParticleSystem.h"
#include "Particle.h"
#include "TextureMgr.h"
#include "Logger.h"
#include <ctime>
using tlib::Logger;

// This will save us some time
extern const double M_CLOCKS_PER_SEC = 1.0 / CLOCKS_PER_SEC;

ParticleSystem::ParticleSystem():
m_iType(Any),
m_Particles(0),
m_dLifeSpan(0.0),
m_uiListId(0),
m_bIsExpired(true)
{}

ParticleSystem::~ParticleSystem()
{
    // Delete system's particles
    if( m_Particles ) {
        delete [] m_Particles;
        m_Particles = 0;
    }

    // Free display list
    if( m_uiListId )
        glDeleteLists( m_uiListId, 1 );
}

// ----------------------------------------------------------------------------
float ParticleSystem::m_vfRandom[500];

// ----------------------------------------------------------------------------
void ParticleSystem::start() 
{ 
    m_Emitter.start();
    m_bIsExpired = false;
    m_lInitTime = clock();
}

// ----------------------------------------------------------------------------
bool ParticleSystem::hasExpired()
{
    // If life span time is negligible force no expiration
    if( m_dLifeSpan < 1e-3 ) return false;
    
    // If system has already expired return
    if( m_bIsExpired ) return true;

    // Check expiration time
    long curTime = clock();
    if( m_dLifeSpan < (double)(curTime - m_lInitTime)*M_CLOCKS_PER_SEC )
    {
        // Turn expiration flag on
        m_bIsExpired = true;
    }
    
    return m_bIsExpired;
}

// ----------------------------------------------------------------------------
void ParticleSystem::init( float fParticleSize )
{
    // Create display list for the laser particle
    m_uiListId = glGenLists(1);
    glNewList( m_uiListId, GL_COMPILE );
        glBegin(GL_QUADS);
            glNormal3f( 0.0f, 0.0f, 1.0f );
            glTexCoord2f( 0.0f, 0.0f );
            glVertex2f( -fParticleSize, -fParticleSize );
            glTexCoord2f( 1.0f, 0.0f );
            glVertex2f(  fParticleSize, -fParticleSize );
            glTexCoord2f( 1.0f, 1.0f );
            glVertex2f(  fParticleSize,  fParticleSize );
            glTexCoord2f( 0.0f, 1.0f );
            glVertex2f( -fParticleSize,  fParticleSize );
        glEnd();
    glEndList();
}

// ----------------------------------------------------------------------------
void ParticleSystem::setTexture( const std::string sTexture )
{
    TextureMgr::Instance().setFormat( Image::ALPHA );
    m_uiTexId = TextureMgr::Instance().getTexture( sTexture.c_str() );
}

// ----------------------------------------------------------------------------
void ParticleSystem::spawn()
{
    // Do nothing if there are no available particles
    if( m_Emitter.getPDead().empty() ) return;

    ParticleList::const_iterator iter;
    for( int i = 0; i < m_Emitter.getReleaseCount(); ++i )
    {
        // Take the last particle of the list
        iter = m_Emitter.getPDead().begin();

        // call derived callback
        onSpawn( *iter );

        // Push it to the alive list
        m_Emitter.getPAlive().push_back( *iter );

        // Set init time for its internal clock
        //(*iter)->setInitTime( clock() + long((*iter)->getLifeSpan() * CLOCKS_PER_SEC) );
        (*iter)->start();

        // Remove it from the dead list
        m_Emitter.getPDead().pop_front();

        if( m_Emitter.getPDead().empty() ) return;
    }

} // end spawn()

// ----------------------------------------------------------------------------
void ParticleSystem::kill( ParticleList &toKill )
{
    ParticleList::const_iterator iter;
    for( iter = toKill.begin();
         iter != toKill.end();
         ++iter )
    {
        // Remove it from the alive list
        m_Emitter.getPAlive().remove( *iter );

        // Push it to the dead list
        m_Emitter.getPDead().push_back( *iter );
    }

    // Empty kill list
    toKill.clear();
}

// ----------------------------------------------------------------------------
void ParticleSystem::genNumbers()
{ 
    // Generate 500 floats 
}