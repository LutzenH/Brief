#ifndef BRIEF_BRIEF_PLATFORM_H
#define BRIEF_BRIEF_PLATFORM_H

#include "brief_types.h"

u8* platform_file_load_in_memory(const char* path, size_t* file_size, bool add_trailing_zero);
bool platform_file_write_to_disk(const char* path, u8* data, size_t data_size);
bool platform_folder_create(const char* path);
bool platform_folder_exists(const char* path);

#endif //BRIEF_BRIEF_PLATFORM_H
