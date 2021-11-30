#ifndef BRIEF_BRIEF_TYPES_H
#define BRIEF_BRIEF_TYPES_H

#include <stdint.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

#define i8(x) ((i8) (x))
#define i16(x) ((i16) (x))
#define i32(x) ((i32) (x))
#define i64(x) ((i64) (x))
#define u8(x) ((u8) (x))
#define u16(x) ((u16) (x))
#define u32(x) ((u32) (x))
#define u64(x) ((u64) (x))

#define f32(x) ((f32) (x))
#define f64(x) ((f64) (x))

#ifndef bool
typedef u8 bool;
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifdef BRIEF_DEBUG
#define ASSERT(Expression) \
	if (!(Expression)) {   \
		abort();           \
	}
#else
#define ASSERT(Expression)
#endif

typedef struct Vector2 Vector2;
struct Vector2 {
	union {
		struct {
			f32 x;
			f32 y;
		};
		f32 data[2];
	};
};

typedef struct Vector3 Vector3;
struct Vector3 {
	union {
		struct {
			f32 x;
			f32 y;
			f32 z;
		};
		f32 data[3];
	};
};

typedef struct Vector4 Vector4;
struct Vector4 {
	union {
		struct {
			f32 x;
			f32 y;
			f32 z;
			f32 w;
		};
		f32 data[4];
	};
};

#endif//BRIEF_BRIEF_TYPES_H
