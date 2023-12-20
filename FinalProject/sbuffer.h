#ifndef _SBUFFER_H_
#define _SBUFFER_H_

#include "config.h"

#define SBUFFER_FAILURE -1
#define SBUFFER_SUCCESS 0
#define SBUFFER_NO_DATA 1

typedef struct sbuffer sbuffer_t;

/**
 * Allocates and initializes a new shared buffer
 * \param buffer a double pointer to the buffer that needs to be initialized
 * \return SBUFFER_SUCCESS on success and SBUFFER_FAILURE if an error occurred
 */
int sbuffer_init(sbuffer_t **buffer);

/**
 * All allocated resources are freed and cleaned up
 * \param buffer a double pointer to the buffer that needs to be freed
 * \return SBUFFER_SUCCESS on success and SBUFFER_FAILURE if an error occurred
 */
int sbuffer_free(sbuffer_t **buffer);

/**
 * Removes the first sensor data in 'buffer' (at the 'head') and returns this sensor data as '*data'
 * If 'buffer' is empty, the function blocks until new sensor data becomes available
 * \param buffer a pointer to the buffer that is used
 * \param data a pointer to pre-allocated sensor_data_t space, the data will be copied into this structure. No new memory is allocated for 'data' in this function.
 * \param stage what stage in the process is this? Only reads nodes with a stage of 1 below given value
 * \return SBUFFER_SUCCESS on success and SBUFFER_FAILURE if an error occurred
 */
int sbuffer_remove(sbuffer_t *buffer, sensor_data_t *data, int stage);

/**
 * Reads the first sensor data in 'buffer' (at the 'head') and returns this sensor data as '*data'
 * Does not remove the data, instead changes a flag in that data
 * If 'buffer' is empty, the function blocks until new sensor data becomes available
 * \param buffer a pointer to the buffer that is used
 * \param data a pointer to pre-allocated sensor_data_t space, the data will be copied into this structure. No new memory is allocated for 'data' in this function.
 * \param stage what stage in the process is this? Only reads nodes with a stage of 1 below given value and increments their stage to the given value
 * \return SBUFFER_SUCCESS on success and SBUFFER_FAILURE if an error occurred
 */
int sbuffer_read(sbuffer_t *buffer, sensor_data_t *data, int stage);

/**
 * Inserts the sensor data in 'data' at the end of 'buffer' (at the 'tail')
 * \param buffer a pointer to the buffer that is used
 * \param data a pointer to sensor_data_t data, that will be copied into the buffer
 * \param stage what stage in the process is this? Sets the stage for every node to this value
 * \return SBUFFER_SUCCESS on success and SBUFFER_FAILURE if an error occured
*/
int sbuffer_insert(sbuffer_t *buffer, sensor_data_t *data, int stage);

/**
 * Sends signals to release condition variables
 * \param amount amount of signals sent
 * \return SBUFFER_SUCCESS on success
*/
int sbuffer_cond(int amount);

#endif  //_SBUFFER_H_