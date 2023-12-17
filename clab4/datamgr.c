#include <assert.h>
#include <stdio.h>
#include "datamgr.h"
#include "lib/dplist.h"

dplist_t *list;

void datamgr_parse_sensor_files(FILE *fp_sensor_map, FILE *fp_sensor_data) {

    // Part 1: make dplist from sensor_map

    list = dpl_create(element_copy, element_free, element_compare);
    char line[12]; // 2x uint16 (max. 5 digits) + space + string terminator = 12 characters
    my_element_t temp_element;
    temp_element.ra_lastadded = 0;
    temp_element.last_modified = 0;
    for (int i=0; i<RUN_AVG_LENGTH; i++) {
        temp_element.running_avg[i] = -300;
    }

    int index_dpl = 1;
    while (fgets(line, sizeof(line), fp_sensor_map)) {
        if (sscanf(line, "%hd %hd", &temp_element.room_id, &temp_element.id) == 2) {
            dpl_insert_at_index(list, &temp_element, index_dpl, true);
        }
        index_dpl++;
    }

    // Part 2: fill dplist with data from sensor_data

    sensor_data_t sensor_data; //defined in config.h
    my_element_t* temp_node;
    // index_dpl gets reused, how fun

    while (!feof(fp_sensor_data)) {
        // Read the content as a sensor_data_t
        fread(&sensor_data.id, 2, 1, fp_sensor_data);
        fread(&sensor_data.value, 8, 1, fp_sensor_data);
        fread(&sensor_data.ts, 8, 1, fp_sensor_data);

        //replace id with 0 at end of file and break out of the while loop
        if (feof(fp_sensor_data)) {
            sensor_data.id = 0;
            break;
        }

        // add values from binary file to the dplist
        index_dpl = dpl_get_index_of_element(list, &sensor_data);
        //printf("%d\n", index_dpl);
        if (index_dpl == -1) {
            // values not in .map file should be dropped, with a log message saying so
            fprintf(stderr, "Sensor with that ID not in list\n");
        } else {
            //temp_node = dpl_get_reference_at_index(list, index_dpl);
            temp_node = (my_element_t *)dpl_get_element_at_index(list, index_dpl);
            temp_node->last_modified = sensor_data.ts;
            if (++temp_node->ra_lastadded >= RUN_AVG_LENGTH) {
                temp_node->ra_lastadded = 0;
            }
            temp_node->running_avg[temp_node->ra_lastadded] = sensor_data.value;


        }
    }
}

void datamgr_free() {
    dpl_free(&list, true);
    free(list);
}

uint16_t datamgr_get_room_id(sensor_id_t sensor_id) {
    int index_dpl;
    uint16_t temp_room = 0;
    my_element_t* vessel_node = malloc(sizeof(my_element_t)); // it's only a vessel to move the sensor id
    vessel_node->id = sensor_id;
    index_dpl = dpl_get_index_of_element(list, vessel_node);
    if (index_dpl == -1) {
        fprintf(stderr, "Sensor with that ID not in list\n");
    } else {
        my_element_t* temp_node = (my_element_t *) dpl_get_element_at_index(list, index_dpl);
        temp_room = temp_node->room_id;
    }
    free(vessel_node);
    vessel_node = NULL;
    return temp_room;
}

sensor_value_t datamgr_get_avg(sensor_id_t sensor_id) {
    int index_dpl;
    my_element_t* vessel_node = malloc(sizeof(my_element_t));
    double average = 0;
    vessel_node->id = sensor_id;
    index_dpl = dpl_get_index_of_element(list, vessel_node);
    if (index_dpl == -1) {
        fprintf(stderr, "Sensor with that ID not in list\n");
    } else {
        my_element_t* temp_node = (my_element_t *) dpl_get_element_at_index(list, index_dpl);
        for (int i=0; i<RUN_AVG_LENGTH; i++) {
            if (temp_node->running_avg[i] > -275) {
                average += temp_node->running_avg[i];
            }
        }
        average = average/RUN_AVG_LENGTH;
    }

    printf("Temp: %lf\n", average);
    if (average > SET_MAX_TEMP) {
        printf("It's too warm\n");
    } else if (average < SET_MIN_TEMP) {
        printf("It's too cold\n");
    }

    free(vessel_node);
    vessel_node = NULL;
    return average;
}

time_t datamgr_get_last_modified(sensor_id_t sensor_id) {
    int index_dpl;
    time_t temp_time = 0;
    my_element_t* vessel_node = malloc(sizeof(my_element_t));
    vessel_node->id = sensor_id;
    index_dpl = dpl_get_index_of_element(list, vessel_node);
    if (index_dpl == -1) {
        fprintf(stderr, "Sensor with that ID not in list\n");
    } else {
        my_element_t* temp_node = (my_element_t *) dpl_get_element_at_index(list, index_dpl);
        temp_time = temp_node->last_modified;
    }
    free(vessel_node);
    vessel_node = NULL;
    return temp_time;
}

int datamgr_get_total_sensors() {
    return dpl_size(list);
}

void *element_copy(void *element)
{
    my_element_t *copy = malloc(sizeof(my_element_t));

    assert(copy != NULL);
    copy->id = ((my_element_t *)element)->id;
    copy->room_id = ((my_element_t *)element)->room_id;
    copy->last_modified = ((my_element_t *)element)->last_modified;
    copy->ra_lastadded = ((my_element_t *)element)->ra_lastadded;
    for (int i=0; i<RUN_AVG_LENGTH; i++) {
        copy->running_avg[i] = ((my_element_t *)element)->running_avg[i];
    }
    return (void *)copy;
}

void element_free(void **element)
{
    free(*element);
    *element = NULL;
}

int element_compare(void *x, void *y) // returns 0 if id is equal
{
    //return ((((my_element_t *)x)->id < ((my_element_t *)y)->id) ? -1 : (((my_element_t *)x)->id == ((my_element_t *)y)->id) ? 0
    //                                                                                                                        : 1);

    // refactored to be more readable
    my_element_t *element_x = (my_element_t *)x;
    my_element_t *element_y = (my_element_t *)y;

    if (element_x->id < element_y->id) {
        return -1;
    } else if (element_x->id == element_y->id) {
        return 0;
    } else {
        return 1;
    }
}
