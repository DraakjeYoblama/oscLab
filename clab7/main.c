#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "main.h"
#include "sbuffer.h"
#include "config.h"


int main() {
    sbuffer_t **buffer = NULL;
    pthread_t thread1, thread2;
    int iret1, iret2;
    //TODO: sbuffer_init(buffer);

    iret1 = pthread_create(&thread1, NULL, reader, NULL);
    //iret2 = pthread_create(&thread2, NULL, reader, NULL);

    // parent (writer thread)
    FILE *fp;

    // Open a file in read mode
    fp = fopen("sensor_data", "r");

    // Store the content of the file
    uint16_t sensor_id;
    double sensor_temperature;
    time_t starttime;

    while (!feof(fp)) {
        // Read the content and store it inside buffer
        fread(&sensor_id, 2, 1, fp);
        fread(&sensor_temperature, 8, 1, fp);
        fread(&starttime, 8, 1, fp);

        // Print file content
        if (!feof(fp)) {
            printf("%d, %lf, %ld\n", sensor_id, sensor_temperature, starttime);
        } else {
            printf("%d", 0);
        }

        usleep(10000);
    }
    // Close the file
    fclose(fp);
    return 0;
}

void *reader() {
    // children (reader threads)
    FILE *csv;
    csv = fopen("sensor_data_out.csv", "w");

    //TODO: sbuffer_free(sbuffer_t **buffer);

    // get data from buffer
    // write data to sensor_data_out.csv
    fprintf(csv, "Hello");


    usleep(25000);
    fclose(csv);
    return NULL;
}
