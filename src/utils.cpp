/* this file is a part of AES project which is under the MIT license; See LICENSE for more info */

#include <aes/utils.hpp>

namespace aes
{
	uint64_t chash(const std::vector<Component>& components)
	{
		constexpr static auto RATIO = 0x6a09e667f3bcc908ULL; /* golden ratio */
		auto hash = 0xcbf29ce484222325ULL;
		const size_t n = components.size();
		for (size_t i = 0; i < n; ++i)
		{
			hash ^= static_cast<uint64_t>(components[i]) * RATIO;
			hash = (hash << 32) | (hash >> (64 - 32));
			hash *= RATIO;
		}

		hash ^= hash >> 33;
		hash *= 0xFF51AFD7ED558CCDULL;
		hash ^= hash >> 33;
		hash *= 0xC4CEB9FE1A85EC53ULL;
		hash ^= hash >> 33;
		return hash;
	}
}