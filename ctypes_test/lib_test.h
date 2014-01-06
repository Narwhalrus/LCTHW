#ifndef _LIB_TEST_H_
#define _LIB_TEST_H_
#ifdef __cplusplus
extern "C" {
#endif

struct TStruct 
{
    int a;
    char b;
    float c;
    double d;
};

struct TStruct2
{
    int a;
    char spares[10];
    int b;
};

void *create_memory_space(void);
struct TStruct *create_struct(void);
void *create_struct_v(void);
struct TStruct2 *create_struct2(void);
void print_struct(struct TStruct *mem);
void print_struct2(struct TStruct2 *mem);
void free_struct(struct TStruct *mem);
void alter_memory(void *buffer, unsigned int offset);
void get_memory(void *buffer, unsigned int offset); 
void destroy_memory_space(void *buffer);

#ifdef __cplusplus
}
#endif
#endif /* _LIB_TEST_H_ */
