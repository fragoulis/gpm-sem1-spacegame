#include <windows.h>
#include <gl/gl.h>
#include "BladeMgr.h"
#include "RotatingBlade.h"
#include "Animation.h"
#include "ObjectMgr.h"
#include "Config.h"
#include "Logger.h"
using namespace tlib;

RotatingBladeList BladeMgr::m_vBlades;

void BladeMgr::update()
{
    IOCAnimation* cAnim;

    RotatingBlade *obj;
    RotatingBladeList toKill;
    RotatingBladeList::const_iterator iter;
    for( iter = m_vBlades.begin();
         iter != m_vBlades.end();
         iter++ )
    {
        obj = *iter;

        if( ObjectMgr::Instance().isCulled( obj ) ) {
            // If object is not active dont bother updating it
            // since it is culled
            continue;
        }

        cAnim = (IOCAnimation*)obj->getComponent("animation");
        // If object has finished its animation kill it
        if( cAnim->isDone() )
        {
            toKill.push_back( obj );
            continue;
        }

        if( !cAnim->isOn() && !cAnim->isDone() ) {
            obj->getBlade().update();
        } else {
            cAnim->update();
        }
    }

    for( iter = toKill.begin(); 
         iter != toKill.end(); 
         ++iter )
    {
         remove( *iter );
    }

} // end update()

// ----------------------------------------------------------------------------
void BladeMgr::render()
{
    // Draw all blade systems
    RotatingBlade *obj;
    RotatingBladeList::const_iterator iter;
    for( iter = m_vBlades.begin();
         iter != m_vBlades.end();
         iter++ )
    {
        obj = *iter;

        // If it's not active don't render as it is culled
        if( !obj->isActive() ) continue;

        obj->render();
    }

} // end render()

// ----------------------------------------------------------------------------
void BladeMgr::remove( RotatingBlade *value )
{
    _ASSERT(value!=0);

    // Remove it from the object list
    m_vBlades.remove( value );

    // Delete object system from memory
    delete value;
    value = 0;
}