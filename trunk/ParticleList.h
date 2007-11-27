#pragma once
#include "Particle.h"
//class Particle;

// A very simple utility class handle the particle rearrangements
class ParticleList
{
private:
    // The first particle
    Particle *m_First;

    // The last particle
    Particle *m_Last;

    // Floating element
    Particle *m_Iter;

    // The number of particles
    int m_iSize;

public:
    // Constructor
    ParticleList();

    // Accesors
    Particle *first() { return m_First; }
    Particle *last() { return m_Last; }
    Particle *iter() { return m_Iter; }
    void begin() { m_Iter = m_First; }
    void end() { m_Iter = m_Last; }
    int size() const { return m_iSize; }
    
    Particle* next() {
        if( !m_Iter->getNext() )
            // current reached the end
            return 0;

        // advance current and return it
        m_Iter = m_Iter->getNext();
        return m_Iter;
    }

    // Pushes a particle at the end of the list
    // Simple and hopefully quick [because that's our aim here]
    // because of the inlining
    void push_back( Particle *particle );

    // Moves a particle from this list to the end of another
    bool swap( ParticleList& list );

};