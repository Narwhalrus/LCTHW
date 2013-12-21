#ifndef _LIB_TEST_H_
#define _LIB_TEST_H_
#ifdef __cplusplus
extern "C" {
#endif

void *create_memory_space(void);
void alter_memory(void *buffer, size_t offset);
void destroy_memory_space(void *buffer);

#ifdef __cplusplus
}
#endif
#endif /* _LIB_TEST_H_ */
