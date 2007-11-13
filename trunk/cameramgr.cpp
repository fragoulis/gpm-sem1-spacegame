
#include "cameramgr.h"

/**
 * Constructor
 */
CameraMgr::CameraMgr(): m_pActiveCamera(0) {}

/**
 * Destructor
 */
CameraMgr::~CameraMgr()
{
    CameraList::iterator iter;
    for( iter = m_vCamera.begin(); 
         iter != m_vCamera.end(); 
         iter++ )
    {
        delete iter->second;
        iter->second = 0;
    }
}

/**
 * Saves the selected camera to the active camera pointer
 */
Camera* CameraMgr::activate( const string &id )
{
    m_pActiveCamera = m_vCamera[ id ];

    return m_pActiveCamera;
}