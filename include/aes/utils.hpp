/* this file is a part of AES project which is under the MIT license; See LICENSE for more info */

#pragma once

#include <vector>
#include <aes/types.hpp>

namespace aes
{
	uint64_t chash(const std::vector<Component>& components);
}