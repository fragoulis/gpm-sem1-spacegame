#include <windows.h>
#include <gl/gl.h>
#include "BladeMgr.h"
#include "RotatingBlade.h"
#include "Animation.h"
#include "ObjectMgr.h"
#include "Tile3d.h"
#include "Config.h"
#include "Logger.h"
using namespace tlib;

RotatingBladeList BladeMgr::m_vBlades;

void BladeMgr::update()
{
    IOCAnimation* cAnim;

    RotatingBlade *obj;
    RotatingBladeList::iterator iter;
    for( iter = m_vBlades.begin();
         iter != m_vBlades.end();
         )
    {
        obj = *iter;

        if( ObjectMgr::Instance().isCulled( obj ) ) {
            // If object is not active dont bother updating it
            // since it is culled
            ++iter;
            continue;
        }

        cAnim = (IOCAnimation*)obj->getComponent("animation");
        // If object has finished its animation kill it
        if( cAnim->isDone() )
        {
            iter = m_vBlades.erase(iter);
            delete obj;
            obj=0;
            continue;
        }

        if( !cAnim->isOn() && !cAnim->isDone() ) {
            obj->getBlade().update();
        } else {
            cAnim->update();
        }

        ++iter;
    }

} // end update()

// ----------------------------------------------------------------------------
void BladeMgr::render() const
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
RotatingBlade* BladeMgr::add( Tile3d *oTile )
{
    // Allocate object
    RotatingBlade *obj = new RotatingBlade;

    // Set its position
    obj->setPosFromIndex( oTile->ijk() );

    // Save it as this tile's occupant
    oTile->setOccupant( (Object*)obj );

    // Push it to the list
    m_vBlades.push_back( obj );

    return obj;
}