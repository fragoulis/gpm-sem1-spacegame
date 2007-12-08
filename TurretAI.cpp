#include "TurretAI.h"
#include "Turret.h"
#include "ParticleEmitter.h"
#include "PSLaser.h"
#include "QuatRotation.h"
#include "Clock.h"
#include "Logger.h"
#include "Config.h"
using tlib::Vector3f;
using tlib::Quatf;
using tlib::OCQuatRotation;
using tlib::Logger;
using tlib::Config;

TurretAI::TurretAI( Object *obj ):
m_oTarget(obj)
{
    // Read turret's rotation factor and hir range
    Config cfg("config.txt");
    cfg.loadBlock("turret");
    
    cfg.getFloat("rot_factor", &m_fRotFactor);
    cfg.getFloat("hit_range", &m_fRange);
    
}

void TurretAI::update( Object *oOwner )
{
    // Save distance between target and turret
    Vector3f vTarget = m_oTarget->getPos() - oOwner->getPos();
    const float fDistance = vTarget.length();
    
    // Shorthand laser emitter
    ParticleEmitter& em = ((Turret*)oOwner)->getLaser()->getEmitter();

    // Check if the spaceship target is closer than the 
    // predefined distance set
    if( fDistance < m_fRange && m_oTarget->isActive() )
    {
        OCQuatRotation *cOri = 
            (OCQuatRotation*)oOwner->getComponent("orientation");
        
        // Normalize target view vector
        vTarget.normalize();

        // Find the dot product between the target view vector
        // and the current view vector
        float fDot = cOri->getView().dot( vTarget );
        if( fDot < -1.0f || fDot > 1.0f )
            return;

        if( fDot < 0.999f )
        {
            // Rotate the view vector and the turrets orientation
            const float fAngle = m_fRotFactor * Clock::Instance().getDeltaTime();
            
            // Get the cross product of the target vector and the view vector
            Vector3f vCross = cOri->getView().cross( vTarget );
            vCross.normalize();
            
            // We are trying to move only about one axis at a time
            // In order to do that we check the cross product for which 
            // component is absolute greater and then assign a vector
            // to rotate about according to the sign of the greater component
            Vector3f vOut;

            if( fabs(vCross.x()) > fabs(vCross.y()) && 
                fabs(vCross.x()) > fabs(vCross.z()) )
            {
                if( vCross.x() < 0.0f ) vOut.xyz(-1.0f, 0.0f, 0.0f);
                else if( vCross.x() > 0.0f ) vOut.xyz(1.0f, 0.0f, 0.0f);

            } 
            else if( fabs(vCross.y()) > fabs(vCross.x()) && 
                     fabs(vCross.y()) > fabs(vCross.z()) )
            {
                if( vCross.y() < 0.0f ) vOut.xyz(0.0f, -1.0f, 0.0f);
                else if( vCross.y() > 0.0f ) vOut.xyz(0.0f, 1.0f, 0.0f);
            }
            else if( fabs(vCross.z()) > fabs(vCross.x()) && 
                     fabs(vCross.z()) > fabs(vCross.y()) )
            {
                if( vCross.z() < 0.0f ) vOut.xyz(0.0f, 0.0f, -1.0f);
                else if( vCross.z() > 0.0f ) vOut.xyz(0.0f, 0.0f, 1.0f);
            }
            else
                return;

            // Rotate model about the cross product
            const Quatf& qRes = cOri->addRotation( fAngle, vOut );

            // Rotate view vector
            cOri->updateView( qRes );
        
        } // end if( fDot < 0.999f ) ...

        // If target view vector and view vector are close 
        // enough fire
        if( fDot > 0.99f ) {
            if( !em.isOn() ) em.start();
        }

    } // end if( fDistance < fRange ) ...
    else {
        em.stop();
    }

} // end update()