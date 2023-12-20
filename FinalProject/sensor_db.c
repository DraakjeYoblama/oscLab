#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "sbuffer.h"
#include "sensor_db.h"

int storagemgr(void* storage_args) {
    storagemgr_args_t* args = (storagemgr_args_t*)storage_args;
    char logmsg[LOG_MESSAGE_LENGTH];

    // open csv
    FILE* csv = fopen(CSV_NAME, "w"); // create empty file
    if (csv == NULL) {
        return 1;
    }
    // make file line buffered
    if (setvbuf(csv, NULL, _IOLBF, 0) != 0) { // TODO: maybe remove this debugging part
        // bummer, no line buffering for you
        /*fclose(csv);
        return 1;*/
    }
    write_to_log_process("A new " CSV_NAME " file has been created.");

    // insert sensor
    sensor_data_t received_data;

    while (1) {
        // get data from buffer
        if (sbuffer_remove(args->buffer, &received_data, 2) == 0) {

            // write data to sensor_data_out.csv
            if (received_data.id != 0) {
                //printf("%lu: %d, %lf, %ld\n", pthread_self(), received_data.id, received_data.value, received_data.ts);

                fprintf(csv, "%d, %lf, %ld\n", received_data.id, received_data.value, received_data.ts);
                sprintf(logmsg, "Data insertion from sensor %u succeeded.", received_data.id);
                write_to_log_process(logmsg);
            } else {
                break;
            }
        }
    }

    // close csv
    fclose(csv);
    write_to_log_process("The " CSV_NAME " file has been closed.");
    return 0;
}