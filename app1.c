#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <pthread.h>
#include "queue.h"
#include "args.h"
#include "proc.h"

sema_t queue_sema;

double rand_double()
{
  unsigned long rnd;
  
  rnd = random();
  return ((rnd>>7) & 0xffff)/(float) 0xffff;
}

quel_t create_job()
{
  return 1;
}

void kill_job(quel_t job)
{
  return;
}

//CLIENT


void client(struct client_arg *ptr)
{
  pthread_t me;
  queue_t *queue;
  sema_t *clock_sema, *finish_sema, *queue_sema;
  float lambda;
  quel_t job;

  clock_sema = ptr->clock_sema;
  finish_sema = ptr->finish_sema;
  queue_sema = ptr->queue_sema;
  queue = ptr->queue;
  lambda = ptr->lambda;

  while (1)
    {
      sema_wait(clock_sema);
      if (lambda>rand_double())
	{
	  job = create_job();
	  push_q(queue,job);
	  printf("CLIENT: queue length %d\n",size_q(queue));
	}
      sema_post(finish_sema);
    }
}

//SERVER

void server(struct server_arg *ptr)
{
  queue_t *queue;
  sema_t *clock_sema, *finish_sema, *queue_sema;
  float mu;
  quel_t job;

  printf("In server\n");

  clock_sema = ptr->clock_sema;
  finish_sema = ptr->finish_sema;
  queue_sema = ptr->queue_sema;
  queue = ptr->queue;
  mu = ptr->mu;

  while (1)
    {				/* Wait the next clock tick */
      sema_wait(clock_sema);
      if ( job != 0 )
	{
	  if (mu>rand_double())
	    {
	      printf("SERVER1: queue length %d\n",size_q(queue));
	      kill_job(job);
	      job = safepop_q(queue);
	    }
	}
      else
	{
	  job = safepop_q(queue);
	  printf("SERVER2: queue length %d\n",size_q(queue));
	}
				/* Let the clock know we are done */
      sema_post(finish_sema);
    }
}

main(int argc, char **argv)
{
  int i;
  int num_of_servers, num_of_clients, num_of_ticks;
  float mu, lambda, load;

  pthread_t server_tid, client_tid;
  sema_t server_sema, client_sema, finish_sema;
  struct client_arg c_rec;
  struct server_arg s_rec;
  queue_t *queue;

  mu = 0.101;
  lambda = 0.1;
  load = lambda / mu;
  num_of_servers = 1;
  num_of_clients = 1;
  num_of_ticks = 100;
				
  i = 1;
  while (i<argc)
    {
      switch (argv[i][0])
	{
	case 'c':
	  if (strncmp(argv[i],"clients",strlen(argv[i])) == 0)
	    {
	      num_of_clients = atoi(argv[++i]);
	    }
	  break;
	case 't':
	  if (strncmp(argv[i],"ticks",strlen(argv[i])) == 0)
	    {
	      num_of_ticks = atoi(argv[++i]);
	    }
	  break;
	case 's':
	   if (strncmp(argv[i],"servers",strlen(argv[i])) == 0)
	    {
	      num_of_servers = atoi(argv[++i]);
	    }
	  break;
	default:
	  fprintf(stderr,"%s: wrong args\nUsage: %s [clients nn] [servers nn] ...",argv[0], argv[0]);
	  exit(1);
	}
      i++;
    }
				
  sema_init(&server_sema,0,USYNC_THREAD,NULL);
  sema_init(&client_sema,0,USYNC_THREAD,NULL);
  sema_init(&finish_sema,0,USYNC_THREAD,NULL);
  sema_init(&queue_sema,1,USYNC_THREAD,NULL);

				/* Create a queue */
  queue = mk_queue();

				/* Setup the arguments and fork the threads */
  c_rec.clock_sema = &server_sema;
  c_rec.finish_sema = &finish_sema;
  c_rec.queue_sema = &queue_sema;
  c_rec.queue = queue;
  c_rec.lambda = lambda;

  pthread_create(&client_tid, NULL, (void * (*)(void *)) client, &c_rec);
				
  s_rec.clock_sema = &client_sema;
  s_rec.finish_sema = &finish_sema;
  s_rec.queue_sema = &queue_sema;
  s_rec.queue = queue;
  s_rec.mu = mu;
  pthread_create(&server_tid, NULL, (void * (*)(void *)) server, &s_rec);

  for (i=0; i<num_of_ticks; i++)
    {
				
      sema_post(&server_sema);
      sema_post(&client_sema);
      printf("Queue length is %d\n",size_q(queue));
      sema_wait(&finish_sema);
      sema_wait(&finish_sema);
    }

  sema_destroy(&server_sema);
  sema_destroy(&client_sema);
  sema_destroy(&finish_sema);
  sema_destroy(&queue_sema);

  free_queue(queue);
}