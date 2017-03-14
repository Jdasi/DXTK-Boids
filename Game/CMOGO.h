#pragma once
#include "GameObject.h"

//=================================================================
//A Class for loading and displaying CMO as GameObjects
//=================================================================

using namespace DirectX;

struct GameData;
class CMOModel;

class CMOGO : public GameObject
{
public:
    explicit CMOGO(CMOModel* _model);
	virtual ~CMOGO() = default;

	virtual void tick(GameData* _GD) override;
	virtual void draw(DrawData* _DD) override;

    void set_model(CMOModel* _model);

    bool is_visible() const;
    void set_visible(bool _visible);

protected:
    CMOModel* model_;
    bool visible_;

};
