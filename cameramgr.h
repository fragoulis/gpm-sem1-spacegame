
#pragma once

#include <map>
#include <string>
#include "singleton.h"
#include "camera.h"

using std::map;
using std::string;

// Map a string to each camera
typedef map< string, Camera* >  CameraList;

class CameraMgr : public Singleton<CameraMgr>
{
    // Declare singleton as a friend so that its destructor
    // call call the manager's destructor
    friend Singleton<CameraMgr>;

private:
    // The associative array of cameras
    CameraList m_vCamera;

    // A pointer to the currently active camera
    Camera *m_pActiveCamera;

private:
    /**
     * Constructor
     */
    CameraMgr();

    /**
     * Destructor
     */
    ~CameraMgr();

public:
    /**
     * Returns the active camera pointer
     */
    inline Camera* getActive() const;

    /**
     * Returns a pointer to a camera
     * NOTE: Why [get() const] gives a compiler error?
     */
    inline Camera* get( const string &id );

    /**
     * Applies the current camera view
     */
    inline void apply() const;

    /**
     * Creates a new camera obejct, maps it to a unique id, pushes it 
     * to the camera list and returns a pointer to the new camera
     */
    template<class T>
    T* add( const string &id, bool bMakeActive = false );

    /**
     * Saves the selected camera to the active camera pointer
     */
    Camera* activate( const string &id );

}; // end of CameraMgr class

// Template functions' definitions
template<class T>
T* CameraMgr::add( const string &id, bool bMakeActive )
{
    T *newCam = new T;

    m_vCamera[ id ] = static_cast<Camera*>(newCam);

    // Make a camera active even if its not flaged as active
    // if the active camera pointer is null
    if( bMakeActive || !m_pActiveCamera ) 
        m_pActiveCamera = newCam;

    return newCam;
}

// Inline functions' definitiosn
Camera* CameraMgr::getActive() const { 
    return m_pActiveCamera; 
}

Camera* CameraMgr::get( const string &id ) { 
    return m_vCamera[ id ]; 
}

void CameraMgr::apply() const { 
    m_pActiveCamera->apply(); 
}