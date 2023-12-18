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

/**
 * writes a log message to the pipe, to be processed by log_pipe_to_file
 * \param msg the log message
 */
int write_to_log_process(char *msg);

/**
 * Reads the first data in the pipe and writes it to a log file
 */
int log_pipe_to_file();

/**
 * Opens pipe and forks process, unction depends on parent or child process calling it
 * \return 0 on succes and 1 if an error occured
 */
int create_log_process();

/**
 * Ends the log process, function depends on parent or child process calling it
 */
int end_log_process();

#endif /* _CONFIG_H_ */

