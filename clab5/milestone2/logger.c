#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
//#include <sys/types.h>
//#include <sys/wait.h>
#include <unistd.h>
#include<time.h>
#include<signal.h>

#include "logger.h"

// pipes: reading end is 0, writing end is 1
int fd1[2];
pid_t pid;
int logcounter;
FILE* logname;


int write_to_log_process(char *msg){

    if (pid>0) {
        // parent
        write(fd1[1], msg, strlen(msg)+1);
    } else if (pid==0) {
        time_t now;
        char message1[30];
        time(&now);
        read(fd1[0], message1, 30);
        fprintf(logname, "%d - %.24s - %s\n", logcounter, ctime(&now), message1);
        logcounter++;
    }

    return 0;
}

int create_log_process() {

    if (pipe(fd1) == -1) {
        //error
        return 1;
    }
    logcounter = 0;

    pid = fork();

    if (pid == -1) {
        //error
        return 1;
    }

    if (pid>0) {
        // parent
        close(fd1[0]);

    } else if (pid==0) {
        //child process
        close(fd1[1]);
        logname = fopen("gateway.log", "a"); // append file

    }

    return 0;
}

int end_log_process() {
    return kill(pid, 0);
    //close(fd1[0]);
    //close(fd1[1]);
    //exit(0);
}

