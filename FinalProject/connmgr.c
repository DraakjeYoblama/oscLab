
// based on clab6/plab3

#include "connmgr.h"

int connmgr(connmgr_args_t args) {
    tcpsock_t *server, *client;
    int conn_counter = 0;

    if(args.argc < 3) {
        printf("Please provide the right arguments: first the port, then the max nb of clients");
        return -1;
    }

    int MAX_CONN = atoi(args.argv[2]);
    int PORT = atoi(args.argv[1]);

    pthread_t thread_id[MAX_CONN];

    printf("Test server is started\n");
    if (tcp_passive_open(&server, PORT) != TCP_NO_ERROR) exit(EXIT_FAILURE);
    do {
        if (tcp_wait_for_connection(server, &client) != TCP_NO_ERROR) exit(EXIT_FAILURE);
        printf("Incoming client connection\n");
        conn_counter++;

        // Create the thread
        if (pthread_create(&thread_id[conn_counter-1], NULL, (void *)connection, (void *)client) != 0) {
            printf("Failed to create thread\n");
            return -1;
        }

    } while (conn_counter < MAX_CONN);
    for (int i=0; i<MAX_CONN; i++) { // Wait for every thread to end
        pthread_join(thread_id[i], NULL);
    }
    if (tcp_close(&server) != TCP_NO_ERROR) exit(EXIT_FAILURE);
    printf("Test server is shutting down\n");
    return 0;
}

int connection(connection_params_t params) {
    int bytes, result;
    int i = 0;
    sensor_data_t data;
    char logmsg[50];

    printf("%lu\n", pthread_self()); // Print thread id for debugging reasons
    do {
        // read sensor ID
        bytes = sizeof(data.id);
        result = tcp_receive(params.client, (void *) &data.id, &bytes);

        if (i == 0) { // only on first loop
            // write to log
            sprintf(logmsg, "Sensor node %u has opened a new connection", data.id);
            write_to_log_process(logmsg);
            i++;
        }

        // read temperature
        bytes = sizeof(data.value);
        result = tcp_receive(params.client, (void *) &data.value, &bytes);
        // read timestamp
        bytes = sizeof(data.ts);
        result = tcp_receive(params.client, (void *) &data.ts, &bytes);
        if ((result == TCP_NO_ERROR) && bytes) {
            sbuffer_insert(params.buffer, &data);
            printf("sensor id = %" PRIu16 " - temperature = %g - timestamp = %ld\n", data.id, data.value,
                   (long int) data.ts);
        }
    } while (result == TCP_NO_ERROR);
    if (result == TCP_CONNECTION_CLOSED) {
        printf("Peer has closed connection\n");
    } else {
        printf("Error occured on connection to peer\n");
    }

    // write to log
    sprintf(logmsg, "Sensor node %u has closed the connection", data.id);
    write_to_log_process(logmsg);

    tcp_close(&params.client);
    return 0;
}




