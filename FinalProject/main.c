
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

// pipe: reading end is 0, writing end is 1
int fd1[2];
pid_t pid;
int logcounter;
FILE* logname;

int main(int argc, char *argv[]) {
    pthread_t connmgr_id, datamgr_id, storagemgr_id;
    connmgr_args_t conn_args;
    datamgr_args_t data_args;
    storagemgr_args_t storage_args;
    conn_args.argc = argc;
    conn_args.argv = argv;
    data_args.sensor_map = "room_sensor.map";
    storage_args.csv_name = "data.csv";

    // create logger child thread
    create_log_process();

    // start buffer
    sbuffer_t** shared_data = malloc(8);
    sbuffer_init(shared_data);
    conn_args.buffer = data_args.buffer = storage_args.buffer = *shared_data;

    // Create the manager threads

    pthread_create(&connmgr_id, NULL, (void*)connmgr, &conn_args);
    pthread_create(&datamgr_id, NULL, (void*)datamgr, &data_args);
    pthread_create(&storagemgr_id, NULL, (void*)storagemgr, &storage_args);


    // wait for threads to end
    pthread_join(connmgr_id, NULL);
    pthread_join(datamgr_id, NULL);
    pthread_join(storagemgr_id, NULL);

    // free buffer
    sbuffer_free(shared_data);

    // end logger thread
    end_log_process();
    return 0;
}

pthread_mutex_t pipemutex;
int write_to_log_process(char *msg){

    if (pid>0) {
        // parent
        pthread_mutex_lock(&pipemutex);
        write(fd1[1], msg, 25);
        pthread_mutex_unlock(&pipemutex);

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
