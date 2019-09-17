#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <semaphore.h>
#include <time.h>


int client_int=0;
int server_int =0;
int client_num = 2, server_num =2;

int jobs= 0;
int queue = 0;
int tick;

int serverStatus = 1; //server is free


float mu= 0.01, lambdas = 0.005;

pthread_t servers_tid[2];
pthread_t clients_tid[2];
pthread_t clock_tid;

sem_t clientmutex;
sem_t servermutex;
sem_t queuemutex;
sem_t clockmutex;
sem_t randMutex;





double randomNumber()
{
	
	double p = ((double)rand()/(double)(RAND_MAX));
	return p;
	
}

void enque(int t)
{
	t++;
	

}

void *client(void *lambda)
{
	
	


	pthread_t clientid = pthread_self();
	int tick=0;

	
	
	double client;
	while (tick<10)
	{
		sem_wait(&clockmutex);
		
		float randomclient = randomNumber() / 100; 
		 
		
		if (randomclient < lambdas)
			{
				sem_wait(&queuemutex);//lock the mutex
				queue++;
							
				sem_post(&queuemutex);
		
			}
				
		sem_post(&clientmutex);
		tick++;

	printf("in client ");
		
	
	}



}

void *servers(void *mu)
{
	pthread_t id = pthread_self();


	
}



void* clockT(void *arg)
{
	tick =0;
	int p;
	printf("in clock");
	while(tick<10)
	{
	
	//for(p= 0; p<2; p++)
		sem_wait(&clientmutex);
		//sem_wait(&clientmutex);
	
		tick++;
		
		printf("tick = %d", tick);
		printf("server is");
		printf("number of job generated %d", jobs);
		
		
	
	}
}

int main (int argc, char **argv)

{
	
	

	srand(time(NULL));


//CLOCK
	int clock;
	clock = pthread_create(&clock_tid, NULL, &clockT, NULL);
			
		if(clock!=0)
		{
			printf("Clock failed");
		}


//CLIENTS
	int pp = 0;	


			int client_a= pthread_create(&clients_tid[0],NULL,&client,NULL);
			if (client_a != 0)
			{
				printf("Client failed");
			}
			else
			{
				printf("created");
			}


	


//SERVER

			int server_t= pthread_create(&servers_tid[0],NULL,&servers,NULL);
			if (server_t!=0)
			{
				printf("Server failed");
			}
			else
			{
				printf("SERVER created");
			}



//JOINS
int i;

{
    pthread_join(clients_tid[0], NULL);
}

int p;
	//for(p = 0; p < 2; p++)
	{ 
	
		pthread_join(servers_tid[0], NULL);  
	}


pthread_join(clock_tid, NULL);



	

//ERROR CHECKING

	//printf("main works"); WORKS
	//randomNumber();	WORKS


}



