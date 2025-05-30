 /**
 * @file data.h
 * @brief
 */

#pragma once

#include <cstdint>

namespace NDATA
{
enum class TEXTURES	: uint8_t
{
	TEXTURE_0 = 0U,
	TEXTURE_1

	//

	, TEXTURES_NUM
};

struct Payload final
{
    uint8_t fps                         	= 0U;

	uint16_t width				= 0U;
	uint16_t height				= 0U;

	//

	TEXTURES texture			= TEXTURES::TEXTURE_0;

	float x						= 0.0f;
	float y						= 0.0f;
};

class Data final
{
public:
	Payload payload;

	//
public:
#if __cplusplus > 201703L
	[[nodiscard]]
#endif
	const char* PrintTextureName()		const;

    Data()					= default;
    ~Data()					= default;
};

static Data data = {};
}
