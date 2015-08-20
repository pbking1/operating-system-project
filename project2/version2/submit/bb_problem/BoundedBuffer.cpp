/*
  ECE 408 Project 2
  Name: Nhan Do, Peng Bin
  Problem: Bounded Buffer
*/

#include <pthread.h>
#include <cstdio>
#include <cstdlib>
#include <semaphore.h>
#include <ctime>
#include <vector>
#include <queue>

// un-comment <unistd.h> to compile in linux
// comment out 3 lines after <unistd.h> compile in linux
#include <unistd.h>
//#include <Windows.h>
//#define sleep(n) Sleep(n*1000)
//#define usleep(n) Sleep(n/1000)

// some constants
#define BUF_SIZE 5

// the structure to synchronize
static std::queue<int> buffer;
sem_t notFull;		// how many filled slots
sem_t notEmpty;		// how many empty slots
sem_t lock;		// the locking mechanism

std::queue<int> list_of_data_in; // the producer will grab data from here
std::vector<int> list_of_data_out; // the consumer will grab data from here

bool flag_all_data_produced; // the flag is true if all data is produced, all producers terminate
bool flag_all_data_consumed; // the flag is true if all data is consumed, all consumers terminate

time_t rawtime; // to store the time
FILE *fout; // the output 

void printSemaphores(){
	return; // disable printing this
	int v1, v2, v3;
	sem_getvalue(&notFull, &v1);
	sem_getvalue(&notEmpty, &v2);
	sem_getvalue(&lock, &v3);
	printf("Semaphores: notFull: %d, notEmpty: %d, lock: %d\n\n", v1, v2, v3);
}

// the producer
void* produce(void *param){
	long tid = (long)param;	// thread id

	while (!flag_all_data_produced){
		
		// wait until there are available spot
		sem_wait(&notFull);

		// wait until the buffer is unlocked
		sem_wait(&lock);

		if (list_of_data_in.empty()){
			flag_all_data_produced = true;
			sem_post(&lock);
			sem_post(&notEmpty);
			break;
		}
	
		// if there are still space, write to buffer and increment size in 50% chance
		if ( buffer.size()<BUF_SIZE){
			buffer.push(list_of_data_in.front());		
			fprintf(fout,"ThreadID: P%ld, produced %d, buffer size is now %lu \n", tid, list_of_data_in.front(), buffer.size());
			list_of_data_in.pop();
			printSemaphores();

			// unlock the buffer
			sem_post(&lock);

			// increment the full slot number
			sem_post(&notEmpty);
		}

		else{
			// unlock the buffer
			sem_post(&lock);
		}

		// sleep
		usleep(300);
	}
	fprintf(fout, "ThreadID: P%ld exits! \n", tid);
	return NULL;
}

void* consume(void* param){
	long tid = (long)param; // thread id

	while (!flag_all_data_consumed){

		// wait until the buffer has data
		sem_wait(&notEmpty);

		// wait until the buffer is unlocked
		sem_wait(&lock);

		// if the data is valid
		if (buffer.size() > 0){
			int data_out = buffer.front();
			list_of_data_out.push_back(data_out);
			buffer.pop();
			fprintf(fout, "ThreadID: C%ld, consumed %d, buffer size is now %lu \n", tid, data_out, buffer.size());

			printSemaphores();
			
			// unlock the buffer
			sem_post(&lock);

			// signal that the buffer is not full yet
			sem_post(&notFull);


		}
		else{ // if the buffer is empty
			fprintf(fout, "ThreadID: C%ld, wait because buffer is empty \n", tid);
			if (flag_all_data_produced){ // everything is consumed
				flag_all_data_consumed = true;
				sem_post(&lock);
				sem_post(&notEmpty);
				break;
			}
			printSemaphores();
			// unlock the buffer
			sem_post(&lock);
		}
		usleep(600);
		
	}
	fprintf(fout, "ThreadID: C%ld exits! \n", tid);
	return NULL;
}


int main(int argc, char * argv[])
{
	int NUM_PRODUCER = 2;
	int NUM_CONSUMER = 2;
	int NUM_TOTAL_DATA = 20;
	FILE* fin;
	
	if ((fin = fopen("input.1", "r"))==NULL){
		printf("cannot read input!\n");
		exit(1);
	}

	fscanf(fin, "%d %d %d", &NUM_PRODUCER, &NUM_CONSUMER, &NUM_TOTAL_DATA);
	printf("Input success!\n");
	printf("Number of producers: %d\n", NUM_PRODUCER);
	printf("Number of consumers: %d\n", NUM_CONSUMER);
	printf("Number of total data: %d\n", NUM_TOTAL_DATA);

	fout = fopen("processLog_BoundedBuffer.log", "w");
	srand((unsigned)time(NULL));


	std::vector<pthread_t> producers(NUM_PRODUCER);
	std::vector<pthread_t> consumers(NUM_CONSUMER);

	// initilize semaphores
	sem_init(&notFull, 0, BUF_SIZE);	// all data slots are free
	sem_init(&notEmpty, 0, 0);			// is empty at first
	sem_init(&lock, 0, 1);				// unlock

	// initialize other global variables
	flag_all_data_produced = false;
	flag_all_data_consumed = false;
	for (int i = 0; i < NUM_TOTAL_DATA; i++){
		list_of_data_in.push(i);
	}

	printSemaphores();
	sleep(1);

	// create the producers
	for (int i = 0; i < NUM_PRODUCER; i++){
		if (pthread_create(&producers[i], NULL, &produce, (void*)i)){
			printf("ERROR creating producer threads\n");
			exit(1);
		}
	}

	usleep(1200);
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

	fprintf(fout, "All producers and consumers finished!\n");
	// release all semaphores
	sem_destroy(&notFull);
	sem_destroy(&notEmpty);
	sem_destroy(&lock);
	printf("Simulation complete!\n");
	return 0;
}

