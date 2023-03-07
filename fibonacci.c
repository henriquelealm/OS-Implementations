/*
The Fibonacci sequence is the series of numbers 0, 1, 1, 2, 3, 5, 8, ....
Formally, it can be expressed as: fib0 = 0 fib1 = 1 fibn = fibn−1 + fibn−2 Write
a multithreaded program that generates the Fibonacci sequence. This program
should work as follows: On the command line, the user will enter the number of
Fibonacci numbers that the program is to generate. The program will then create
a separate thread that will generate the Fibonacci numbers, placing the sequence
in data that can be shared by the threads (an array is probably the most
convenient data structure). When the thread finishes execution, the parent
thread will output the sequence generated by the child thread. Because the
parent thread cannot begin outputting the Fibonacci sequence until the child
thread finishes, the parent thread will have to wait for the child thread to
finish.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 500

void *generateFibonacci(void *ptr);
void *printFibonacci(void *ptr);

int output[MAX_SIZE];

int main(int argc, char *argv[]) {

  int size = atoi(argv[1]);
  pthread_t thread1, thread2;
  int t1, t2;

  t1 = pthread_create(&thread1, NULL, generateFibonacci, &size);
  if (t1) {
    fprintf(stderr, "Error - pthread_create() return code: %d \n", t1);
    exit(EXIT_FAILURE);
  }

  pthread_join(thread1, NULL);

  t2 = pthread_create(&thread2, NULL, printFibonacci, &size);
  if (t2) {
    fprintf(stderr, "Error - pthread_create() return code: %d \n", t2);
    exit(EXIT_FAILURE);
  }

  pthread_join(thread2, NULL);
  return 0;
}

void *generateFibonacci(void *ptr) {
  int *size, t1 = 0, t2 = 1;
  int nextTerm = t1 + t2;

  size = (int *)ptr;
  output[0] = t1;
  output[1] = t2;

  for (int i = 2; i < *size; ++i) {
    output[i] = nextTerm;
    t1 = t2;
    t2 = nextTerm;
    nextTerm = t1 + t2;
  }

  return NULL;
}

void *printFibonacci(void *ptr) {
  int *size;
  size = (int *)ptr;
  printf("Fibonacci Series: ");

  for (int i = 0; i < *size; i++) {
    printf("%d ", output[i]);
  }
  printf("\n");

  return NULL;
}
