#pragma once
#include <string>

/*! \file Constants.h
@brief   Constants that are used throughout the game. 
@details Add any new constants to this file. 
*/

// ======================================================================================
// WINDOW SETTINGS
// ======================================================================================
const int WINDOW_WIDTH                  = 1024; // Width of the screen.
const int WINDOW_HEIGHT                 = 768; // Height of the screen.

// ======================================================================================
// GAMEPLAY SETTINGS
// ======================================================================================
const float PLAYABLE_AREA               = 200.0f; // Size of the playable area for boids.
const int   MAX_BOIDS                   = 2000; // Maximum number of concurrent boids.
const float NEIGHBOUR_SCAN_MOD_GROWTH   = 5.0f; // Rate of boid awareness growth.
const float NEIGHBOUR_SCAN_MOD_MIN      = 1.0f; // Min boid awareness bonus.
const float NEIGHBOUR_SCAN_MOD_MAX      = 50.0f; // Max boid awareness bonus.

// ======================================================================================
// COMMON PATH STRINGS
// ======================================================================================


// ======================================================================================
// TEXTURE STRINGS
// ======================================================================================


// ======================================================================================
// SOUND STRINGS
// ======================================================================================

