
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>



int main() {
    
    printf("Completed\n");
    
    //Create FIFO
    int fd1, fd2;
    char * myfifo = "/tmp/myfifo";
    
    pid_t pid1 = fork();
    if (pid1 < 0)
    {
        perror("fork is failed");
        exit(1);
    }
    else if (pid1 == 0)          //in the child1
    {

        execlp("./sigperf1", "sigperf1","0",NULL);
        
        // open, read, and display the message from the FIFO
        
        fd2 = open(myfifo, O_RDONLY);
        
        if (fcntl(fd2, F_GETFD) == -1)
        {
            perror("fd2 is failed");
            exit(1);
        }
        //other child can write
    }
    
    pid_t pid2 = fork();
    
    if (pid2 < 0) {
        perror("fork is failed");
        exit(1);
    }
    else if (pid2 == 0)          //in the child2
    {

        execlp("./sigperf1", "sigperf1","0",NULL);
        
        /* write "Hello" to the FIFO */
        fd1 = open(myfifo, O_WRONLY);
        if (fcntl(fd1, F_GETFD) == -1) {
            perror("open failed");
            exit(1);
            

        }
        write(fd1, "Hello", sizeof("Hello"));
    }
    

    
    
    
    
    
   
    
    //close(fd);
    
    /* remove the FIFO */
    //unlink(myfifo);   
}
