#pragma once

#include "GX/GXBase.h"
// Object managers
#include "DoorMgr.h"
#include "BladeMgr.h"
#include "ForcefieldMgr.h"
#include "OutletMgr.h"
#include "TurretMgr.h"

using namespace gxbase;

// this is a simple example: it creates one window,
// and draws a rotating sphere with hidden surface removal

class MyWindow :public GLWindowEx {
private:
	// Here we declare our individual managers
    // These are managers for the barriers, the power outlets 
    // and the defence guns
    DoorMgr         m_DoorMgr;
    BladeMgr        m_BladeMgr;
    ForcefieldMgr   m_ForcefieldMgr;
    OutletMgr       m_OutletMgr;
    TurretMgr       m_TurretMgr;

public:
	MyWindow();
	void OnCreate();
	void OnDisplay();
	void OnIdle();
	void OnResize(int w, int h);
    void OnDestroy();
	void OnKeyboard(int key, bool down);
    void drawInterface();
};