/* this file is a part of AES project which is under the MIT license; See LICENSE for more info */

#include <unordered_set>
#include <aes/world.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("single entity creation", "lifecycle")
{
	aes::World world;
	REQUIRE_NOTHROW(world.entity());
}

TEST_CASE("recycling", "lifecycle")
{
	aes::World world;

	const auto first = world.entity();
	world.despawn(first);
	const auto recycled = world.entity();

	/* recycled entity should have different generation but same underlying id*/
	CHECK(world.get_eid(first) == world.get_eid(recycled));
	CHECK(world.get_egen(first) != world.get_egen(recycled));
}

TEST_CASE("multiple entity creation", "lifecycle")
{
	aes::World world;
	std::vector<aes::Entity> entities;

	entities.reserve(100);
	for (auto i = 0; i < 100; ++i)
		entities.emplace_back(world.entity());

	std::unordered_set<uint64_t> uniqueIds;
	for (const auto entity : entities)
		uniqueIds.insert(aes::World::get_eid(entity));

	CHECK(uniqueIds.size() == 100);
}

TEST_CASE("respawn", "lifecycle")
{
	aes::World world;

	const auto e1 = world.entity();
	const auto e2 = world.entity();
	const auto e3 = world.entity();

	world.despawn(e1);
	world.despawn(e2);
	world.despawn(e3);

	const auto reused = world.entity();
	CHECK(world.get_eid(reused) == world.get_eid(e3));
	CHECK(world.get_egen(reused) != world.get_egen(e3));
}
