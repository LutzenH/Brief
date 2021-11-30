#include "brief_kdtree.h"

#include <stdlib.h>

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

int kdtree_sort_node_axis(const KDTreeNode* node_a, const KDTreeNode* node_b, i32 axis) {
	f32 a = node_a->position.data[axis];
	f32 b = node_b->position.data[axis];

	return (a < b) ? -1 : ((a > b) ? 1 : 0);
}

int kdtree_sort_node_axis_x(const void* node_a, const void* node_b) { return kdtree_sort_node_axis(node_a, node_b, 0); }
int kdtree_sort_node_axis_y(const void* node_a, const void* node_b) { return kdtree_sort_node_axis(node_a, node_b, 1); }
int kdtree_sort_node_axis_z(const void* node_a, const void* node_b) { return kdtree_sort_node_axis(node_a, node_b, 2); }
static int (*kdtree_sort_axis_function_table[3])(const void*, const void*) = {
		kdtree_sort_node_axis_x,
		kdtree_sort_node_axis_y,
		kdtree_sort_node_axis_z,
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

KDTreePointId kdtree_nearest(KDTreeHandle* tree, Vector3 position, f32* distance) {


	return KDTreePointId_INVALID;
}

i32 kdtree_nearest_n(KDTreeHandle* tree, Vector3 position, KDTreePointId* out_neighbours, f32* out_neighbours_distances, i32 out_neighbours_max) {
	return 0;
}

i32 kdtree_nearest_radius(KDTreeHandle* tree, Vector3 position, f32 radius, KDTreePointId* out_neighbours, f32* out_neighbours_distances, i32 out_neighbours_max) {
	return 0;
}

i32 kdtree_nearest_aabb(KDTreeHandle* tree, Vector3 position, Vector2 aabb, KDTreePointId* out_neighbours, f32* out_neighbours_distances, i32 out_neighbours_max) {
	return 0;
}
