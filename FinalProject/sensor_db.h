/**
 * \author Bert Lagaisse
 */

#ifndef _SENSOR_DB_H_
#define _SENSOR_DB_H_

#define CSV_NAME  "data.csv"

#include "sbuffer.h"

typedef struct storagemgr_args {
    sbuffer_t* buffer;
} storagemgr_args_t;

/**
 * Removes sensor data from the buffer and writes it to a csv file with name CSV_NAME
 * \param storage_args pointer to a struct that contains the buffer (and other arguments if necessary)
 */
int storagemgr(void* storage_args);


#endif /* _SENSOR_DB_H_ */