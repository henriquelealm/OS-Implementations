/*Write a multi-threaded C program that gives readers priority over writers concerning a shared (global) variable.
Essentially, if any readers are waiting, then they have priority over writer threads -- writers can only write when there are no readers. 
This program should adhere to the following constraints:

Multiple readers/writers must be supported (5 of each is fine)
Readers must read the shared variable X number of times
Writers must write the shared variable X number of times
Readers must print:
The value read
The number of readers present when value is read
Writers must print:
The written value
The number of readers present were when value is written (should be 0)
Before a reader/writer attempts to access the shared variable it should wait some random amount of time
Note: This will help ensure that reads and writes do not occur all at once
Use pthreads, mutexes, and condition variables to synchronize access to the shared variable*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_READERS 5
#define NUM_WRITERS 5
#define NUM_READS_WRITES 10

int shared_variable = 0;
int num_readers = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t can_read = PTHREAD_COND_INITIALIZER;
pthread_cond_t can_write = PTHREAD_COND_INITIALIZER;

void *reader(void *arg) {
  int id = *(int*) arg;
  int i;
  for (i = 0; i < NUM_READS_WRITES; i++) {
    // Wait for some random time
    usleep(rand() % 1000);
    
    // Acquire the lock
    pthread_mutex_lock(&mutex);
    while (num_readers == -1) {
      // If a writer is writing, wait to read
      pthread_cond_wait(&can_read, &mutex);
    }
    num_readers++;
    
    // Release the lock
    pthread_mutex_unlock(&mutex);
    
    // Read the shared variable
    printf("Reader %d: read %d (num_readers=%d)\n", id, shared_variable, num_readers);
    
    // Acquire the lock
    pthread_mutex_lock(&mutex);
    num_readers--;
    if (num_readers == 0) {
      // If this is the last reader, signal any waiting writers
      pthread_cond_signal(&can_write);
    }
    
    // Release the lock
    pthread_mutex_unlock(&mutex);
  }
  
  return NULL;
}

void *writer(void *arg) {
  int id = *(int*) arg;
  int i;
  for (i = 0; i < NUM_READS_WRITES; i++) {
    // Wait for some random time
    usleep(rand() % 1000);
    
    // Acquire the lock
    pthread_mutex_lock(&mutex);
    while (num_readers != 0) {
      // If there are readers, wait to write
      pthread_cond_wait(&can_write, &mutex);
    }
    num_readers = -1;
    
    // Release the lock
    pthread_mutex_unlock(&mutex);
    
    // Write to the shared variable
    shared_variable = id * 100 + i;
    printf("Writer %d: wrote %d (num_readers=%d)\n", id, shared_variable, num_readers);
    
    // Acquire the lock
    pthread_mutex_lock(&mutex);
    num_readers = 0;
    // Signal all waiting readers
    pthread_cond_broadcast(&can_read);
    
    // Release the lock
    pthread_mutex_unlock(&mutex);
  }
  
  return NULL;
}

int main() {
  int i;
  pthread_t readers[NUM_READERS];
  pthread_t writers[NUM_WRITERS];
  int reader_ids[NUM_READERS];
  int writer_ids[NUM_WRITERS];
  
  // Create reader threads
  for (i = 0; i < NUM_READERS; i++) {
    reader_ids[i] = i;
    pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
  }
  
  // Create writer threads
  for (i = 0; i < NUM_WRITERS; i++) {
    writer_ids[i] = i;
    pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
  }
  
  // Wait for all threads to finish
  for (i = 0; i < NUM_READERS; i++) {
    pthread_join(readers[i], NULL);
    }
    for (i = 0; i < NUM_WRITERS; i++) {
    pthread_join(writers[i], NULL);
    }

return 0;
}
