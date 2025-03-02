/* this file is a part of AES project which is under the MIT license; See LICENSE for more info */

#include <aes/types.hpp>
#include <aes/world.hpp>

namespace aes
{
	constexpr uint64_t ENTITY_MASK = 0x0000FFFFFFFFFFFF; /* 48 lower bits for entity id */
	constexpr uint64_t GENERATION_SHIFT = 48; /* we need to shift 16 bits upper to accommodate the entity bits */
	constexpr Generation MAX_GENERATION = 0xFFFF; /* for 16-bit generation */

	World::World() : alive_count(0), next_id(0) {} /* initializes our world */

	/* this method creates a new entity by either reusing a previously deleted one or a previously deleted one */
	Entity World::entity()
	{
		Entity entity;
		Generation gen;

		if (alive_count < entity_pool.size())
		{
			/* recycling */
			entity = entity_pool[alive_count]; /* get the entity id to recycle */

			const auto it = generations.find(entity);
			gen = (it != generations.end() ? it->second : 0) + 1;

			/* wrap around if generation is overflow */
			gen = gen > MAX_GENERATION /* 16-bit */ ? 0 : gen;
		}
		else
		{
			/* newborn path */
			entity = next_id;
			++next_id;
			gen = 0;

			/* add to the pool */
			entity_pool.emplace_back(entity);
		}

		++alive_count;
		generations[entity] = gen;
		return encode_entity(entity, gen);
	}

	/* finalizes the entity creation by masking the entity id and generation with their mask counterparts */
	Entity World::encode_entity(const uint64_t id, const Generation gen)
	{
		return (static_cast<Entity>(gen) << GENERATION_SHIFT) | (id & ENTITY_MASK);
	}

	/* note: the 'entity' parameter is encoded */
	void World::despawn(Entity entity)
	{
		/* get the metadata from the encoded id; this maybe be optimized to make it return a pair instead
		 * to reduce function call overhead */
		uint64_t entity_id = get_eid(entity);
		Generation gen = get_egen(entity);

		/* check if the entity exists with valid generation */
		auto it = generations.find(entity_id);
		if (it == generations.end() || it->second != gen)
			return;

		/* find the entity in the pool */
		size_t index = 0;
		for (size_t i = 0; i < alive_count; ++i)
		{
			if (entity_pool[i] == entity_id)
			{
				index = i;
				break;
			}
		}

		/* move the last alive entity to this position if it's not */
		if (index < alive_count - 1)
			entity_pool[index] = entity_pool[alive_count - 1];

		entity_pool[alive_count - 1] = entity_id; /* place the destroyed entity at the end of the alive section */
		--alive_count; /* an entity dies so we decrease this value */

		/* increment the generation for future reuse; wrap around if it exceeds MAX_GENERATION */
		generations[entity_id] = (generations[entity_id] + 1) > MAX_GENERATION ? 0 : generations[entity_id] + 1;
	}

	/* gets an unmasked entity id */
	uint64_t World::get_eid(const Entity entity)
	{
		return entity & ENTITY_MASK;
	}

	/* gets an unmasked entity generation */
	Generation World::get_egen(const Entity entity)
	{
		return static_cast<Generation>(entity >> GENERATION_SHIFT);
	}
}