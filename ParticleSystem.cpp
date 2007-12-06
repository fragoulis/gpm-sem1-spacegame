#include "ParticleSystem.h"
#include "Particle.h"
#include "TextureMgr.h"
#include "Logger.h"
#include <ctime>
using tlib::Logger;

ParticleSystem::ParticleSystem():
m_iType(Any),
m_Particles(0),
m_uiListId(0),
m_oOwner(0)
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
void ParticleSystem::start() 
{ 
    m_Emitter.start();
    m_Timer.start();
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
    // Set the texture format to alpha
    TextureMgr::Instance().setFormat( Image::ALPHA );
    // Generate the texture using the texture manager and save the id
    m_uiTexId = TextureMgr::Instance().getTexture( sTexture.c_str() );
}

// ----------------------------------------------------------------------------
void ParticleSystem::spawn()
{
    // Do nothing if there are no available particles
    if( m_Emitter.getPDead().empty() ) return;

    Particle *obj;
    ParticleList::const_iterator iter;
    for( int i = 0; i < m_Emitter.getReleaseCount(); ++i )
    {
        // Take the last particle of the list
        obj = *(m_Emitter.getPDead().begin());
        
        // call derived callback
        onSpawn( obj );

        // Push it to the alive list
        m_Emitter.getPAlive().push_back( obj );

        // Remove it from the dead list
        m_Emitter.getPDead().pop_front();

        if( m_Emitter.getPDead().empty() ) return;
    }

} // end spawn()

// ----------------------------------------------------------------------------
void ParticleSystem::kill( ParticleList &toKill )
{
    Particle *obj;
    ParticleList::const_iterator iter;
    for( iter = toKill.begin();
         iter != toKill.end();
         ++iter )
    {
        obj = *iter;
        // Remove it from the alive list
        m_Emitter.getPAlive().remove( obj );

        // Push it to the dead list
        m_Emitter.getPDead().push_back( obj );
    }

    // Empty kill list
    toKill.clear();
}