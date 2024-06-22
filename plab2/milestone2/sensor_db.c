#include "sensor_db.h"
#include "logger.h"

//TODO: use better log messages

FILE * open_db(char * filename, bool append) {
    create_log_process();
    FILE* csv;

    if (!append) {
        csv = fopen(filename, "w"); // replace file
    } else {
        csv = fopen(filename, "a"); //append to existing file
    }
    write_to_log_process("Data file opened.");

    return csv;
}

int insert_sensor(FILE * f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts) {
    fprintf(f, "%d, %lf, %ld\n", id, value, ts);
    write_to_log_process("Data inserted.");
    return 0;
}

int close_db(FILE * f){
    fclose(f);
    write_to_log_process("Data file closed.");
    end_log_process();
    return 0;
}