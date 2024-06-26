#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
//#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include<time.h>
//#include<signal.h>

#include "logger.h"

// pipes: reading end is 0, writing end is 1
int fd1[2];
pid_t pid;
int logcounter;
FILE* logname;


int write_to_log_process(char *msg){

    if (pid>0) {
        // parent
        write(fd1[1], msg, 25);

    } else if (pid==0) {
        time_t now;
        char message1[25];
        time(&now);
        if (read(fd1[0], message1, 25) > 0) {
        fprintf(logname, "%d - %.24s - %s\n", logcounter, ctime(&now), message1);
    	}
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
        
        if (logname == NULL) {
            return 1;
        }
        // make log line buffered, so no logs get lost
        if (setvbuf(logname, NULL, _IOLBF, 0) != 0) {
            return 1;
        }

    }

    return 0;
}

int end_log_process() {
    if (pid>0) {
        // parent
        close(fd1[0]);
        close(fd1[1]);
        wait(NULL);

    } else if (pid==0) {
        fclose(logname);
        close(fd1[0]);
        close(fd1[1]);
        exit(0);
    }
    return 0;
}

