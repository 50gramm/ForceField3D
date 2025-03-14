#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// file
size_t platform_read_asset(const char *name, void** pbuff);

int platform_read_file(const char *name, void** pbuff);

int platform_write_file(const char *name, const void* buff, int size);

#ifdef __cplusplus
}
#endif
