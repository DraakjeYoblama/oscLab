#include "sensor_db.h"

//TODO: use better log messages

FILE * open_db(char * filename, bool append) {
    FILE* fp;

    if (!append) {
        fp = fopen(filename, "w+"); // replace file
        write_to_log_process("create new csv file");
    } else {
        fp = fopen(filename, "a+"); //append to existing file
        // this also creates a new file if none exists, not sure if that needs to change for loggin purposes
        write_to_log_process("append to existing csv file or create new csv file");
    }

    return fp;
}

int insert_sensor(FILE * f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts) {
    fprintf(f, "%d, %lf, %ld\n", id, value, ts);
    write_to_log_process("sensor value added");
    return 0;
}

int close_db(FILE * f){
    fclose(f);
    write_to_log_process("csv file closed");
    return 0;
}