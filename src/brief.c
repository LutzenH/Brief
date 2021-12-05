#include "brief_kdtree.h"
#include "brief_platform.h"
#include "brief_types.h"
#include "brief_visualization.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
	// Setup.
	const i32 point_count = 131072;
	Vector3* random_data = calloc(point_count, sizeof(Vector3));
	KDTreePointId* random_data_identifiers = calloc(point_count, sizeof(i32));
	for (i32 i = 0; i < point_count; ++i) {
		Vector3* position = &random_data[i];
		position->x = f32(rand()) / f32(RAND_MAX / 8192.0f) - 4096.0f;
		position->y = 0.0f;
		position->z = f32(rand()) / f32(RAND_MAX / 8192.0f) - 4096.0f;

		random_data_identifiers[i] = i;
	}
#if defined(BRIEF_DEBUG)
	VisualizationScatterInfo info = {
			512,
			256,
			{-1024.0f, 1024.0f},
			{-512.0f, 512.0f},
			{15, 15, 15},
	};
	VisualizationScatter* scatter = visualization_scatter_create(&info);

	VisualizationScatterSeries series_all = {
			{40, 40, 40},
			point_count,
			&random_data[0].data[VECTOR_AXIS_X],
			sizeof(Vector3),
			&random_data[0].data[VECTOR_AXIS_Z],
			sizeof(Vector3),
	};
	visualization_scatter_series_add(scatter, &series_all);
#endif

	// Example: Allocate and create tree.
	KDTreeHandle* tree = kdtree_create(random_data, random_data_identifiers, point_count);

	// The 'random_data' and 'random_data_identifiers' are actually no longer needed beyond this point,
	//  but they are used as look-up for the VisualizationScatter.

	// Example: Serialize, de-allocate and re-allocate tree.
	{
		const char* file_path = "random_data.tree";
		bool success = kdtree_serialize(tree, file_path);
		kdtree_destroy(tree);
		tree = kdtree_deserialize(file_path);
	}

	// Example: Nearest neighbour.
	{
		const Vector3 position = {0.0f, 0.0f, 0.0f};
		f32 distance = 0.0f;
		KDTreePointId nearest_point_id = kdtree_nearest(tree, position, &distance);
	}

	// Example: Nearest n neighbours.
	{
		const Vector3 position = {-512.0f, 0.0f, 0.0f};

		KDTreePointId neighbours[64];
		f32 neighbours_distances[64];
		i32 nearest_point_count = kdtree_nearest_n(tree, position, neighbours, neighbours_distances, 64);

#if defined(BRIEF_DEBUG)
		Vector2 neighbour_positions[64];
		for (i32 i = 0; i < nearest_point_count; ++i) {
			neighbour_positions[i].x = random_data[neighbours[i]].x;
			neighbour_positions[i].y = random_data[neighbours[i]].z;
		}

		VisualizationScatterSeries series_n_neighbours = {
				{0, 255, 0},
				nearest_point_count,
				&neighbour_positions[0].data[VECTOR_AXIS_X],
				sizeof(Vector2),
				&neighbour_positions[0].data[VECTOR_AXIS_Y],
				sizeof(Vector2),
		};
		visualization_scatter_series_add(scatter, &series_n_neighbours);
#endif
	}

	// Example: Nearest radius neighbours.
	{
		const Vector3 position = {0.0f, 0.0f, 0.0f};
		const f32 radius = 256.0f;

		KDTreePointId neighbours[512];
		f32 neighbours_distances[512];
		i32 nearest_point_count = kdtree_nearest_radius(tree, position, radius, neighbours, neighbours_distances, 512);

#if defined(BRIEF_DEBUG)
		Vector2 neighbour_positions[512];
		for (i32 i = 0; i < nearest_point_count; ++i) {
			neighbour_positions[i].x = random_data[neighbours[i]].x;
			neighbour_positions[i].y = random_data[neighbours[i]].z;
		}

		VisualizationScatterSeries series_radius_neighbours = {
				{0, 0, 255},
				nearest_point_count,
				&neighbour_positions[0].data[VECTOR_AXIS_X],
				sizeof(Vector2),
				&neighbour_positions[0].data[VECTOR_AXIS_Y],
				sizeof(Vector2),
		};
		visualization_scatter_series_add(scatter, &series_radius_neighbours);
#endif
	}

	// Example: Within AABB detection.
	{
		const Vector3 min = {512.0f + -128.0f, -128.0f, -128.0f};
		const Vector3 max = {512.0f + 128.0f, 128.0f, 128.0f};

		KDTreePointId neighbours[512];
		f32 neighbours_distances[512];
		i32 nearest_point_count = kdtree_nearest_aabb(tree, min, max, neighbours, neighbours_distances, 512);

#if defined(BRIEF_DEBUG)
		Vector2 neighbour_positions[512];
		for (i32 i = 0; i < nearest_point_count; ++i) {
			neighbour_positions[i].x = random_data[neighbours[i]].x;
			neighbour_positions[i].y = random_data[neighbours[i]].z;
		}

		VisualizationScatterSeries series_aabb_neighbours = {
				{255, 0, 0},
				nearest_point_count,
				&neighbour_positions[0].data[VECTOR_AXIS_X],
				sizeof(Vector2),
				&neighbour_positions[0].data[VECTOR_AXIS_Y],
				sizeof(Vector2),
		};
		visualization_scatter_series_add(scatter, &series_aabb_neighbours);
#endif
	}

	free(random_data);
	free(random_data_identifiers);

#if defined(BRIEF_DEBUG)
	visualization_scatter_save(scatter, "debug_visualization_scatter_xz.png");
	visualization_scatter_destroy(scatter);
#endif

	// Deallocate tree.
	kdtree_destroy(tree);

	return 0;
}
