#include <data.h>

using namespace NDATA;

/**
 * @brief
 */
const char* Data::PrintTextureName() const
{
    switch (payload.texture)
	{
	case TEXTURES::TEXTURE_0:
		return "Texture 1";
	case TEXTURES::TEXTURE_1:
		return "Texture 2";
	default:
		return nullptr;
	}
}
