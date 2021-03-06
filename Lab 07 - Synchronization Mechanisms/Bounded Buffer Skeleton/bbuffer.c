#include <pthread.h>
#include <stdio.h>

// shared buffer, adjust this size and notice the interleaving
#define BSIZE 3
typedef struct {
	char buf[BSIZE];
	int occupied;
	int nextin, nextout;
	pthread_mutex_t mutex;
	pthread_cond_t more;
	pthread_cond_t less;
} buffer_t;
buffer_t buffer;

// running flag for thread termination
int done = 0;
int sleep_time;

// threads, and pointers to associated functions
void* producerFunction(void *);
void* consumerFunction(void *);
pthread_t producerThread;
pthread_t consumerThread;

//
// program entry point
//
int main( int argc, char *argv[] ){

	// sanity check
	if( argc != 2 ){
		printf("useage: bound sleeptime \n");
		return(0);
	}

	// get parameters
	sleep_time = atoi(argv[1]);

	pthread_cond_init(&(buffer.more), NULL);
	pthread_cond_init(&(buffer.less), NULL);

	pthread_create(&consumerThread, NULL, consumerFunction, NULL);
	pthread_create(&producerThread, NULL, producerFunction, NULL);

	pthread_join(consumerThread, NULL);
	pthread_join(producerThread, NULL);

	printf("main() exiting properly, both threads have terminated. \n");
	return(1);
}


void* producerFunction(void * parm){

	printf("producer starting... \n");

	// objects to produce, place in buffer for the consumer
	char item[]= "More than meets the eye!";

	int i;
	for( i=0 ;; i++){

		// done producing when end of null terminated string
		if( item[i] == '\0') break;

		// acquire lock
		if( ... ) printf("producer has the lock. \n");

		// debug info
		if( ... ) printf("producer waiting, full buffer ... \n");

		// wait condition
		while( ... )
			pthread_cond_wait(&(buffer.less), &(buffer.mutex) );

		// add to the buffer
		...

		// debug info
		printf("producing object number: %i [%c]\n", i, item[i]);

		// signal the producer, release the lock
		....

		// tell consumer we are no longer producing more items
		// by setting the done flag if this is the last element
		if( ... )
			done = 1;

		// impose a delay to show mutual exclusion
		sleep(sleep_time);
	}

	printf("producer exiting. \n");
	pthread_exit(0);
}


void* consumerFunction(void * parm){

	printf("consumer starting \n");

	char item;
	int i;
	for( i=0 ;; i++ ){

		// is the producer still running?
		if( ... ) break;

		// acquire lock
		if( pthread_mutex_lock(&(buffer.mutex)) == 0 )
			printf("consumer has the lock. \n");

		// debug info
		if ( ... ) printf("consumer waiting, empty buffer ... \n");

		// wait condition
		while(...)
			pthread_cond_wait(...);

		// consume from buffer by displaying to the terminal
		item = buffer.buf[buffer.nextout++];
		printf("consuming object number %i [%c]\n", i ,item);

		// now there is room in the buffer for the producer to add
		...

		// signal the producer, and release the lock
		...
	}

	printf("consumer exiting. \n");
	pthread_exit(0);
}