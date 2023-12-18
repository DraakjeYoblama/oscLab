
// based on milestone2/clab5/plab2 (logger.c/.h)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include "config.h"
#include "sbuffer.h"
#include "connmgr.h"
#include "datamgr.h"
#include "sensor_db.h"

#define LOG_FILE_NAME  "gateway.log"

// pipe: reading end is 0, writing end is 1
int fd1[2];
pid_t pid;
int logcounter;
FILE* log_file;

int main(int argc, char *argv[]) {

    // create logger child thread
    int result = 0;
    create_log_process();
    if (pid == 0) {
        // Child process
        while (result == 0) {
            result = log_pipe_to_file();
        }
        end_log_process();
        return 0;
    }
    write_to_log_process("Log process started");

    // pass arguments to manager threads
    connmgr_args_t *conn_args = malloc(sizeof *conn_args);
    datamgr_args_t *data_args= malloc(sizeof *data_args);
    storagemgr_args_t *storage_args= malloc(sizeof *storage_args);

    conn_args->argc = argc;
    conn_args->argv = argv;

    // start buffer and set as argument for manager threads
    sbuffer_t** shared_data = malloc(8);
    sbuffer_init(shared_data);
    conn_args->buffer = data_args->buffer = storage_args->buffer = *shared_data;


    // Create the manager threads
    pthread_t connmgr_id, datamgr_id, storagemgr_id;
    pthread_create(&connmgr_id, NULL, (void*)connmgr, conn_args);
    pthread_create(&datamgr_id, NULL, (void*)datamgr, data_args);
    pthread_create(&storagemgr_id, NULL, (void*)storagemgr, storage_args);


    // wait for threads to end
    pthread_join(connmgr_id, NULL);
    pthread_join(datamgr_id, NULL);
    pthread_join(storagemgr_id, NULL);

    // free buffer
    sbuffer_free(shared_data);
    free(shared_data);

    // end logger thread
    write_to_log_process("Log process closed");
    end_log_process();
    return 0;
}

pthread_mutex_t pipemutex;
int write_to_log_process(char *msg){
    pthread_mutex_lock(&pipemutex);
    write(fd1[1], msg, 60);
    pthread_mutex_unlock(&pipemutex);
    return 0;
}

int log_pipe_to_file() {
    time_t now;
    char message1[60];
    time(&now);
    if (read(fd1[0], message1, 60) > 0) {
        fprintf(log_file, "%d - %.24s - %s\n", logcounter, ctime(&now), message1);
    } else {
        return 1;
    }
    logcounter++;
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
        log_file = fopen(LOG_FILE_NAME, "a"); // append file

        if (log_file == NULL) {
            return 1;
        }
        // make log line buffered, so no logs get lost
        if (setvbuf(log_file, NULL, _IOLBF, 0) != 0) {
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
        fclose(log_file);
        close(fd1[0]);
        close(fd1[1]);
        exit(0);
    }
    return 0;
}
