#ifndef COPYX
#define COPYX

int copy_file(const char *src, const char *dst);

int copy_file_fast(const char *src, const char *dst);

int copy_file_fast_range(const char *src, const char *dst);

int copy_strategy( const char *src, const char *dst);

#endif 