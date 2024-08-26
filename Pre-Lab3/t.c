#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 4

int A[N][N], sum[N];
int total = 0;
pthread_mutex_t lock;
void *func(void *arg) // threads function
{
    int j, row;
    pthread_t tid = pthread_self(); // get thread ID number
    row = (int)arg; // get row number from arg
    printf("Thread %d [%lu] computes sum of row %d\n", row, tid, row);
    for (j=0; j<N; j++)
    {// compute sum of A[row]in global sum[row]
        sum[row] += A[row][j];
    }
    // Lock the mutex before updating the global variable
    pthread_mutex_lock(&lock);
    total += sum[row];       // update total with sum[row]
    pthread_mutex_unlock(&lock); // unlock the mutex

    printf("Thread %d [%lu] done: sum[%d] = %d\n",
        row, tid, row, sum[row]);
    pthread_exit((void*)0); // thread exit: 0=normal termination

    int tmp = total;   // tmp = current total
    tmp += sum[row];        // update tmp locally 

    sleep(1);          // simulate interrupts, which may switch process/threads 

    total = tmp;       // write tmp to total

    printf("Thread %d [%lu] done: sum[%d] = %d\n",
        row, tid, row, sum[row]);
    pthread_exit((void*)0); // thread exit: 0=normal termination
}

int main (int argc, char *argv[])
{
    pthread_t thread[N]; // thread IDs
    int i, j, r;
    void *status;
    printf("Main: initialize A matrix\n");
    for (i=0; i<N; i++)
    {
        sum[i] = 0;
        for (j=0; j<N; j++)
        {
            A[i][j] = i*N + j + 1;
            printf("%4d ", A[i][j]);
        }
        printf("\n");
    }
    printf("Main: create %d threads\n", N);
    for(i=0; i<N;i++)
    {
        pthread_create(&thread[i], NULL, func, (void *)i);
    }
    //printf("Main: try to join with threads\n");
    for(i=0; i<N; i++)
    {
        pthread_join(thread[i], &status);
        printf("Main: joined with %d [%lu]: status=%d\n",
                                i, thread[i], (int)status);
    }
    printf("Main: try to join with threads\n");
    printf("Main: compute and print total sum: ");
    printf("total = %d\n", total);
    pthread_exit(NULL);
}

//Record the total output value: 2080