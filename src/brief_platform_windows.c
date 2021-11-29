#include "brief_platform.h"

#include <stdio.h>
#include <Windows.h>
#include <WinUser.h>

u8* platform_file_load_in_memory(const char* path, size_t* file_size, bool add_trailing_zero)
{
    HANDLE file_handle = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file_handle == INVALID_HANDLE_VALUE)
    {
        *file_size = 0;
        return NULL;
    }

    DWORD win_file_size = GetFileSize(file_handle, NULL);

    u8* buffer = (u8*)malloc(win_file_size + (add_trailing_zero ? 1 : 0));

    DWORD number_of_bytes_read = 0;
    bool success = ReadFile(file_handle, buffer, win_file_size, &number_of_bytes_read,NULL);
    CloseHandle(file_handle);
    if(!success) {
        fprintf(stderr, "Failed to read file: %s , reason: GetLastError=%08lx\n", path, GetLastError());
        *file_size = 0;
        return NULL;
    }

    ASSERT(number_of_bytes_read == win_file_size);

    if(add_trailing_zero) {
        buffer[win_file_size] = '\0';
    }

    *file_size = win_file_size;
    return buffer;
}

bool platform_file_write_to_disk(const char* path, u8* data, size_t data_size)
{
    HANDLE file_handle = CreateFile(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file_handle == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    DWORD number_of_bytes_written = 0;
    bool success = WriteFile(file_handle, data, data_size, &number_of_bytes_written, NULL);
    CloseHandle(file_handle);
    if(!success) {
        fprintf(stderr, "Failed to write file: %s , reason: GetLastError=%08lx\n", path, GetLastError());
        return false;
    }

    ASSERT(data_size == number_of_bytes_written);

    return true;
}

bool platform_folder_create(const char* path)
{
    BOOL success = CreateDirectoryA(
            path,
            NULL
    );

    return (success || GetLastError() == ERROR_ALREADY_EXISTS);
}

bool platform_folder_exists(const char* path)
{
    DWORD file_type = GetFileAttributesA(path);
    if (file_type == INVALID_FILE_ATTRIBUTES) {
        return false;
    }

    if (file_type & FILE_ATTRIBUTE_DIRECTORY) {
        return true;
    }

    return false;
}
