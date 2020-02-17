#include <stdlib.h>

#define MSS_BY_MALLOC 0
#define MSS_BY_FREE 1
void global_list_init(void);
void mm_leak_detect(void);
int wild_ptr_detect(void *__ptr);
void *mss_malloc (size_t __size, int type, \
                const char *file, const char *func, int line);
void *mss_free(void *__ptr, int type, \
                const char *file, const char *func, int line);

#define malloc(s) mss_malloc(s,MSS_BY_MALLOC, \
__FILE__, __PRETTY_FUNCTION__, __LINE__)
#define free(p) mss_free(p,MSS_BY_FREE, \
__FILE__, __PRETTY_FUNCTION__, __LINE__)
