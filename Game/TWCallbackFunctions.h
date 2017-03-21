#pragma once
#include "BoidManager.h"

#include <AntTweakBar.h>

/* Header for all Callback Functions used by AntTweakBar.
 * The functions need to be global or the bar can't access them correctly.
 */

inline void TW_CALL delete_all_of_spawn_type(void* _client_data)
{
    auto boid_manager = static_cast<BoidManager*>(_client_data);
    boid_manager->delete_all_of_current_type_selection();
}

inline void TW_CALL delete_all_boids(void* _client_data)
{
    auto boid_manager = static_cast<BoidManager*>(_client_data);
    boid_manager->delete_all_boids();
}
