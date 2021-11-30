#include "brief_kdtree.h"
#include "brief_platform.h"
#include "brief_types.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
	// Setup
	const i32 point_count = 131072;
	Vector3* random_data = calloc(point_count, sizeof(Vector3));
	i32* random_data_identifiers = calloc(point_count, sizeof(i32));
	for (i32 i = 0; i < point_count; ++i) {
		Vector3* position = &random_data[i];
		position->x = f32(rand()) / f32(RAND_MAX / 8192.0f) - 4096.0f;
		position->y = f32(rand()) / f32(RAND_MAX / 256.0f) - 128.0f;
		position->z = f32(rand()) / f32(RAND_MAX / 8192.0f) - 4096.0f;
	}

	// Allocate and create tree
	KDTreeHandle* tree = kdtree_create(random_data, random_data_identifiers, point_count);

	// No longer needed beyond this point
	free(random_data);
	free(random_data_identifiers);


	// Deallocate tree
	kdtree_destroy(tree);

	return 0;
}
