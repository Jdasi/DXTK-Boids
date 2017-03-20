#pragma once
#include "BoidManager.h"

#include <AntTweakBar.h>

inline void TW_CALL delete_all_of_spawn_type(void* _client_data)
{
    auto boid_manager = static_cast<BoidManager*>(_client_data);
    boid_manager->delete_all_of_current_type_selection();
}
