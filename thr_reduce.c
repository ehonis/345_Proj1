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

void *threadCalc(void *arg)
{
    // put arguments into structure
    thread_arg_t *targ = (thread_arg_t *)arg;
    int tid = targ->tid;
    int start = targ->start;
    int end = targ->end;
    // partial sum for the individual thread
    double partial = 0.0;

    // calculate the thread's partial sum
    for (int i = start; i <= end; i++)
    {
        partial += sqrt(i);
    }
    // put partial into partial sums array
    partial_sums[tid] = partial;

    int r = 0;
    // find when 2^r is >= m, determining reduction rounds
    // 1 << r shifts the 1, r places, effectively calculating 2^r
    // when 2^r passes or is equal to m, this will effectively calculating r = log(m)
    while ((1 << r) < m)
        r++;

    // reduce and combine with other threads r amount of times
    for (int x = 1; x <= r; x++)
    {
        // calculate partner with bitshift operations equiv to 2^(r-x)
        // this calculation was given by the instructions
        int partner = tid ^ (1 << (r - x));

        if (tid < partner)
        {

            // if active thread, meaning the thread id is less than its partner's id, combine threads and partial sum
            // by calling pthread_join, it waits for its partner's thread to finish
            pthread_join(threads[partner], NULL);
            // add partners partial sum to the current threads partial thread.
            partial_sums[tid] += partial_sums[partner];
        }
        else
        {
            // non active thread
            return NULL;
        }
    }
    // if the thread id is the last one (0), print the array and final sum
    if (tid == 0)
    {
        int length = m;
        printf("============= Partial Sums Array =============\n");
        for (int g = length - 1; g >= 0; g--)
        {
            printf("Thread %d's Partial Sum: %.6f\n", g, partial_sums[g]);
        }
        printf("=============== Final Sum ====================\n");
        printf("%.6f\n", partial_sums[0]);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    // get arguments from command line
    m = atoi(argv[1]);
    n = atoi(argv[2]);

    // find chunk for each process
    int chunk = n / m;
    // allocate memory for each array
    threads = malloc(m * sizeof(pthread_t));
    partial_sums = malloc(m * sizeof(double));

    // create structures for amount of threads needed
    thread_arg_t args[m];

    // loop through, designate start and end of m thread and create the thread
    for (int i = m - 1; i >= 0; i--)
    {

        args[i].tid = i;
        args[i].start = i * chunk + 1;
        args[i].end = (i + 1) * chunk;

        if (pthread_create(&threads[i], NULL, threadCalc, &args[i]) != 0)
        {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // after creating the thread, kill main thread.

    pthread_exit(NULL);
    return 0;
}