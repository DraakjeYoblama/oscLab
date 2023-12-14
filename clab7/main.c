#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "sbuffer.h"
#include "config.h"
#include "main.h"


FILE* csv;


int main() {
    sbuffer_t** myBuffer = malloc(8);
    sbuffer_init(myBuffer);
    pthread_t thread1, thread2;

    csv = fopen("sensor_data_out.csv", "w");
    //start threads
    pthread_create(&thread1, NULL, reader, *myBuffer);
    pthread_create(&thread2, NULL, reader, *myBuffer);


    // parent (writer thread)
    FILE *fpsd;
    // Open a file in read mode
    fpsd = fopen("sensor_data", "r");

    sensor_data_t sensor_data; //defined in config.h

    while (!feof(fpsd)) {
        // Read the content as a sensor_data_t
        fread(&sensor_data.id, 2, 1, fpsd);
        fread(&sensor_data.value, 8, 1, fpsd);
        fread(&sensor_data.ts, 8, 1, fpsd);


        //replace id with 0 at end of file
        if (feof(fpsd)) {
            sensor_data.id = 0;
        }

        // Insert data entry into buffer
        sbuffer_insert(*myBuffer, &sensor_data);
        //printf("%d, %lf, %ld\n", sensor_data.id, sensor_data.value, sensor_data.ts);

        usleep(10000); //10 millisec
    }

    sbuffer_cond();
    sbuffer_cond();
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Close the file
    fclose(csv);
    sbuffer_free(myBuffer);
    free(myBuffer);
    fclose(fpsd);
    return 0;
}

void *reader(void* buffer) {
    // children (reader threads)

    sensor_data_t received_data;

    while (1) {
        usleep(25000); //25 millisec
        // get data from buffer
        if (sbuffer_remove(buffer, &received_data) == 0) {

            // write data to sensor_data_out.csv
            if (received_data.id != 0) {
                //printf("%lu: %d, %lf, %ld\n", pthread_self(), received_data.id, received_data.value, received_data.ts);

                fprintf(csv, "%d, %lf, %ld\n", received_data.id, received_data.value, received_data.ts);
            } else {
                break;
            }
        }
    }
    return NULL;
}
