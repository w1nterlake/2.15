#include "header.h"

int main(int argc, char *argv[]){
    int p = argc - 1;
    pthread_t *th = (pthread_t*)malloc((p - 1) * sizeof(pthread_t));
    Args *args = (Args*)malloc(p * sizeof(Args));
    double t;
    for(int i = 0; i < p; i++){
        args[i].count = 0;
        args[i].filename = argv[i + 1];
        args[i].p = p;
        args[i].sum = 0;
        args[i].tid = i;
        args[i].res = 0;
        args[i].time = 0;
    }
    t = get_full_time();
    for(int i = 1; i < p; i++){
        pthread_create(th + i - 1, nullptr, func, (void*)(args + i));
    }
    func((void*)args);
    for(int i = 0; i < p - 1; i++){
        pthread_join(th[i], nullptr);
    }
    t = get_full_time() - t;
    
    for(int i = 0; i < p; i++){
        if(!args[i].ret1){
            printf("Num file %d is not correct\n", args[i].tid);
            continue;
        }
        if(!args[i].ret){
            printf("File num = -%d does not exist\n", args[i].tid);
        }
        else{
            printf("File num %d count = %d, time thread = %lf\n", args[i].tid, args[i].res, args[i].time);
        }
    }

    free(th);
    free(args);
}