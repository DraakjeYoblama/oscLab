
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

typedef struct connection_params {
    tcpsock_t *client;
    sbuffer_t* buffer;
} connection_params_t;



int connmgr(connmgr_args_t args);
int connection(connection_params_t params);


#endif //CLAB6_TEST_SERVER_H


