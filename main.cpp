#pragma once
#define VLD_MAX_DATA_DUMP 100
//#include <vld.h>
#include "MyWindow.h"

using namespace gxbase;

class MyApp :public App {
public:
	MyWindow w;
};
static MyApp tut6;