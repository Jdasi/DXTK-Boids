#pragma once
#include "BoidData.h"
#include "BoidSettings.h"
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
    BoidManager(CMOManager& _cmo_manager, unsigned int _initial_humans = 0);
    ~BoidManager() = default;

    void tick(GameData* _GD);
    void draw(DrawData* _DD);

private:
    void add_boid(BoidType _type, Vector3 _pos);
    CMOModel* fetch_model(BoidType _type) const;
    BoidSettings& fetch_settings(BoidType _type);

    std::map<Rule::ID, std::unique_ptr<Rule>> rules_;
    CMOManager& cmo_manager_;
    BoidData boid_data_;

    BoidSettings human_settings_;
    BoidSettings zombie_settings_;

};
