#include "ParticleList.h"

ParticleList_::ParticleList_():
m_First(0),
m_Last(0),
m_Iter(0),
m_iSize(0)
{}

void ParticleList_::push_back( Particle *particle )
{
    if( !m_First && !m_Last )
    {
        m_First = m_Last = m_Iter = particle;
    } 
    else 
    {
        m_Last->setNext( particle );
        particle->setPrev( m_Last );
        m_Last = particle;
        m_Last->setNext(0);
    }

    ++m_iSize;
}

bool ParticleList_::swap( ParticleList_& list )
{
    if( !m_iSize ) return false;

    // Rearrange current's pointers
    Particle *prev = m_Iter->getPrev();
    if( prev ) {
        prev->setNext( m_Iter->getNext() );
    }
    
    if( m_Iter->getNext() ) {
        m_Iter->getNext()->setPrev( prev );
    }
    
    // Rellocate [the value of current]
    list.push_back( m_Iter );

    // Reassign current
    if( prev ) {
        m_Iter = prev;
    } else {
        m_Iter = m_First;
    }

    --m_iSize;

    return true;

} // end swap()