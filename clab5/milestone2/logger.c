#include "logger.h"

int write_to_log_process(char *msg){
    //TODO
    return 0;
};

int create_log_process() {
    //TODO
    return 0;
};

int end_log_process() {
    //TODO
    return 0;
};

/* TODO: use child process and stuff

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <ctype.h>
#include <sys/types.h>


#include "main.h"

int main() {

    int fd1[2]; //pipe 1
    int fd2[2]; //pipe 2

    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        //error
        return 1;
    }

    pid_t pid = fork();

    if (pid == -1) {
        //error
        return 1;
    }

    if (pid==0) {
        //child process
        close(fd1[1]);

        char message1[50];
        char message2[50];
        read(fd1[0], message1, 50);

        for (int i=0; i<strlen(message1); i++) {
            if (islower(message1[i]) == 0) {
                message2[i] = tolower(message1[i]);
            } else {
                message2[i] = toupper(message1[i]);
            }
        }
        message2[strlen(message1)+1] = '\0';


        close(fd1[0]);
        close(fd2[0]);

        write(fd2[1], message2, strlen(message2)+1);
        close(fd2[1]);


    } else if (pid>0) {
        // parent
        char* message1 = "Hi There6";
        char message2[50];
        close(fd1[0]);
        write(fd1[1], message1, strlen(message1)+1);
        close(fd1[1]);

        //wait for child to send string
        wait(NULL);
        close(fd2[1]);

        read(fd2[0], message2, 50);
        printf("%s\n", message2);
        close(fd2[0]);
    }

    return 0;
};

 */