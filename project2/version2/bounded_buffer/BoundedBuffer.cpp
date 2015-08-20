/*
  ECE 408 Project 2
  Name: Nhan Do, Peng Bin
  Problem: Bounded Buffer
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <queue>

// un-comment <unistd.h> to compile in linux
// comment out 3 lines after <unistd.h> compile in linux
#include <unistd.h>
//#include <Windows.h>
#define sleep(n) sleep(n*1000)
#define usleep(n) sleep(n/1000)

// some constants
#define BUF_SIZE 5
#define NUM_PRODUCER 2
#define NUM_CONSUMER 2
#define ITER 10 //number of iterations

// the structure to synchronize
struct Buffer{
	int data[BUF_SIZE]; // the shared buffer
	int in;				// denote the first empty slot
	int out;			// denote the first full slot
};

// the shared buffer between producer and consumer
static Buffer buffer;

sem_t notFull;		// how many filled slots
sem_t notEmpty;		// how many empty slots
sem_t lock;		// the locking mechanism

void printSemaphores(){
	int v1, v2, v3;
	sem_getvalue(&notFull, &v1);
	sem_getvalue(&notEmpty, &v2);
	sem_getvalue(&lock, &v3);
	printf("Semaphores: notFull: %d, notEmpty: %d, lock: %d\n\n", v1, v2, v3);
}


// the producer
void* produce(void *param){
	long tid = (long)param;	// thread id


	for (int i = 0; i < ITER; i++){
		int data_in = (tid+1)*1000 + i;	// temp variable to write to buffer

		// wait until there are available spot
		sem_wait(&notFull);

		// wait until the buffer is unlocked
		sem_wait(&lock);

		// write to buffer and increment size
		buffer.data[buffer.in] = data_in;
		buffer.in = (buffer.in + 1) % BUF_SIZE;
		printf("ThreadID: P%ld, in=%d, out=%d, produced %d... \n", tid, buffer.in, buffer.out, data_in);
		printSemaphores();

		// unlock the buffer
		sem_post(&lock);

		// increment the full slot number
		sem_post(&notEmpty);

		// sleep and wait for the consumer
		if (i % 2 == 1) usleep(100);
	}

	return NULL;
}

void* consume(void* param){
	long tid = (long)param; // thread id

	for (int i = 0; i < ITER; i++){

		// wait until the buffer has data
		sem_wait(&notEmpty);

		// wait until the buffer is unlocked
		sem_wait(&lock);

		// get the data from the buffer
		int data_out = buffer.data[buffer.out];

		// if the data is valid
		if (buffer.out != buffer.in){
			printf("ThreadID: C%ld, in=%d, out=%d, consumed %d \n", tid, buffer.in, buffer.out, data_out);
			printSemaphores();
			buffer.out = (buffer.out + 1) % BUF_SIZE;

			// unlock the buffer
			sem_post(&lock);

			// signal that the buffer is not full yet
			sem_post(&notFull);
		}
		else{ // if the buffer is empty
			printf("ThreadID: C%ld, in=%d, out=%d, consumed Empty \n", tid, buffer.in, buffer.out);
			printSemaphores();
			// unlock the buffer
			sem_post(&lock);
		}

	}

	return NULL;
}


int main(int argc, char * argv[])
{
	pthread_t producers[NUM_PRODUCER];
	pthread_t consumers[NUM_CONSUMER];

	// initilize semaphores
	sem_init(&notFull, 0, BUF_SIZE);	// all data slots are free
	sem_init(&notEmpty, 0, 0);			// is empty at first
	sem_init(&lock, 0, 1);				// unlock

	buffer.in = 0;
	buffer.out = 0;
	printSemaphores();
	sleep(1);

	// create the producers
	for (int i = 0; i < NUM_PRODUCER; i++){
		if (pthread_create(&producers[i], NULL, &produce, (void*)i)){
			printf("ERROR creating producer threads\n");
			exit(1);
		}
	}

	// create the consumers
	for (int i = 0; i < NUM_CONSUMER; i++){
		if (pthread_create(&consumers[i], NULL, &consume, (void*)i)){
			printf("ERROR creating consumer threads\n");
			exit(1);
		}
	}
	
	// join all producer threads
	for (int i = 0; i < NUM_PRODUCER; i++){
		if (pthread_join(producers[i], NULL)){
			printf("ERROR joining producer thread\n");
			exit(1);
		}
	}

	// join all consumer threads
	for (int i = 0; i < NUM_CONSUMER; i++){
		if (pthread_join(consumers[i], NULL)){
			printf("ERROR joining producer thread\n");
			exit(1);
		}
	}

	// release all semaphores
	sem_destroy(&notFull);
	sem_destroy(&notEmpty);
	sem_destroy(&lock);

	return 0;
}

