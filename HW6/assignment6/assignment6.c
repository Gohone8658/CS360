#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/shm.h>


//globals
pthread_mutex_t chopsticks[5];											//globals to share between threads.
pthread_mutex_t readyCheck[5];

const int FINISHEDEATING = 100;

void philoActions(int *tNum);
int randomGaussian(int mean, int stddev);

int main(int argc, char *argv[]){
	/* code */
	pthread_t thread[5];
	int i;
	int tNum[5];

	for(i=0; i<5; i++){//init mutexes
		int mutex1Check = pthread_mutex_init(&chopsticks[i], NULL);		//create thread mutexes for chopstick usage.
		int mutex2Check = pthread_mutex_init(&readyCheck[i], NULL); 	//create mutex to pause child threads until all are ready.
		int firstLockCheck = pthread_mutex_lock(&readyCheck[i]); 		//locks the ready check so that child processes will wait for all to be created.

		if(mutex1Check != 0 || mutex2Check != 0 || firstLockCheck != 0){//error check mutex inits and first lock of readyCheck
			printf("%s\n", strerror(errno));
			return 1;
		}
	}	

	for(i=0; i<5; i++){//create 5 threads all executing philoActions with a variable to indicate what thread number (philosopher) it is.
		tNum[i]=i;
		int threadCheck = pthread_create(&thread[i], NULL, (void *)philoActions, (int *) &tNum[i]);
		if(threadCheck != 0){//error returned.
			printf("%s\n", strerror(errno));
			return 1;
		}
		printf("philosopher %d has joined the table.\n", i);
	}
	
	for(i=0; i<5; i++){//unlock mutexes so child processes begin.
		pthread_mutex_unlock(&readyCheck[i]);
	}

	for(i=0; i<5; i++){//make the calling thread wait for each thread to terminate.
		pthread_join(thread[i], NULL);
	}
	
	printf("All philosophers have finished eating, exiting.\n");

	for(i=0; i<5; i++){//destroy mutexes before exiting. All are unlocked at the end of child processes.
		int destroyCheck1 = pthread_mutex_destroy(&chopsticks[i]);
		int destroyCheck2 = pthread_mutex_destroy(&readyCheck[i]);

		if(destroyCheck1 != 0 || destroyCheck2 != 0){ //error check mutex destroys
			printf("%s\n", strerror(errno));
			return 1;
		}
	}

	return 0;
}

//This function is called by each child thread to simulate the actions of one philosopher.
//The function sets adjacent chopstick parameters and creates an eat/think cycle until specified eating time is reached.
//mutexes are used to indicate chopstick use. philosophers will wait for chopsticks to be unlocked before continuing their cycles.
void philoActions(int *tNum){
	int firstChopstick;
	int secondChopstick;
	int eatingTime = 0;
	time_t t;
	srand((unsigned) time(&t)+*tNum);

	//set each philosophers adjacent chopsticks. Else statements prevent deadlock at last philosopher.
	if(*tNum < 4){
		firstChopstick = *tNum;
	} else{
		firstChopstick = 0;
	}
	if(*tNum < 4){
		secondChopstick = *tNum+1;
	}else{
		secondChopstick = 4;
	}

	pthread_mutex_lock(&readyCheck[*tNum]);			//variables have been initialized, pause until all threads are ready (readyCheck is already locked in main).

	while(eatingTime < FINISHEDEATING){//loop for eat/sleep cycles
		
		int eatingCycle = randomGaussian(9, 3);
		if(eatingCycle < 0){ eatingCycle = 0;}//check for negative
		int thinkingCycle = randomGaussian(11, 7);
		if(thinkingCycle < 0){thinkingCycle = 0;}//check for negative

		printf("Philosopher %d is looking for chopsticks\n", *tNum);

		//lock adjacent chopsticks from other threads
		pthread_mutex_lock(&chopsticks[firstChopstick]);
		pthread_mutex_lock(&chopsticks[secondChopstick]);

		//eating cycle begins
		eatingTime += eatingCycle;
		printf("Philosopher %d is eating for %d second (total = %d)\n", *tNum, eatingCycle, eatingTime);
		sleep(eatingCycle);
		//eating cycle ends

		//unlock adjacent chopsticks for other threads to use
		pthread_mutex_unlock(&chopsticks[firstChopstick]);
		pthread_mutex_unlock(&chopsticks[secondChopstick]);

		if(eatingTime < FINISHEDEATING){//then perform thinking cycle
			printf("Philosopher %d is thinking for %d seconds\n", *tNum, thinkingCycle);
			sleep(thinkingCycle);
		}
	}
	printf("Philosopher %d has finished eating\n", *tNum);

	pthread_mutex_unlock(&readyCheck[*tNum]);
}

//random number generator for eat/think cycle times. Seeded in philoActions()
int randomGaussian(int mean, int stddev) {
	double mu = 0.5 + (double) mean;
	double sigma = fabs((double) stddev);
	double f1 = sqrt(-2.0 * log((double) rand() / (double) RAND_MAX));
	double f2 = 2.0 * 3.14159265359 * (double) rand() / (double) RAND_MAX;
	if (rand() & (1 << 5)) 
		return (int) floor(mu + sigma * cos(f2) * f1);
	else            
		return (int) floor(mu + sigma * sin(f2) * f1);
}