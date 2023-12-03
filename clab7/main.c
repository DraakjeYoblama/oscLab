#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "main.h"


int main() {
    pthread_t thread1, thread2;
    int iret1, iret2;

    iret1 = pthread_create(&thread1, NULL, reader, NULL);
    iret1 = pthread_create(&thread1, NULL, reader, NULL);

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
            printf("%d %lf %ld\n", sensor_id, sensor_temperature, starttime);
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
    printf("I'm a child--------------------------------------\n");
    return NULL;
}
