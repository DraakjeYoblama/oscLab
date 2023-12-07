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
    FILE *fp;
    // Open a file in read mode
    fp = fopen("sensor_data", "r");

    sensor_data_t sensor_data;

    while (!feof(fp)) {
        // Read the content and store it inside buffer
        fread(&sensor_data.id, 2, 1, fp);
        fread(&sensor_data.value, 8, 1, fp);
        fread(&sensor_data.ts, 8, 1, fp);


        // Insert data entry into buffer
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
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    fclose(csv);
    sbuffer_free(myBuffer);
    free(myBuffer);
    fclose(fp);
    return 0;
}

void *reader(void* buffer) {
    // children (reader threads)

    sensor_data_t received_data;

    while (1) {
        usleep(25000);
        // get data from buffer
        sbuffer_remove(buffer, &received_data);

        // write data to sensor_data_out.csv
        if (received_data.id!=0) {
            printf("%lu: %d, %lf, %ld\n",pthread_self(), received_data.id, received_data.value, received_data.ts);

            fprintf(csv, "%d, %lf, %ld\n", received_data.id, received_data.value, received_data.ts);
        } else {
            break;
        }
    }
    return NULL;
}
