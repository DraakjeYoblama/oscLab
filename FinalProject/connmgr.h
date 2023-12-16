
// based on clab6/plab3

//
// Created by douwe on 16/12/23.
//

#ifndef CLAB6_TEST_SERVER_H
#define CLAB6_TEST_SERVER_H

#include "config.h"
#include "lib/tcpsock.h"

typedef struct connmgr_args {
    int argc;
    char **argv;
} connmgr_args_t;

int connmgr(connmgr_args_t args);
int connection(tcpsock_t *client);


#endif //CLAB6_TEST_SERVER_H


