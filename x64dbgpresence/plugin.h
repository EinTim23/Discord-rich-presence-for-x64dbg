#pragma once

#include "pluginmain.h"


#define PLUGIN_NAME "x64dbg discord rich presence"
#define PLUGIN_VERSION 2

bool pluginInit(PLUG_INITSTRUCT* initStruct);
void pluginStop();
void pluginSetup();
