// Dining philosophers using System V Semaphores
// Original: http://www.lisha.ufsc.br/~guto/teaching/os/exercise/phil.html
// Modified by Chris Kauffman
//
// Short random delays are added between each philosophers
// thinking/eating cycle to generate some variance in execution order
//
// To see the multiple processes, run the following commands
// > gcc -o philosophers philosophers.c
// > philosophers > xxx & watch -d -n 0.1 'ps ux | grep philosophers'
// Ctrl-c to stop the "watch" command (may screw up the terminal display)

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <math.h>

const int N_PHILOSOPHERS  =  5; // Number of philosophers
//const int MEALS_TO_HEAVEN = 10; // Number of iterations before philosophers finish
const int FINISHED_EATING = 100;
const int MAX_DELAY = 500000;   // Maximum delay between meal iterations

// Semaphore ids
int chopsticks;                 // ID for array of IPC semaphores
int not_at_table;               // Start dinner when semaphore reaches 0

int philosopher(int n);         // Defined after main()
int randomGaussian(int mean, int stddev);

int main(){
  int i, status;
  pid_t phil[N_PHILOSOPHERS];
  printf("The Dining-Philosophers Problem\n");

  // Parent process only:
  // 
  // Allocate chopsticks: semaphores which are initially set to value
  // 1. 5 chopsticks total in an array.
  chopsticks = semget(IPC_PRIVATE, N_PHILOSOPHERS, IPC_CREAT | 0600);
  for(i=0; i<N_PHILOSOPHERS; i++){
    semctl(chopsticks, i, SETVAL, 1);
  }
  not_at_table = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);

  // Prevent children from starting to eat
  semctl(not_at_table, 0, SETVAL, 5);

  // Parent generates child processes
  for(i=0; i < N_PHILOSOPHERS; i++){
    int pid = fork();
    if(pid == 0){               // child has pid 0
      int ret = philosopher(i); // child acts as philosopher
      exit(ret);                // then exits
    }
    else{                       // parent gets pid > 0
      phil[i] = pid;            // parent tracks children
    }
  }

  // Parent waits on all children to finish
  for(i = 0; i < N_PHILOSOPHERS; i++) {
    waitpid(phil[i], &status, 0);
  }

  // Eliminate the chopsticks and table semaphores
  semctl(chopsticks, 0, IPC_RMID, 0);
  semctl(not_at_table, 0, IPC_RMID, 0);
  return 0;
}

// Code for dining philosopher child processes 
int philosopher(int n){
  int i, j, first, second;
  struct sembuf op;             // Used to perform semaphore operations
  op.sem_flg = 0;
  srand(n);                     // Seed random number generator
  int eatingTime = 0;

  // Avoid deadlock via slightly different order of chopstick requests
  // for last philospher
  first =  (n < N_PHILOSOPHERS)? n     : 0;                
  second = (n < N_PHILOSOPHERS)? n + 1 : N_PHILOSOPHERS-1; 

  // Check in for dinner
  op.sem_op = -1;
  op.sem_num = 0;
  semop(not_at_table, &op, 1);
  printf("Philosopher %d at the table\n", n);

  // Wait for everyone to check in before start the meal
  op.sem_op = 0;
  op.sem_num = 0;
  semop(not_at_table, &op, 1);

  // Main loop of thinking/eating cycles
  while(eatingTime < FINISHED_EATING) {
    int sleep_time = randomGaussian(9, 3);
    sleep(sleep_time);         // sleep for 0-9 microseconds
    printf("sleep time = %d\n", sleep_time);

    printf("%2d: Philosopher %d is thinking ...\n", i,n);
    /* get first chopstick */
    op.sem_op = -1;
    op.sem_num = first;
    semop(chopsticks, &op, 1);
    /* get second chopstick */
    op.sem_op = -1;
    op.sem_num = second;
    semop(chopsticks, &op, 1);

    printf("%2d: Philosopher %d is eating ...\n", i,n);
    /* release first chopstick */
    op.sem_op = +1;
    op.sem_num = first;
    semop(chopsticks, &op, 1);
    /* release second chopstick */
    op.sem_op = +1;
    op.sem_num = second;
    semop(chopsticks, &op, 1);

    eatingTime += sleep_time;
  }

  printf("Philosopher %d finished eating total time = %d\n",n, eatingTime);
  exit(n);
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

