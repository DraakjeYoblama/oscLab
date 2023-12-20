#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "config.h"
#include "lib/tcpsock.h"
#include "sbuffer.h"
#include "connmgr.h"

int connmgr(void* connmgr_args) {
    connmgr_args_t* mgr_args = (connmgr_args_t*)connmgr_args;
    tcpsock_t *server;

    if(mgr_args->argc < 3) {
        printf("Please provide the right arguments: first the port, then the max nb of clients");
        return -1;
    }

    int MAX_CONN = atoi(mgr_args->argv[2]);
    int PORT = atoi(mgr_args->argv[1]);


    // arguments for the connection with client
    pthread_t thread_id[MAX_CONN];
    conn_args_t* cl_args[MAX_CONN];
    for(int i = 0; i < MAX_CONN; i++) {
        cl_args[i] = malloc(sizeof(conn_args_t));
        cl_args[i]->buffer = mgr_args->buffer;
    }

    // start listening for client connections
    write_to_log_process("Server started");
    if (tcp_passive_open(&server, PORT) != TCP_NO_ERROR) exit(EXIT_FAILURE);

    for (int conn_counter= 0; conn_counter < MAX_CONN; conn_counter++) {
        // wait for connection (blocks until connection is found)
        if (tcp_wait_for_connection(server, &cl_args[conn_counter]->client) != TCP_NO_ERROR) exit(EXIT_FAILURE);
        //Incoming client connection

        // Create the thread
        if (pthread_create(&thread_id[conn_counter], NULL, (void *)connection, cl_args[conn_counter]) != 0) {
            write_to_log_process("Failed to create connection thread\n");
            return -1;
        }

    }
    for (int i=0; i<MAX_CONN; i++) { // Wait for every thread to end
        pthread_join(thread_id[i], NULL);
    }

    // indicate end of sbuffer
    sensor_data_t data;
    data.id = 0;
    sbuffer_insert(mgr_args->buffer, &data, 0);

    if (tcp_close(&server) != TCP_NO_ERROR) exit(EXIT_FAILURE);

    for(int i = 0; i < MAX_CONN; i++) {
        free(cl_args[i]);
    }
    write_to_log_process("All connections closed - connection manager finished");
    return 0;
}

int connection(void* connection_args) {
    conn_args_t* cl_args = (conn_args_t*)connection_args;
    int bytes, result;
    int id = 0;
    sensor_data_t data;
    char logmsg[LOG_MESSAGE_LENGTH];

    //printf("%lu\n", pthread_self()); // Print thread id for debugging reasons
    do {
        // read sensor ID
        bytes = sizeof(data.id);
        result = tcp_receive(cl_args->client, (void *) &data.id, &bytes);

        if (id == 0) { // only on first loop
            // write to log
            id = data.id; // a persistent value for the id that doesn't corrupt on timeout
            sprintf(logmsg, "Sensor node %u has opened a new connection", id);
            write_to_log_process(logmsg);
        }
        if (result != TCP_NO_ERROR) { // error on receiving id data
            break;
        }

        // read temperature
        bytes = sizeof(data.value);
        result = tcp_receive(cl_args->client, (void *) &data.value, &bytes);

        if (result != TCP_NO_ERROR) { // error on receiving temperature data
            break;
        }

        // read timestamp
        bytes = sizeof(data.ts);
        result = tcp_receive(cl_args->client, (void *) &data.ts, &bytes);
        if ((result == TCP_NO_ERROR) && bytes) {
            sbuffer_insert(cl_args->buffer, &data, 0);
        }
    } while (result == TCP_NO_ERROR);

    // write to log
    if (result == TCP_CONNECTION_CLOSED) {
        sprintf(logmsg, "Sensor node %u has closed the connection", id);
    } else if (result == TCP_CONNECTION_TIMEOUT) {
        sprintf(logmsg, "Sensor node %u has timed out, connection closed", id);
    } else {
        sprintf(logmsg, "Error connecting to sensor node %u, connection closed", id);
    }
    write_to_log_process(logmsg);

    tcp_close(&cl_args->client);
    return 0;
}




