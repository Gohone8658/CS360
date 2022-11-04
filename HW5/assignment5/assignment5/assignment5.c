#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <time.h>
#include <semaphore.h>

int chopsticks; //id for semaphore array
int notReady;   //id for ready check
const int FINISHEDEATING = 100;

int randomGaussian(int mean, int stddev);

int main(int argc, char *argv[]){
	int i;
	int status;
	pid_t processID;
	pid_t processIDs[5];

	chopsticks = semget(IPC_PRIVATE, 5, IPC_CREAT | 0600);  //IPC_PRIVATE gets a new shared memory segment
	if(chopsticks == -1){ //semget failed
		printf("%s\n", strerror(errno));
		return 1;
	}

	for(i=0; i<5; i++){
		semctl(chopsticks, i, SETVAL, 1); //set initial vals for philoloop semaphore
	}
	notReady = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600); //IPC_PRIVATE gets a new shared memory segment
	if(notReady == -1){ //semget failed
		printf("%s\n", strerror(errno));
		return 1;
	}

	semctl(notReady, 0, SETVAL, 5); //set initial vals for readycheck

	for(i=0; i < 5; i++){ //create philosopher processes
		processID = fork();
		if(processID==0){//child
			int firstChopstick;
			int secondChopstick;
			int eatingTime = 0;
			time_t t;
			srand((unsigned) time(&t)+i);

			struct sembuf operation; //create buffer for operations on semaphores

			operation.sem_flg = 0;

			//else statements prevent deadlock on 5th philosopher
			if(i < 4){
				firstChopstick = i;
			} else{
				firstChopstick = 0;
			}
			if(i < 4){ 
				secondChopstick = i+1;
			}else{
				secondChopstick = 4;
			}

			//make sure all processes are started and ready
			operation.sem_op = -1;
			operation.sem_num = 0;
			semop(notReady, &operation, 1);

			while(eatingTime < FINISHEDEATING){
				int errcheck;
				int eatingCycle = randomGaussian(9, 3);
				if(eatingCycle < 0){ eatingCycle = 0;} //check for negative
				int thinkingCycle = randomGaussian(11, 7);
				if(thinkingCycle < 0){thinkingCycle = 0;} //check for negative

				//get chopsticks, decrements each chopstick in the semaphore and checks if available.
				printf("Philosopher %d is looking for chopsticks\n", i);
				operation.sem_op = -1;
				operation.sem_num = firstChopstick;
				errcheck = semop(chopsticks, &operation, 1);
				
				operation.sem_op = -1;
				operation.sem_num = secondChopstick;
				semop(chopsticks, &operation, 1);

				eatingTime += eatingCycle;
				printf("Philosopher %d is eating for %d second (total = %d)\n", i, eatingCycle, eatingTime);
				sleep(eatingCycle);

				//release chopsticks, increments chopstick count in semaphore
				operation.sem_op = +1;
				operation.sem_num = firstChopstick;
				semop(chopsticks, &operation, 1);
				operation.sem_op = +1;
				operation.sem_num = secondChopstick;
				semop(chopsticks, &operation, 1);

				if(eatingTime < FINISHEDEATING){
					printf("Philosopher %d is thinking for %d seconds\n", i, thinkingCycle);
					sleep(thinkingCycle);
				}		
			}
			printf("Philosopher %d has finished eating\n", i);
			exit(0);

		} else if(processID>0){
			processIDs[i] = processID;
		} else{ //fork failed
			printf("%s\n", strerror(errno));
			return 1;
		}
	}

	for(i=0; i<5; i++){ //wait for child processes
		waitpid(processIDs[i], &status, 0);
	}

	//cleanup semaphores
	semctl(chopsticks, 0, IPC_RMID, 0);
	semctl(notReady, 0, IPC_RMID, 0);

	return 0;
}


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