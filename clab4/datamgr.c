#include <assert.h>
#include "datamgr.h"
#include "lib/dplist.h"

dplist_t *list;

void datamgr_parse_sensor_files(FILE *fp_sensor_map, FILE *fp_sensor_data) {

    list = dpl_create(element_copy, element_free, element_compare);

    char line[12];
    my_element_t temp_element;
    int i = 1;
    while (fgets(line, sizeof(line), fp_sensor_map)) {
        if (sscanf(line, "%d %hd", &temp_element.room_id, &temp_element.id) == 2) {
            dpl_insert_at_index(list, &temp_element, i, true);
        }
        i++;
    }

    // Open sensor data file in read mode

    sensor_data_t sensor_data; //defined in config.h

    void* test;
    int index_dpl;

    while (!feof(fp_sensor_data)) {
        // Read the content as a sensor_data_t
        fread(&sensor_data.id, 2, 1, fp_sensor_data);
        fread(&sensor_data.value, 8, 1, fp_sensor_data);
        fread(&sensor_data.ts, 8, 1, fp_sensor_data);


        //replace id with 0 at end of file
        if (feof(fp_sensor_data)) {
            sensor_data.id = 0;
            break;
        }

        // TODO: add values from binary file to the dplist
        index_dpl = dpl_get_index_of_element(list, &sensor_data);
        printf("%d\n", index_dpl);
        if (index_dpl == -1) {
            // values not in .map file should be dropped, with a log message saying so
            printf("element not in .map file\n");
        }
        test = dpl_get_element_at_index(list, index_dpl);
        //temp_element = test;
    }
}

void datamgr_free() {
    dpl_free(&list, true);
    free(list);
}

uint16_t datamgr_get_room_id(sensor_id_t sensor_id) {
    // TODO: write this
    // give the room id value for element with given id
}

sensor_value_t datamgr_get_avg(sensor_id_t sensor_id) {
    // TODO: write this
    // give the running average value for element with given id
}

time_t datamgr_get_last_modified(sensor_id_t sensor_id) {
    // TODO: write this
    // give the last modified value for element with given id
}

int datamgr_get_total_sensors() {
    // TODO: write this
    // go through all elements of dplist, count them
}

void *element_copy(void *element)
{
    my_element_t *copy = malloc(sizeof(my_element_t));

    assert(copy != NULL);
    copy->id = ((my_element_t *)element)->id;
    copy->room_id = ((my_element_t *)element)->room_id;
    copy->running_avg = ((my_element_t *)element)->running_avg;
    copy->last_modified = ((my_element_t *)element)->last_modified;
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