#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

int m, n;             
double *partial_sums;               
pthread_t *threads;

typedef struct
{
    int tid;   // thread id
    int start; // starting number
    int end;   // ending number
} thread_arg_t;

void *threadCalc(void *arg){
    thread_arg_t *targ = (thread_arg_t *)arg;
    int tid = targ->tid;
    int start = targ->start;
    int end = targ->end;
    double partial = 0.0;

    //calculate the thread's partial sum
    for (int i = start; i <= end; i++)
    {
        partial += sqrt(i);
    }

    partial_sums[tid] = partial;

    //determine reduction rounds
    int r = 0;
    //
    while ((1 << r) < m) r++;

    //reduce and combine with other threads r amount of times
    for (int x = 1; x <= r ; x++){
        //calculate partner with bitshift operations
        int partner = tid ^ (1 << (r - x));

        if (tid < partner) {
            //if active thread, combine threads and partial sum
            pthread_join(threads[partner], NULL);
            partial_sums[tid] += partial_sums[partner];
        }else{
            //non active thread
            return NULL;
        }
    
    }
    if (tid == 0) {
        printf("Final sum: %.6f\n", partial_sums[0]);
    }

    return NULL;

}

int main (int argc, char *argv[]){

    m = atoi(argv[1]);
    n = atoi(argv[2]);

    int chunk = n / m;

    threads = malloc(m * sizeof(pthread_t));
    partial_sums = malloc(m * sizeof(double));

    thread_arg_t args[m];


    for (int i = m-1; i >= 0 ; i--){

        args[i].tid = i;
        args[i].start = i * chunk + 1;
        args[i].end = (i + 1) * chunk;

        if (pthread_create(&threads[i], NULL, threadCalc, &args[i]) != 0)
        {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }

    }

    pthread_exit(NULL);
    return 0;
}