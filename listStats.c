/*Write a multithreaded program that calculates various statistical values
for a list of numbers. This program will be passed a series of numbers
on the command line and will then create three separate worker threads.
One thread will determine the average of the numbers, the second will
determine the maximum value, and the third will determine the minimum value.
For example, suppose your program is passed the integers
90 81 78 95 79 72 85
The program will report
The average value is 82
The minimum value is 72
The maximum value is 95
The variables representing the average, minimum, and maximum values
will be stored globally. The worker threads will set these values, and
the parent thread will output the values once the workers have exited.
(We could obviously expand this program by creating additional threads
that determine other statistical values, such as median and standard
deviation.)*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *avg_fun(void *ptr);
void *max_fun (void *ptr);
void *min_fun (void *ptr);

double avg;
int max;
int min;

typedef struct 
{
    int size;
    int *values;
}datastruct;

int main(int argc, char *argv[]){
    int copy[argc-1];
    for(int i = 0; i< argc-1; i++){
        copy[i] = atoi(argv[i+1]);
    }

    pthread_t thread1, thread2, thread3;
    int t1,t2,t3;

    datastruct ds = {argc-1 , copy};

    t1 = pthread_create(&thread1, NULL, (void *)avg_fun, (void *) &ds);
    if(t1){
        fprintf(stderr, "Error - pthread_create() return code: %d \n",t1);
        exit(EXIT_FAILURE);
    }
    t2 = pthread_create(&thread2 ,NULL, (void *)max_fun, (void *) &ds);
        if(t2){
        fprintf(stderr, "Error - pthread_create() return code: %d \n",t1);
        exit(EXIT_FAILURE);
    }
    t3 = pthread_create(&thread3, NULL, (void *)min_fun, (void *) &ds);
        if(t3){
        fprintf(stderr, "Error - pthread_create() return code: %d \n",t1);
        exit(EXIT_FAILURE);
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    printf("The Average: %.2f\n",avg);
    printf("The Maximum: %d\n",max);
    printf("The Minimum: %d\n",min);

    exit(EXIT_SUCCESS);

}

void *avg_fun(void *ptr){
    datastruct *ds;
    ds = (datastruct *) ptr;

    for(int i = 0; i< ds->size; i++){
        avg += (ds -> values[i]);
    }
    avg = (double) (avg/ds->size);
    return NULL;
}

void *max_fun(void *ptr){
    datastruct *ds;
    ds = (datastruct *) ptr;
    max = ds->values[0];
    for(int i= 0; i< ds->size; i++){
        if(ds ->values[i]> max){
            max = ds->values[i];
        }
    }
    return NULL;
}
void *min_fun(void *ptr){
    datastruct *ds;
    ds = (datastruct *) ptr;
    min = ds->values[0];
    for(int i= 1 ; i < ds->size; i++){
        if(ds ->values[i] < min){
            min = ds->values[i];
        }
    }
    return NULL;
}
