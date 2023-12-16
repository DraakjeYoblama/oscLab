
// based on milestone2/clab5/plab2

#include "sensor_db.h"
#include "config.h"

int storagemgr() {
    char logmsg[50];

    // open csv
    FILE* csv = fopen("data.csv", "w");
    write_to_log_process("A new data.csv file has been created.");

    // TODO: make this listen for incoming sensor messages in a loop
    // insert sensor
    fprintf(csv, "%d, %lf, %ld\n", sensor_id_t id, sensor_value_t value, sensor_ts_t ts);
    sprintf(logmsg, "Data insertion from sensor %u succeeded.", sensor_id_t id);
    write_to_log_process(logmsg);

    // close csv
    fclose(csv);
    write_to_log_process("The data.csv file has been closed.");
    return 0;
}