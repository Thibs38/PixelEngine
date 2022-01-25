#pragma once

extern "C" {
#include "wren.h"
}

class WrenManager
{
public:
	static void init();
	static void destroy();
private:
	static WrenVM* VM;

};