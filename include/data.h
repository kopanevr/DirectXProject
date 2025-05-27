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
    uint8_t fps                         = 0U;

	//

	TEXTURES texture                    = TEXTURES::TEXTURE_0;

	Payload();
    Payload(const Payload&);
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
    bool IsChanged()                    const;
#if __cplusplus > 201703L
	[[nodiscard]]
#endif
	const char* PrintTextureName()      const;

    Data()                              = default;
    ~Data()                             = default;

    Data(const Data&);
};
}