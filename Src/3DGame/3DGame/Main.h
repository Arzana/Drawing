#pragma once

#include <cstdio>
#include "FireworksGame.h"
#include "Room.h"

#define _USE_FULL_HD

#ifdef _USE_FULL_HD
#define HEIGHT		1080
#define WIDTH		1920
#else
#define HEIGHT		600
#define WIDTH		800
#endif