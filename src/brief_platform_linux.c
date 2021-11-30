#include "brief_platform.h"

#include <errno.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

u8* platform_file_load_in_memory(const char* path, size_t* file_size, bool add_trailing_zero) {
	// Read file into memory
	FILE* model_file = fopen(path, "rb");
	if (model_file == NULL) {
		*file_size = 0;
		return NULL;
	}
	fseek(model_file, 0, SEEK_END);
	size_t size = ftell(model_file);
	fseek(model_file, 0, SEEK_SET);

	if (add_trailing_zero) {
		size++;
	}

	u8* buffer = (u8*) malloc(size);
	fread(buffer, size, 1, model_file);
	fclose(model_file);

	if (add_trailing_zero) {
		buffer[size - 1] = '\0';
	}

	*file_size = size;
	return buffer;
}

bool platform_file_write_to_disk(const char* path, u8* data, size_t data_size) {
	FILE* file = fopen(path, "wb");

	if (file) {
		fwrite(data, data_size, 1, file);
		fclose(file);

		return true;
	} else {
		fprintf(stderr, "Failed to write data to file: %s\n", path);

		return false;
	}
}

bool platform_folder_create(const char* path) {
	int result = mkdir(path, 0777);

	if (result == -1) {
		i32 status = errno;
		if (status != EEXIST) {
			fprintf(stderr, "Failed to create folder: %s, reason: %s\n", path, strerror(status));
			return false;
		}
	}

	return true;
}

bool platform_folder_exists(const char* path) {
	struct stat sb;
	if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode)) {
		return true;
	} else {
		return false;
	}
}
