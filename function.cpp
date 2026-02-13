#include "header.h"

pthread_mutex_t t = PTHREAD_MUTEX_INITIALIZER;

double get_cpu_time()
{
  struct rusage buf;
  getrusage(RUSAGE_THREAD, &buf);
  return buf.ru_utime.tv_sec + buf.ru_utime.tv_usec /1.e6;
}

double get_full_time()
{
  struct timeval buf;
  gettimeofday(&buf, 0);
  return buf.tv_sec + buf.tv_usec / 1.e6;
}

void synchronize(int p, double *a, int n){
    static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
    static pthread_cond_t c_in = PTHREAD_COND_INITIALIZER;
    static pthread_cond_t c_out = PTHREAD_COND_INITIALIZER;
    static int t_in = 0;
    static int t_out = 0;
    static double *r = nullptr;
    if(p <= 1){
        return;
    }
    pthread_mutex_lock(&m);
    if(r == nullptr){
        r = a;
    }
    else{
        for(int i = 0; i < n;i++){
            r[i] += a[i];
        }
    }
    t_in++;
    if(t_in >= p){
        t_out = 0;
        pthread_cond_broadcast(&c_in);
    }
    else{
        while(t_in < p){
            pthread_cond_wait(&c_in,&m);
        }
    }
    if(r != a){
        for(int i = 0; i < n; i++){
            a[i] = r[i];
        }
    }
    t_out++;
    if(t_out >= p){
        t_in = 0;
        r = nullptr;
        pthread_cond_broadcast(&c_out);
    }
    else{
        while(t_out < p){
            pthread_cond_wait(&c_out, &m);
        }
    }
    pthread_mutex_unlock(&m);
}


void *func(void* args){
    Args *tmp = (Args*) args;
    FILE *fin;
    fin = fopen(tmp->filename, "r");
    double current, last_last, last, size = 0, count = 0;
    bool ok = true;
    int res = 0;
    bool fl = true;
    bool tmp_fl = false;
    double t = get_cpu_time();
    if(fin == nullptr){
        ok = false;
    }
    else{
        while(fscanf(fin, "%lf", &current) == 1){
            size++;
            if(size > 1){
                if(last == current){
                    count++;
                    tmp_fl = true;
                }
                else if(last != current){
                    tmp->count += count;
                    tmp->sum += count * last;
                    count = 0;
                    tmp_fl = false;
                }
            }
            if(size <= 2){
                last = current;
            }
            else if(size > 2){
                last_last = last;
                last = current;
            }
        }

        if(tmp_fl){
            tmp->count += count;
            tmp->sum += count * last;
            count = 0;
        }

        if(!feof(fin)){
            fl = false;
        }
        // if(last_last == last){
        //     count++;
        // }
        // else{

        // }
        synchronize(tmp->p, &(tmp->count), 1);
        double mid = tmp->sum / tmp->count;

        fseek(fin, 0, SEEK_SET);
        
        while(fscanf(fin, "%lf", &current) == 1){
            if(current > mid){
                res++;
            }
        }
    }







    // if (fscanf(fin, "%lf", &last) != 1) {
    //     if (!feof(fin)){
    //         fl = false;
    //     }
    //     else{
    //     size = 1;
    //     count = 0;

    //     while (fscanf(fin, "%lf", &current) == 1) {
    //         size++;
    //         if (current == last) {
    //             count++;
    //         }
    //         else{
    //             tmp->count += count;
    //             tmp->sum  += count * last;
    //             count = 0;
    //             last = current;
    //         }
    //     }
    //     tmp->count += count;
    //     tmp->sum  += count * last;

    //     if (!feof(fin)){
    //         fl = false;
    //     }
    // }
    

    if(ok){
        fclose(fin);
    }
    tmp->ret = ok;
    tmp->ret1 = fl;
    t = get_cpu_time() - t;
    tmp->time = t;
    tmp->res = res;
    return NULL;
}