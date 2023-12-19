
// based on clab6/plab3

#include "connmgr.h"

int connmgr(void* connmgr_args) {
    connmgr_args_t* mgr_args = (connmgr_args_t*)connmgr_args;
    tcpsock_t *server, *client; // TODO: does client need to be client[MAX_CONN]? It seems like data is going to be overwritten
    int conn_counter = 0;

    if(mgr_args->argc < 3) {
        printf("Please provide the right arguments: first the port, then the max nb of clients");
        return -1;
    }

    int MAX_CONN = atoi(mgr_args->argv[2]);
    int PORT = atoi(mgr_args->argv[1]);

    pthread_t thread_id[MAX_CONN];

    // arguments for the connection with client
    conn_args_t* cl_args = malloc(sizeof *cl_args);
    cl_args->buffer = mgr_args->buffer;

    write_to_log_process("Server started");
    if (tcp_passive_open(&server, PORT) != TCP_NO_ERROR) exit(EXIT_FAILURE);
    do {
        if (tcp_wait_for_connection(server, &client) != TCP_NO_ERROR) exit(EXIT_FAILURE);
        //Incoming client connection
        conn_counter++;
        cl_args->client = client; // pass client ID to

        // Create the thread
        if (pthread_create(&thread_id[conn_counter-1], NULL, (void *)connection, cl_args) != 0) {
            write_to_log_process("Failed to create connection thread\n");
            return -1;
        }

    } while (conn_counter < MAX_CONN);
    for (int i=0; i<MAX_CONN; i++) { // Wait for every thread to end
        pthread_join(thread_id[i], NULL);
    }

    // indicate end of sbuffer
    sensor_data_t data;
    data.id = 0;
    sbuffer_insert(mgr_args->buffer, &data, 0);

    if (tcp_close(&server) != TCP_NO_ERROR) exit(EXIT_FAILURE);
    free(cl_args);
    printf("Test server is shutting down\n");
    return 0;
}

int connection(void* connection_args) {
    conn_args_t* cl_args = (conn_args_t*)connection_args;
    int bytes, result;
    int i = 0;
    sensor_data_t data;
    char logmsg[60];

    //printf("%lu\n", pthread_self()); // Print thread id for debugging reasons
    do {
        // read sensor ID
        bytes = sizeof(data.id);
        result = tcp_receive(cl_args->client, (void *) &data.id, &bytes);

        if (i == 0) { // only on first loop
            // write to log
            sprintf(logmsg, "Sensor node %u has opened a new connection", data.id);
            write_to_log_process(logmsg);
            i++;
        }

        // read temperature
        bytes = sizeof(data.value);
        result = tcp_receive(cl_args->client, (void *) &data.value, &bytes);
        // read timestamp
        bytes = sizeof(data.ts);
        result = tcp_receive(cl_args->client, (void *) &data.ts, &bytes);
        if ((result == TCP_NO_ERROR) && bytes) {
            sbuffer_insert(cl_args->buffer, &data, 0);
        }
    } while (result == TCP_NO_ERROR);

    // write to log
    if (result == TCP_CONNECTION_CLOSED) {
        sprintf(logmsg, "Sensor node %u has closed the connection", data.id);
    } else if (result == TCP_CONNECTION_TIMEOUT) {
        sprintf(logmsg, "Sensor node %u has timed out, connection closed", data.id);
    } else {
        sprintf(logmsg, "Error connecting to sensor node %u, connection closed", data.id);
    }
    write_to_log_process(logmsg);

    tcp_close(&cl_args->client);
    return 0;
}




