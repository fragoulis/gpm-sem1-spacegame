#include "TurretMgr.h"
#include "Turret.h"
#include "TurretAI.h"
#include "ParticleSystemMgr.h"
#include "PSLaser.h"
#include "VisualGXModel.h"
#include "QuatRotation.h"
#include "Vitals.h"
#include "ObjectMgr.h"
#include "Config.h"
#include "Logger.h"
using namespace tlib;

TurretList TurretMgr::m_vTurrets;

// ----------------------------------------------------------------------------
void TurretMgr::init( Object *obj )
{
    // Set object's type
    setType( TURRET );

    Config cfg("config.txt");
    cfg.loadBlock("turret");

    // Initialize visual component
    string sModel;
    float fScale;
    cfg.getString("model", sModel );
    cfg.getFloat("scale", &fScale );
    setComponent( new OCGXModel( sModel.c_str(), fScale ) );

    // Initialize AI component
    setComponent( new TurretAI( obj ) );
}

// ----------------------------------------------------------------------------
void TurretMgr::update()
{
    IOCAI *cAI = (IOCAI*)getComponent("ai");
    OCQuatRotation *cShipOri;

    IOCVitals *cVitals;
    TurretList toKill;
    TurretList::const_iterator iter;
    for( iter = m_vTurrets.begin();
         iter != m_vTurrets.end();
         ++iter )
    {
        // Cache dereference
        Turret *obj = *iter;
        _ASSERT(obj!=0);

        if( ObjectMgr::Instance().isCulled( obj ) ) {
            // If object is not active dont bother updating it
            // since it is culled
            continue;
        }

        // Check the turret's health status
        cVitals = (IOCVitals*)obj->getComponent("vitals");
        if( !cVitals->update() ) 
        {
            // If turret is dead, remove it from the list
            toKill.push_back( obj );
            continue;
        }

        // Get turret orientation by updating the AI component
        cAI->update( obj );

        // Update each turrets individual orientation
        cShipOri = (OCQuatRotation*)obj->getComponent("orientation");
        cShipOri->update();

    } // end for( ... )

    for( iter = toKill.begin(); 
         iter != toKill.end(); 
         ++iter )
    {
         remove( *iter );
    }

} // end update()

// ----------------------------------------------------------------------------
void TurretMgr::render()
{
    IOCVisual *cModel = (IOCVisual*)getComponent("visual");

    // Draw all turrets
    Turret *obj;
    TurretList::const_iterator iter;
    for( iter = m_vTurrets.begin();
         iter != m_vTurrets.end();
         ++iter )
    {
        // Cache dereference
        obj = *iter;
        _ASSERT(obj!=0);

        if( !obj->isActive() ) {
            // If object is not active dont bother rendering it
            // since it is culled
            continue;
        }

         // Get visual component
        getPos().xyz( obj->getPos() );
        getDir().wxyz( obj->getDir() );

#ifdef _SHOW_AXIS
        glPushMatrix();
            glTranslatef( m_vPos.x(), m_vPos.y(), m_vPos.z() );
            IComponent *cmp = obj->getComponent("orientation");
            OCOrientation3D *qr = (OCOrientation3D*)cmp;
            const Vector3f& view = qr->getView() * 20.0f;
            const Vector3f& right = qr->getRight() * 20.0f;
            const Vector3f& up = qr->getUp() * 20.0f;
            
            glDisable(GL_LIGHTING);
            glBegin(GL_LINES);
                glColor3f(1,0,0);
                glVertex3f(0,0,0);
                glVertex3f(view.x(), view.y(), view.z());
                glColor3f(0,1,0);
                glVertex3f(0,0,0);
                glVertex3f(right.x(), right.y(), right.z());
                glColor3f(0,0,1);
                glVertex3f(0,0,0);
                glVertex3f(up.x(), up.y(), up.z());
            glEnd();
            glEnable(GL_LIGHTING);
        glPopMatrix();
#endif
        cModel->render();
    }

} // end render()

// ----------------------------------------------------------------------------
void TurretMgr::remove( Turret *value )
{
    _ASSERT(value!=0);

    // Remove it from the object list
    m_vTurrets.remove( value );

    // Delete object system from memory
    delete value;
    value = 0;
}