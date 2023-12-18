
// based on clab6/plab3

//
// Created by douwe on 16/12/23.
//

#ifndef CLAB6_TEST_SERVER_H
#define CLAB6_TEST_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <pthread.h>
#include "config.h"
#include "lib/tcpsock.h"
#include "sbuffer.h"

typedef struct connmgr_args {
    int argc;
    char **argv;
    sbuffer_t* buffer;
} connmgr_args_t;

typedef struct conn_args {
    tcpsock_t *client;
    sbuffer_t* buffer;
} conn_args_t;


/**
 * Starts up connections with TCP clients
 * \param conn_args pointer to a struct that contains the buffer and the arguments passed from main() (and other arguments if necessary)
 */
int connmgr(void* conn_args);

/**
 * TCP client connection that writes received data to the shared buffer
 * \param connection_args pointer to a struct that contains the client socket and buffer (and other arguments if necessary)
 */
int connection(void* connection_args);


#endif //CLAB6_TEST_SERVER_H


