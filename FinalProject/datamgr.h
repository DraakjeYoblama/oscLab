
// based on clab4/plab1

#ifndef DATAMGR_H_
#define DATAMGR_H_

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "sbuffer.h"
#include "config.h"
#include "lib/dplist.h"


#ifndef RUN_AVG_LENGTH
#define RUN_AVG_LENGTH 5
#endif

#ifndef SET_MAX_TEMP
#error SET_MAX_TEMP not set
#endif

#ifndef SET_MIN_TEMP
#error SET_MIN_TEMP not set
#endif

/*
 * Use ERROR_HANDLER() for handling memory allocation problems, invalid sensor IDs, non-existing files, etc.
 */
#define ERROR_HANDLER(condition, ...)    do {                       \
                      if (condition) {                              \
                        printf("\nError: in %s - function %s at line %d: %s\n", __FILE__, __func__, __LINE__, __VA_ARGS__); \
                        exit(EXIT_FAILURE);                         \
                      }                                             \
                    } while(0)


typedef struct
{
    uint16_t id;
    uint16_t room_id;
    double running_avg[RUN_AVG_LENGTH];
    int ra_lastadded;
    time_t last_modified;
} my_element_t;

typedef struct datamgr_args {
    char* sensor_map;
    sbuffer_t* buffer;
} datamgr_args_t;

void *element_copy(void *element);
void element_free(void **element);
int element_compare(void *x, void *y);

/**
 *  This method holds the core functionality of datamgr.
 *  \param args contains the name of the .map file and a pointer to the shared buffer
 */
int datamgr(datamgr_args_t args);

/**
 * This method should be called to clean up the datamgr, and to free all used memory.
 * \param sensor_map .map file to initiate dplist from
 */
int datamgr_parse_map(char* sensor_map);

/**
 * This method should be called to clean up the datamgr, and to free all used memory.
 * After this, any call to datamgr_get_avg or datamgr_get_total_sensors will not return a valid result
 */
void datamgr_free();

/**
 * Gets the running AVG of a certain sensor ID
 * Values that are physically impossible (below 0K) are not counted
 * \param node the sensor node to check the average temperature for
 * \return the running AVG of the given sensor
 */
sensor_value_t datamgr_get_avg(my_element_t* node);

/**
 *  Return the total amount of unique sensor ID's recorded by the datamgr
 *  \return the total amount of sensors
 */
int datamgr_get_total_sensors();

#endif  //DATAMGR_H_