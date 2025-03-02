/* this file is a part of AES project which is under the MIT license; See LICENSE for more info */

#pragma once

#include <unordered_map>
#include <vector>
#include <aes/types.hpp>

namespace aes
{
	class World
	{
	public:
		World();

		Entity entity(); /* creates or reuses an entity */

		void despawn(Entity entity); /* despawn an entity and put them in the pool */

		Component component();

		/* utils; for testing or debug purposes or actual utils */
		static Entity encode_entity(uint64_t id, Generation gen); /* binary encoding */

		static uint64_t get_eid(Entity entity); /* gets an unmasked entity id */

		static Generation get_egen(Entity entity); /* gets an unmasked entity generation */

	private:
		/* members are ordered by their sizes */
		std::unordered_map<Entity, Generation> generations;  /* a sparse set to track decoded entity's id */
		std::unordered_map<uint64_t, size_t> entity_indices; /* maps entity ids to their index position in the
																								entity pool array*/
		std::vector<Entity> entity_pool;                     /* available ids */
		uint64_t alive_count;                                /* the current number of alive & active entity */
		uint64_t next_eid;                                   /* next entity id */
		uint16_t next_cid{};                                   /* next component id */
	};
}
