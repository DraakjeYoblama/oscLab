#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "logger.h"

// pipes: reading end is 0, writing end is 1
int fd1[2]; //pipe 1
pid_t pid;

int write_to_log_process(char *msg){
    //TODO: fix this shit

    if (pid>0) {
        // parent
        write(fd1[1], msg, strlen(msg)+1);

    }

    return 0;
}

int create_log_process() {

    if (pipe(fd1) == -1) {
        //error
        return 1;
    }

    pid = fork();

    if (pid == -1) {
        //error
        return 1;
    }

    //TODO: use child process and stuff
    if (pid>0) {
        // parent


        write(fd1[1], "test", 5);
        close(fd1[1]);

    } else if (pid==0) {
        //child process
        char message1[50];
        FILE* fp = fopen("gateway.log", "a"); // replace file

        while (1) {
            pause();
            read(fd1[0], message1, 50);
            fprintf(fp, "%s\n", message1);
        }

    }

        return 0;
}

int end_log_process() {
    //TODO: end process and stuff
    return kill(pid, 0);
    //close(fd1[0]);
    //close(fd1[1]);
    //exit(0);
}

