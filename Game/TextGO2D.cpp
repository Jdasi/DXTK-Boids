#include "TextGO2D.h"
#include "DrawData2D.h"
#include "StringUtils.h"

TextGO2D::TextGO2D(const std::string& _text)
    : text_(_text)
{
}

void TextGO2D::tick(GameData* _GD)
{
}

void TextGO2D::draw(DrawData2D* _DD)
{
	_DD->font->DrawString(_DD->sprites.get(), StringUtils::char_to_wchar(text_.c_str()), pos_, colour_,rot_,origin_,scale_);
}
