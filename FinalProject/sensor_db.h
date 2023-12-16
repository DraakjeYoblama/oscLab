
// based on milestone2/clab5/plab2

/**
 * \author Bert Lagaisse
 */

#ifndef _SENSOR_DB_H_
#define _SENSOR_DB_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "config.h"
#include "sbuffer.h"

typedef struct storagemgr_args {
    char* csv_name;
    sbuffer_t* buffer;
} storagemgr_args_t;

int storagemgr(storagemgr_args_t args);


#endif /* _SENSOR_DB_H_ */