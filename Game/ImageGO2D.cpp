#include "ImageGO2D.h"
#include "DrawData2D.h"
#include "GameData.h"
#include "StringUtils.h"

#include "DDSTextureLoader.h"

ImageGO2D::ImageGO2D(const std::string& _file_name, ID3D11Device* _GD)
    : texture_rv(nullptr)
{
	std::string fullfilename =
#if DEBUG
		"../Debug/"
#else
		"../Release/"
#endif
		+ _file_name + ".dds";
	HRESULT hr = CreateDDSTextureFromFile(_GD, StringUtils::char_to_wchar(fullfilename.c_str()), nullptr, &texture_rv);

	//this nasty thing is required to find out the size of this image!
	ID3D11Resource *pResource;
	D3D11_TEXTURE2D_DESC Desc;
	texture_rv->GetResource(&pResource);
	((ID3D11Texture2D *)pResource)->GetDesc(&Desc);

	origin_ = 0.5f*Vector2((float)Desc.Width, (float)Desc.Height);//around which rotation and scaing is done
}

ImageGO2D::~ImageGO2D()
{
	if (texture_rv)
	{
		texture_rv->Release();
		texture_rv = nullptr;
	}
}

void ImageGO2D::tick(GameData* _GD)
{

}

void ImageGO2D::draw(DrawData2D* _DD)
{
	_DD->sprites->Draw(texture_rv, pos_, nullptr, colour_, rot_, origin_, scale_, SpriteEffects_None);
}
