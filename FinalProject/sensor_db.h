
// based on milestone2/clab5/plab2

/**
 * \author Bert Lagaisse
 */

#ifndef _SENSOR_DB_H_
#define _SENSOR_DB_H_

#define CSV_NAME  "data.csv"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "config.h"
#include "sbuffer.h"

typedef struct storagemgr_args {
    sbuffer_t* buffer;
} storagemgr_args_t;

/**
 * Removes sensor data from the buffer and writes it to a csv file with name CSV_NAME
 * \param args struct that contains the buffer (and other arguments if necessary)
 */
int storagemgr(storagemgr_args_t args);


#endif /* _SENSOR_DB_H_ */