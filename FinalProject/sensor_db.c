
// based on milestone2/clab5/plab2 and milestone3

#include "sensor_db.h"

int storagemgr(storagemgr_args_t args) {
    char logmsg[50];

    // open csv
    FILE* csv = fopen(args.csv_name, "w");
    write_to_log_process("A new data.csv file has been created.");

    // insert sensor
    sensor_data_t received_data;

    while (1) {
        // get data from buffer
        if (sbuffer_remove(args.buffer, &received_data, 2) == 0) {

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
    write_to_log_process("The data.csv file has been closed.");
    return 0;
}