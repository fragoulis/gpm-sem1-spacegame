#pragma once
#pragma comment( lib, "glee" )
#include "GX/GXBase.h"

using namespace gxbase;

// this is a simple example: it creates one window,
// and draws a rotating sphere with hidden surface removal

class MyWindow :public GLWindowEx {
private:
    enum AppControlFlags {
        FULLSCREEN,
        CAMERA,
        RECORD,
        COLLISION,
        WIREFRAME,
        GOD_MODE,
        NUM_OF_CONTROLS
    };
    char *AppControlLabel[NUM_OF_CONTROLS];
    char AppControlKey[NUM_OF_CONTROLS];
    bool AppControl[NUM_OF_CONTROLS];

    float m_fFovY, m_fPlanes[2], m_fDimRatio;

public:
	MyWindow();
	void OnCreate();
	void OnDisplay();
	void OnIdle();
	void OnResize(int w, int h);
    void OnDestroy();
	void OnKeyboard(int key, bool down);
    void drawInterface();
    GLuint makeGrid( int , float );
};