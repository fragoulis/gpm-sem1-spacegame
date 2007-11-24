#include "TurretAI.h"
#include "QuatRotation.h"
#include "Movement.h"
#include "Logger.h"
#include "Config.h"
using tlib::Vector3f;
using tlib::Quatf;
using tlib::OCQuatRotation;
using tlib::Logger;
using tlib::IOCMovement;
using tlib::Config;

TurretAI::TurretAI( Object *obj ):
m_oTarget(obj)
{
    Config cfg("config.txt");
    cfg.loadBlock("turret");
    cfg.getFloat("rot_factor", &m_fRotFactor);
}

void TurretAI::update( Object *oOwner )
{
    // Check if the spaceship target is closer than the 
    // predefined distance set
    const float fRange = 80.0f;

    Vector3f vTarget = m_oTarget->getPos() - oOwner->getPos();
    const float fDistance = vTarget.length();
    
    // If object is within range
    if( fDistance < fRange )
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
        
        // If target view vector and view vector are close 
        // enough
        if( fDot > 0.9f )
        {
            // Fire...

        } // end if( fDot > 0.9f ) ...

        if( fDot < 0.999f )
        {
            // Rotate the view vector and the turrets orientation
            const float fAngle = m_fRotFactor * IOCMovement::DeltaTime();
            
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

            //std::cout << vOut << std::endl;
            // Rotate model about the cross product
            const Quatf& qRes = cOri->addRotation( fAngle, vOut );
            // Rotate view vector
            cOri->updateView( qRes );
        
        } // end if( fDot < 0.999f ) ...

    } // end if( fDistance < fRange ) ...

} // end update()