#ifndef BRIEF_BRIEF_KDTREE_H
#define BRIEF_BRIEF_KDTREE_H

#include "brief_types.h"

typedef struct KDTreeHandle KDTreeHandle;
typedef i32 KDTreePointId;
#define KDTreePointId_INVALID (-1)

KDTreeHandle* kdtree_create(const Vector3* points, const KDTreePointId* identifiers, u32 point_count);
bool kdtree_destroy(KDTreeHandle* tree);

bool kdtree_serialize(KDTreeHandle* tree, const char* path);
KDTreeHandle* kdtree_deserialize(const char* path);

KDTreePointId kdtree_nearest(KDTreeHandle* tree, Vector3 position, f32* distance);
i32 kdtree_nearest_n(KDTreeHandle* tree, Vector3 position, KDTreePointId* out_neighbours, f32* out_neighbours_distances, i32 out_neighbours_max);
i32 kdtree_nearest_radius(KDTreeHandle* tree, Vector3 position, f32 radius, KDTreePointId* out_neighbours, f32* out_neighbours_distances, i32 out_neighbours_max);
i32 kdtree_nearest_aabb(KDTreeHandle* tree, Vector3 aabb_min, Vector3 aabb_max, KDTreePointId* out_neighbours, f32* out_neighbours_distances, i32 out_neighbours_max);

#endif//BRIEF_BRIEF_KDTREE_H
