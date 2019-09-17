#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_BUF 1024

#define N 100       //default 100 signals


int process1(void);
int process2(void);
void signal_callback(int signo);

void sighandler(int signumber, siginfo_t *info, void *ptr)      //created handler
{
    
    printf("Signal originates from process %lu\n",
           (unsigned long)info->si_pid);
    sender_pid = info->si_pid;
    
    printf("Form child %d to the child %d signal is %d\n", sender_pid, getpid(), counter);
  

}
// variable initializing

char work =1, quit =0;
char inter_snd = 0, inter_rcv = 0;

//sigaction setting
struct sigaction act;
int i, j, k;
int res[0];
static const char *fifoname[] = { "sigperf2", "sigperf2" };
pid_t c1, c2;
pid_t sender_pid;

static
void main_quit(int n)
{
    printf("%s(): signal %d\n", __func__, n);  
    kill(res[0], SIGUSR1);
}


//sigset_t signal_mask;



int main(int argc, char *argv[])
    {
    
    //FIFO accross processes
    int fd;
    char * myfifo = "/tmp/myfifo";
    char buf[MAX_BUF];
    /* create the FIFO (named pipe) */
    mkfifo(myfifo, 0666);

    
    //number of seperate processes to create
    int num_processes = 2;

    
   
        pid_t pid1 = fork();
        if (pid1 < 0)
        {
            perror("fork is failed");
            exit(1);
        }
        else if (pid1 == 0)          //in the child
        {

            execlp("./sigperf1", "sigperf1","0",NULL);
        }
    
        pid_t pid2 = fork();
    
        if (pid2 < 0) {
            perror("fork is failed");
            exit(1);
        }
        else if (pid2 == 0)          //in the child
        {

            execlp("./sigperf1", "sigperf1","0",NULL);
        }
    
    
    printf("Parent doing stuff\n");
    
    

    printf("Parent waiting on child\n");
    
    
    
    
    
    //Wait for child processes to finish
    
    for (; q < num_processes; q++)
    {
        read(fd, buf, MAX_BUF);
        printf("Received: %s\n", buf);
        wait(NULL);
    }
    //Close
    close(fd);
    return 0;
}
