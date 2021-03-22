#pragma once

// uncomment the line below to enable emscripten compatibility
//#define EMSCRIPTEN_COMPATIBLE

#include "olcPixelGameEngine.h"
#include "olcPGEX_SplashScreen.h"
#include "olcPGEX_AnimatedSprite.h"
#include <math.h>
#include <chrono>

#define time std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()