/* this file is a part of AES project which is under the MIT license; See LICENSE for more info */

#include <aes/world.hpp>
#include <catch2/catch_test_macros.hpp>

/* non-trivial */
struct Position
{
	float x, y, z;
};

/* non-trivial */
struct Velocity
{
	float dx, dy, dz;
};

/* non-trivial */
struct Name
{
	std::string value;
};

TEST_CASE("component creation", "component management")
{
	SECTION("adding non-trivial components")
	{
		aes::World world;
		const auto e = world.entity();
		REQUIRE_NOTHROW(world.set<Position>(e, { 1.0F, 2.0F, 3.0F }));
		REQUIRE_NOTHROW(world.set<Velocity>(e, { 1.0F, 2.0F, 3.0F }));
	}

	SECTION("adding name component with dynamic memory")
	{
		aes::World world;
		const auto e = world.entity();
		REQUIRE_NOTHROW(world.set<Name>(e, { "TEST" }));
		REQUIRE_NOTHROW(world.set<Name>(e, { "UPDATE TEST" }));
	}

	SECTION("component overwrite")
	{
		aes::World world;
		const auto e = world.entity();

		world.set<Name>(e, { "INIT" });
		world.set<Name>(e, { "SUPER LONNNNNNNNNNNNNNG" });
	}

	SECTION("mixed trivial and non-trivial")
	{
		aes::World world;
		const auto entity = world.entity();

		world.set<Position>(entity, { 1.0F, 2.0F, 3.0F });
		world.set<Name>(entity, { "MIXED" });
	}
}