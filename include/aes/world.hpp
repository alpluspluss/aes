/* this file is a part of AES project which is under the MIT license; See LICENSE for more info */

#pragma once

#include <typeindex>
#include <unordered_map>
#include <vector>
#include <aes/types.hpp>
#include <aes/utils.hpp>

namespace aes
{
	class World
	{
	public:
		World(); /* initialize world as resources */

		[[nodiscard]] Entity entity(); /* creates or reuses an entity */

		void despawn(Entity entity); /* despawn an entity and put them in the pool */

		template<typename T>
		World *set(Entity entity, T data)
		{
			auto cid = get_cid<T>();
			const auto arch_it = entity_arch_map.find(entity);

			/* if the entity doesn't exist in any archetype */
			if (arch_it == entity_arch_map.end())
			{
				Archetype new_arch;
				new_arch.components.emplace_back(cid);

				/* allocate space for component data */
				/* note that this only works for trivial types; c++ object is way more complicated */
				void* cdata = new T(data);
				new_arch.data.emplace_back(cdata);

				/* add the entity to the newly created archetype */
				new_arch.entities.emplace_back(entity);
				new_arch.entity_rows[entity] = 0;

				/* compute archetype identifier */
				new_arch.id = chash(new_arch.components);

				/* then store the archetype */
				archetypes[new_arch.id] = new_arch;
				entity_arch_map[entity] = &archetypes[new_arch.id];
				return this;
			}

			Archetype* current_arch = arch_it->second;
			auto comp_it = std::find(current_arch->components.begin(),
						current_arch->components.end(),
						cid); /* replacable with a binary search */

			/* if the entity exists but the component hasn't been updated */
			if (comp_it != current_arch->components.end())
			{
				const size_t index = std::distance(current_arch->components.begin(), comp_it);

				/* note that this only works for trivial types;
				 * c++ object is way more complicated */
				delete static_cast<T*>(current_arch->data[index]);
				current_arch->data[index] = new T(data);
				return this;
			}

			/* create a new archetype with additional component */
			Archetype new_arch = *current_arch;
			new_arch.components.emplace_back(cid);

			/* allocate space for component data */
			/* again, this only works for trivial types; c++ object is way more complicated */
			void* ndata = new T(data);
			new_arch.data.emplace_back(ndata);
			new_arch.id = chash(new_arch.components);

			/* store the new archetype */
			archetypes[new_arch.id] = new_arch;
			entity_arch_map[entity] = &archetypes[new_arch.id];
			return this;
		}

		/* utils; for testing or debug purposes or actual utils */
		static Entity encode_entity(uint64_t id, Generation gen); /* binary encoding */

		static uint64_t get_eid(Entity entity); /* gets an unmasked entity id */

		static Generation get_egen(Entity entity); /* gets an unmasked entity generation */

	private:
		/* some private definitions */
		struct Archetype
		{
			std::unordered_map<Entity, size_t> entity_rows; /* maps entity id to rows in component arrays */
			std::vector<Entity> entities;                   /* entity of the same type */
			std::vector<Component> components;              /* list of component ids in this achetype */
			std::vector<void *> data;                       /* component data storage */
			uint64_t id;                                    /* the hash id calculated from the component set */
		};

		/* private methods */
		template<typename T>
		Component get_cid()
		{
			/* use the type */
			if (const auto it = component_types.find(typeid(T));
				it != component_types.end())
			{
				return it->second;
			}
			return next_cid++; /* returns a new id */
		}

		/* members are ordered by their sizes and access frequency */
		std::unordered_map<Entity, Generation> generations; /* a sparse set to track decoded entity's id */
		std::unordered_map<uint64_t, size_t> entity_indices; /* maps entity ids to their index position in the
																								entity pool array*/
		std::unordered_map<Entity, Archetype *> entity_arch_map; /* quick entity location lookup */
		std::unordered_map<uint64_t, Archetype> archetypes; /* all archetype indexed by hash */
		std::unordered_map<std::type_index, Component> component_types; /* map component type to component id */

		std::vector<Entity> entity_pool; /* available ids */
		Archetype *empty_arch {}; /* special case for entities with no components */
		uint64_t alive_count; /* the current number of alive & active entity */
		uint64_t next_eid; /* next entity id */
		uint16_t next_cid {}; /* next component id */
	};
}
