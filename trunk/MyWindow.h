#pragma once
#pragma comment( lib, "glee" )
#include "GX/GXBase.h"



using namespace gxbase;

// this is a simple example: it creates one window,
// and draws a rotating sphere with hidden surface removal

class MyWindow :public GLWindowEx {
private:
    float m_fFovY, m_fPlanes[2];

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