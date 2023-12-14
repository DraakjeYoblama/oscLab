#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdint.h>
#include <time.h>

typedef uint16_t sensor_id_t;
typedef double sensor_value_t;
typedef time_t sensor_ts_t;         // UTC timestamp as returned by time() - notice that the size of time_t is different on 32/64 bit machine

typedef struct {
    sensor_id_t id;
    sensor_value_t value;
    sensor_ts_t ts;
} sensor_data_t;

#endif /* _CONFIG_H_ */

// TODO: the next part is just copied over from milestone 2/plab2/clab5 (logger.h), make it correct and fix bugs

#include <stdio.h>
#include <stdlib.h>

int main();
int write_to_log_process(char *msg);
int create_log_process();
int end_log_process();
