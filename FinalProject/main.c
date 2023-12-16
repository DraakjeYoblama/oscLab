
// based on milestone2/clab5/plab2 (logger.c/.h)

// start header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>
#include<time.h>
#include "config.h"
#include "sbuffer.h"
#include "connmgr.h"
#include "datamgr.h"
#include "sensor_db.h"

int main(int argc, char *argv[]);

// end header


// pipes: reading end is 0, writing end is 1
int fd1[2];
pid_t pid;
int logcounter;
FILE* logname;

int main(int argc, char *argv[]) {
    pthread_t connmgr_id, datamgr_id, storagemgr_id;
    connmgr_args_t conn_args;
    conn_args.argc = argc;
    conn_args.argv = argv;

    // create logger child thread
    create_log_process();

    // Create the threads
    pthread_create(&connmgr_id, NULL, (void*)connmgr, &conn_args); // TODO: add functions
    pthread_create(&datamgr_id, NULL, (void*)datamgr_parse_sensor_files, NULL);
    pthread_create(&storagemgr_id, NULL, (void*)open_db, NULL);


    // wait for threads to end
    pthread_join(connmgr_id, NULL);
    pthread_join(datamgr_id, NULL);
    pthread_join(storagemgr_id, NULL);

    end_log_process();

    return 0;
}


int write_to_log_process(char *msg){

    if (pid>0) {
        // parent
        write(fd1[1], msg, 25);

    } else if (pid==0) {
        time_t now;
        char message1[25];
        time(&now);
        read(fd1[0], message1, 25);
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
    if (pid>0) {
        // parent
        wait(NULL);
        close(fd1[0]);
        close(fd1[1]);

    } else if (pid==0) {
        fclose(logname);
        close(fd1[0]);
        close(fd1[1]);
        exit(0);
    }
    return 0;
}
