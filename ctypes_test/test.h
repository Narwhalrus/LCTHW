#include <stdio.h>
#include <stdlib.h>

struct test_struct {
    int a;
    unsigned int b;
    char c;
    float d;
    double e;
    int f[10];

    struct {
        int a;
        int b;
        float c;
    } structy[2];

    struct {
        int lel;
        float lel2;
        double lel3[4];
    } another[2];

    char wub;
};

int halloFnc(int a);
int anotherOne(int a);
int OhBizzoy(int a);

struct another_struct {
    int stuff;
    char stuffArr[20];
};
