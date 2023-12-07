#include <stdio.h>
//#include <stdint.h>
//#include <inttypes.h>
//#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "sbuffer.h"
#include "config.h"
#include "main.h"


int main() {
    sbuffer_t** myBuffer = malloc(sizeof(*myBuffer));
    sbuffer_init(myBuffer);
    pthread_t thread1, thread2;
    int iret1, iret2;

    sbuffer_free(myBuffer);

    //iret1 = pthread_create(&thread1, NULL, reader, myBuffer); //TODO: make mutithreading work
    //iret2 = pthread_create(&thread2, NULL, reader, NULL);

    // parent (writer thread)
    FILE *fp;

    // Open a file in read mode
    fp = fopen("sensor_data", "r");

    // Store the content of the file
    sensor_data_t sensor_data;

    while (!feof(fp)) {
        // Read the content and store it inside buffer
        fread(&sensor_data.id, 2, 1, fp);
        fread(&sensor_data.value, 8, 1, fp);
        fread(&sensor_data.ts, 8, 1, fp);

        // Print file content
        if (!feof(fp)) {
            sbuffer_insert(*myBuffer, &sensor_data);
            //printf("%d, %lf, %ld\n", sensor_data.id, sensor_data.value, sensor_data.ts);
        } else {
            sensor_data.id = 0;
            sbuffer_insert(*myBuffer, &sensor_data);
        }

        usleep(10000);
    }
    // Close the file
    sbuffer_free(myBuffer);
    free(myBuffer);
    fclose(fp);
    return 0;
}

void *reader(sbuffer_t **buffer) {
    // children (reader threads)
    FILE* csv;
    sensor_data_t received_data;
    csv = fopen("sensor_data_out.csv", "w");

    while (1) {
        sbuffer_remove(*buffer, &received_data); //TODO: make this not an infinite loop

        // get data from buffer
        // write data to sensor_data_out.csv
        fprintf(csv, "%d, %lf, %ld\n", received_data.id, received_data.value, received_data.ts);;

        usleep(25000);
    }

    fclose(csv);
    return NULL;
}
