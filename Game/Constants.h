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
const int WINDOW_HEIGHT                 = 768;  // Height of the screen.

// ======================================================================================
// GAMEPLAY SETTINGS
// ======================================================================================
const float PLAYABLE_AREA               = 200.0f;   // Size of the playable area for boids.
const int   MAX_BOIDS                   = 2000;     // Maximum number of concurrent boids.
const float NEIGHBOUR_SCAN_MOD_GROWTH   = 5.0f;     // Rate of boid awareness growth.
const float NEIGHBOUR_SCAN_MOD_MIN      = 1.0f;     // Min boid awareness bonus.
const float NEIGHBOUR_SCAN_MOD_MAX      = 50.0f;    // Max boid awareness bonus.

const float TT_CAM_MIN_ZOOM             = 40.0f;    // Min zoom of the Tabletop cam.
const float TT_CAM_MAX_ZOOM             = 500.0f;   // Max zoom of the Tabletop cam.
const float TT_CAM_MOVE_SPEED           = 30.0f;    // Speed of moving the Tabletop cam.
const float TT_CAM_SCROLL_SPEED         = 0.1f;     // Speed of scrolling the Tabletop cam.
const float TT_CAM_SHIFT_MODIFIER       = 3.0f;     // Speed modifier when shift is pressed.

const float PLAYER_MOVE_SPEED           = 40.0f;    // Move speed of the player.
const float PLAYER_MAX_SPEED            = 500.0f;   // Max speed the player can reach.
const float PLAYER_ROTATE_SPEED         = 2.0f;     // Rotate speed of the player.

// ======================================================================================
// COMMON PATH STRINGS
// ======================================================================================


// ======================================================================================
// TEXTURE STRINGS
// ======================================================================================


// ======================================================================================
// SOUND STRINGS
// ======================================================================================

