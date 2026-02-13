#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sys/resource.h>

double get_cpu_time();

double get_full_time();



struct Args{
    int p;
    int count;
    double sum;
    char *filename;
    int tid;
    bool ret = false;
    bool ret1 = false;
    double time;
    int res = 0;
};

void synchronize(int p, double *a, int n);



void* func(void *args);


