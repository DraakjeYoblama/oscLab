
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

typedef struct client_params {
    tcpsock_t *client;
    sbuffer_t* buffer;
} client_params_t;


/**
 * Starts up connections with TCP clients
 * \param args struct that contains the buffer and the arguments passed from main() (and other arguments if necessary)
 */
int connmgr(connmgr_args_t args);

/**
 * TCP client connection that writes received data to the shared buffer
 * \param params struct that contains the client socket and buffer (and other arguments if necessary)
 */
int connection(client_params_t params);


#endif //CLAB6_TEST_SERVER_H


