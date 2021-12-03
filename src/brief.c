#include "brief_kdtree.h"
#include "brief_platform.h"
#include "brief_types.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
	// Setup
	const i32 point_count = 131072;
	Vector3* random_data = calloc(point_count, sizeof(Vector3));
	KDTreePointId* random_data_identifiers = calloc(point_count, sizeof(i32));
	for (i32 i = 0; i < point_count; ++i) {
		Vector3* position = &random_data[i];
		position->x = f32(rand()) / f32(RAND_MAX / 8192.0f) - 4096.0f;
		position->y = f32(rand()) / f32(RAND_MAX / 256.0f) - 128.0f;
		position->z = f32(rand()) / f32(RAND_MAX / 8192.0f) - 4096.0f;

		random_data_identifiers[i] = i;
	}

	// Allocate and create tree
	KDTreeHandle* tree = kdtree_create(random_data, random_data_identifiers, point_count);

	// No longer needed beyond this point
	free(random_data);
	free(random_data_identifiers);

	// Nearest neighbour
	{
		const Vector3 position = {0.0f, 0.0f, 0.0f};
		f32 distance = 0.0f;
		KDTreePointId nearest_point_id = kdtree_nearest(tree, position, &distance);
	}

	// Nearest n neighbours
	{
		const Vector3 position = {0.0f, 0.0f, 0.0f};

		KDTreePointId neighbours[32];
		f32 neighbours_distances[32];
		i32 nearest_point_count = kdtree_nearest_n(tree, position, neighbours, neighbours_distances, 32);
	}

	// Nearest radius neighbours
	{
		const Vector3 position = {0.0f, 0.0f, 0.0f};
		const f32 radius = 128.0f;

		KDTreePointId neighbours[32];
		f32 neighbours_distances[32];
		i32 nearest_point_count = kdtree_nearest_radius(tree, position, radius, neighbours, neighbours_distances, 32);
	}

	// Within AABB detection
	{
		const Vector3 min = {-64.0f, -64.0f, -64.0f};
		const Vector3 max = {64.0f, 64.0f, 64.0f};

		KDTreePointId neighbours[32];
		f32 neighbours_distances[32];
		i32 nearest_point_count = kdtree_nearest_aabb(tree, min, max, neighbours, neighbours_distances, 32);
	}

	// Deallocate tree
	kdtree_destroy(tree);

	return 0;
}
