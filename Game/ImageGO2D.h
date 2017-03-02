#pragma once
#include "GameObject2D.h"

//=================================================================
//A Game Object 2D for displaying images
//image needs to have been converted to a dds by the Assets project
//add orginal file to this project and set it to "Image Content Pipeline"
// in a similar fashion as getting the models
//=================================================================

class ImageGO2D :public GameObject2D
{
public:
	ImageGO2D(const std::string& _file_name, ID3D11Device* _GD);
	virtual ~ImageGO2D();

	virtual void tick(GameData* _GD);
	virtual void draw(DrawData2D* _DD);

protected:
	ID3D11ShaderResourceView* texture_rv;

};
