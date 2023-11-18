#include "sensor_db.h"

//TODO: check if this file doesn't cause any issues

FILE * open_db(char * filename, bool append) {
    FILE* fp;

    if (!append) {
        fp = fopen(filename, "w+"); // replace file
    } else {
        fp = fopen(filename, "a+"); //append to existing file
    }

    return fp;
}

int insert_sensor(FILE * f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts) {
    fprintf(f, "%d, %lf, %ld\n", id, value, ts);
    return 0;
}

int close_db(FILE * f){
    fclose(f);
    return 0;
}