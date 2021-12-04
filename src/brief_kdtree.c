#include "brief_kdtree.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

typedef struct KDTreeNode KDTreeNode;
struct KDTreeNode {
	Vector3 position;
	KDTreePointId identifier;
	KDTreeNode* left;
	KDTreeNode* right;
};

struct KDTreeHandle {
	KDTreeNode* root;
	u32 length;
	KDTreeNode* nodes;
};

Vector3 kdtree_vector3_subtract(const Vector3* vector_a, const Vector3* vector_b) {
	Vector3 value;
	value.x = vector_a->x - vector_b->x;
	value.y = vector_a->y - vector_b->y;
	value.z = vector_a->z - vector_b->z;

	return value;
}

Vector3 kdtree_vector3_pow(const Vector3* vector, const Vector3* power) {
	Vector3 value;
	value.x = powf(vector->x, power->x);
	value.y = powf(vector->y, power->y);
	value.z = powf(vector->z, power->z);

	return value;
}

i32 kdtree_sort_node_axis(const KDTreeNode* node_a, const KDTreeNode* node_b, i32 axis) {
	f32 value = node_a->position.data[axis] - node_b->position.data[axis];
	return (value < 0.0f) ? -1 : ((value > 0.0f) ? 1 : 0);
}

int kdtree_sort_node_axis_x(const void* node_a, const void* node_b) { return kdtree_sort_node_axis(node_a, node_b, VECTOR_AXIS_X); }
int kdtree_sort_node_axis_y(const void* node_a, const void* node_b) { return kdtree_sort_node_axis(node_a, node_b, VECTOR_AXIS_Y); }
int kdtree_sort_node_axis_z(const void* node_a, const void* node_b) { return kdtree_sort_node_axis(node_a, node_b, VECTOR_AXIS_Z); }
int kdtree_sort_node_axis_w(const void* node_a, const void* node_b) { return kdtree_sort_node_axis(node_a, node_b, VECTOR_AXIS_W); }
static int (*kdtree_sort_axis_function_table[4])(const void*, const void*) = {
		kdtree_sort_node_axis_x,
		kdtree_sort_node_axis_y,
		kdtree_sort_node_axis_z,
		kdtree_sort_node_axis_w,
};

KDTreeNode* kdtree_build(KDTreeHandle* handle, u32 start, u32 end, i32 depth) {
	if (end <= start) {
		return NULL;
	}

	// The axis that should be used to sort the nodes.
	i32 axis = depth % 3;

	// Sort the nodes between 'start' and 'start + end'.
	qsort(handle->nodes + start, end - start, sizeof(KDTreeNode), kdtree_sort_axis_function_table[axis]);
	u32 median_idx = (start + end) / 2;

	KDTreeNode* median = handle->nodes + median_idx;
	median->left = kdtree_build(handle, start, median_idx, depth + 1);
	median->right = kdtree_build(handle, median_idx + 1, end, depth + 1);

	return median;
}

KDTreeHandle* kdtree_create(const Vector3* points, const KDTreePointId* identifiers, u32 point_count) {
	KDTreeHandle* handle = calloc(1, sizeof(KDTreeHandle) + sizeof(KDTreeNode) * point_count);

	handle->length = point_count;
	handle->nodes = (KDTreeNode*) (((u8*) handle) + sizeof(KDTreeHandle));
	ASSERT((i64(handle->nodes) % sizeof(void*)) == 0);

	for (i32 i = 0; i < handle->length; ++i) {
		KDTreeNode* node = handle->nodes + i;

		node->position = points[i];
		node->identifier = identifiers[i];
	}

	handle->root = kdtree_build(handle, 0, handle->length, 0);

	return handle;
}

bool kdtree_destroy(KDTreeHandle* tree) {
	free(tree);
	return true;
}

void kdtree_nearest_search(KDTreeNode* root_node, Vector3 position, i32 depth, KDTreePointId* out_point_id, f32* out_distance) {
	if (root_node == NULL) {
		return;
	}

	i32 axis = (depth % 3);
	f32 comparison = position.data[axis] - root_node->position.data[axis];

	KDTreeNode* near = NULL;
	KDTreeNode* far = NULL;
	if (comparison <= 0.0f) {
		near = root_node->left;
		far = root_node->right;
	} else {
		near = root_node->right;
		far = root_node->left;
	}

	kdtree_nearest_search(near, position, depth + 1, out_point_id, out_distance);
	if ((comparison * comparison) < *out_distance) {
		kdtree_nearest_search(far, position, depth + 1, out_point_id, out_distance);
	}

	Vector3 difference = kdtree_vector3_subtract(&position, &root_node->position);
	Vector3 power = {2.0f, 2.0f, 2.0f};
	Vector3 difference_squared = kdtree_vector3_pow(&difference, &power);
	f32 distance_squared = difference_squared.x + difference_squared.y + difference_squared.z;

	if (distance_squared < *out_distance) {
		*out_point_id = root_node->identifier;
		*out_distance = distance_squared;
	}
}

KDTreePointId kdtree_nearest(KDTreeHandle* tree, Vector3 position, f32* distance) {
	KDTreePointId id = KDTreePointId_INVALID;
	f32 shortest_distance = INFINITY;

	kdtree_nearest_search(tree->root, position, 0, &id, &shortest_distance);

	*distance = sqrtf(shortest_distance);
	return id;
}

void kdtree_nearest_n_search(KDTreeNode* root_node, Vector3 position, i32 depth, KDTreePointId* out_neighbours, f32* out_neighbours_distances, i32* n_count, i32 n_max, f32* shortest_distance) {
	if (root_node == NULL) {
		return;
	}

	i32 axis = (depth % 3);
	f32 comparison = position.data[axis] - root_node->position.data[axis];

	KDTreeNode* near = NULL;
	KDTreeNode* far = NULL;
	if (comparison <= 0.0f) {
		near = root_node->left;
		far = root_node->right;
	} else {
		near = root_node->right;
		far = root_node->left;
	}

	kdtree_nearest_n_search(near, position, depth + 1, out_neighbours, out_neighbours_distances, n_count, n_max, shortest_distance);
	if ((comparison * comparison) < *shortest_distance) {
		kdtree_nearest_n_search(far, position, depth + 1, out_neighbours, out_neighbours_distances, n_count, n_max, shortest_distance);
	}

	Vector3 difference = kdtree_vector3_subtract(&position, &root_node->position);
	Vector3 power = {2.0f, 2.0f, 2.0f};
	Vector3 difference_squared = kdtree_vector3_pow(&difference, &power);
	f32 distance_squared = difference_squared.x + difference_squared.y + difference_squared.z;

	if (distance_squared < *shortest_distance) {
		i32 start_idx = 0;
		i32 end_idx = *n_count;

		while (start_idx < end_idx) {
			i32 center_idx = (start_idx + end_idx) / 2;

			if (out_neighbours_distances[center_idx] < distance_squared) {
				start_idx = center_idx + 1;
			} else {
				end_idx = center_idx;
			}
		}

		memmove(out_neighbours + start_idx + 1, out_neighbours + start_idx, MAX(0, (*n_count - start_idx - 1)) * sizeof(KDTreePointId));
		memmove(out_neighbours_distances + start_idx + 1, out_neighbours_distances + start_idx, MAX(0, (*n_count - start_idx - 1)) * sizeof(f32));
		out_neighbours[start_idx] = root_node->identifier;
		out_neighbours_distances[start_idx] = distance_squared;

		if (*n_count < n_max) {
			*n_count += 1;
		} else {
			*shortest_distance = out_neighbours_distances[n_max - 1];
		}
	}
}

i32 kdtree_nearest_n(KDTreeHandle* tree, Vector3 position, KDTreePointId* out_neighbours, f32* out_neighbours_distances, i32 out_neighbours_max) {
	for (i32 i = 0; i < out_neighbours_max; ++i) {
		out_neighbours[i] = KDTreePointId_INVALID;
		out_neighbours_distances[i] = INFINITY;
	}

	i32 n_count = 0;
	f32 shortest_distance = INFINITY;
	kdtree_nearest_n_search(tree->root, position, 0, out_neighbours, out_neighbours_distances, &n_count, out_neighbours_max, &shortest_distance);

	for (i32 i = 0; i < n_count; ++i) {
		out_neighbours_distances[i] = sqrtf(out_neighbours_distances[i]);
	}

	return n_count;
}

i32 kdtree_nearest_radius(KDTreeHandle* tree, Vector3 position, f32 radius, KDTreePointId* out_neighbours, f32* out_neighbours_distances, i32 out_neighbours_max) {
	return 0;
}

i32 kdtree_nearest_aabb(KDTreeHandle* tree, Vector3 aabb_min, Vector3 aabb_max, KDTreePointId* out_neighbours, f32* out_neighbours_distances, i32 out_neighbours_max) {
	return 0;
}
