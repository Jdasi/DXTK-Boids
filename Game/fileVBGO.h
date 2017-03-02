#pragma once
#include "VBGO.h"

//=================================================================
//create a VBGO using data from a file 
//format is a sort-of a simplified version of the obj format
//=================================================================

class FileVBGO : public VBGO
{
public:
	FileVBGO(const std::string& _file_name, ID3D11Device* _GD);
	virtual ~FileVBGO() = default;

private:
	bool tex_coords_;
	bool colour_;

};
