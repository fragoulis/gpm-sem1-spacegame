#include "BladeMgr.h"
#include "RotatingBlade.h"
#include "Animation.h"
#include "Config.h"
#include "Logger.h"
using namespace tlib;

vector<RotatingBlade*> BladeMgr::m_vBlades;

void BladeMgr::render()
{
    // Draw all blade systems
    vector<RotatingBlade*>::const_iterator iter;
    for( iter = m_vBlades.begin();
         iter != m_vBlades.end();
         iter++ )
    {
        (*iter)->render();
    }

} // end render()

void BladeMgr::update()
{
    IOCAnimation* cAnim;

    vector<RotatingBlade*>::const_iterator iter;
    for( iter = m_vBlades.begin();
         iter != m_vBlades.end();
         iter++ )
    {
        cAnim = (IOCAnimation*)(*iter)->getComponent("animation");
        if( !cAnim->isOn() && !cAnim->isDone() ) {
            (*iter)->getBlade().update();
        } else {
            cAnim->update();
        }
    }
} // end update()