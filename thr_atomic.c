#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>

int m, n;           // number of threads and total numbers to process
double total = 0.0; // shared global sum

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t sem;
int finished_threads = 0;

typedef struct
{
    int tid;   // thread id
    int start; // starting number
    int end;   // ending number
} thread_arg_t;

void *calc(void *arg)
{
    thread_arg_t *targ = (thread_arg_t *)arg;
    int tid = targ->tid;
    int start = targ->start;
    int end = targ->end;
    double partial = 0.0;

    // Compute partial sum of square roots from start to end
    for (int i = start; i <= end; i++)
    {
        partial += sqrt(i);
    }

    // print this thread partial sum
    printf("thr %d: %.6f\n", tid, partial);

    //lock 
    pthread_mutex_lock(&mutex);
    total += partial;
    finished_threads++;
    pthread_mutex_unlock(&mutex);

    //signal the main thread that this thread finished
    sem_post(&sem);

    return NULL;
}

int main(int argc, char *argv[])
{
    // Get command line arguments
    m = atoi(argv[1]);
    n = atoi(argv[2]);

    // Get the different chunks for the threads
    int chunk = n / m;

    // initialize the sem to 0
    sem_init(&sem, 0, 0);

    // Allocate arrays
    pthread_t threads[m];
    thread_arg_t args[m];
    
    // create m threads
    for (int i = 0; i < m; i++)
    {
        args[i].tid = i;
        args[i].start = i * chunk + 1;
        args[i].end = (i + 1) * chunk;

        if (pthread_create(&threads[i], NULL, calc, &args[i]) != 0)
        {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // wait for all threads to complete
    for (int i = 0; i < m; i++)
    {
        sem_wait(&sem);
    }

    // print final sum 
    printf("sum of square roots: %.6f\n", total);

    //destroy semaphore
    sem_destroy(&sem);

    return 0;
}