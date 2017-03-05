#pragma once
#include "BoidData.h"
#include "Rule.h"

#include <d3d11.h>
#include "SimpleMath.h"

#include <map>
#include <memory>

using namespace DirectX;
using namespace SimpleMath;

class CMOManager;
struct DrawData;
struct GameData;

class BoidManager
{
public:
    BoidManager(CMOManager& _cmo_manager, unsigned int _initial_boids = 0);
    ~BoidManager() = default;

    void tick(GameData* _GD);
    void draw(DrawData* _DD);

private:
    void add_boid(Vector3 _pos);

    std::map<Rule::ID, std::unique_ptr<Rule>> rules_;
    CMOManager& cmo_manager_;
    BoidData boid_data_;

};
