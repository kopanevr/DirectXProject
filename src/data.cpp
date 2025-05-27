#include <data.h>

using namespace NDATA;

/**
 * @brief
 */
Payload::Payload()
{}

/**
 * @brief
 */
Payload::Payload(const Payload&)
{}

/**
 * @brief
 */
bool Data::IsChanged() const
{
	return true;
}

/**
 * @brief
 */
const char* Data::PrintTextureName() const
{
    switch (payload.texture)
	{
	case TEXTURES::TEXTURE_0:
		return "Текстура 1";
	case TEXTURES::TEXTURE_1:
		return "Текстура 2";
	default:
		return nullptr;
	}
}

/**
 * @brief
 */
Data::Data(const Data& other)
{
    if (other.IsChanged() == true) { payload = other.payload; }
}
