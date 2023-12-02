#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>


int main() {
    FILE *fp;
    int i;

    // Open a file in read mode
    fp = fopen("sensor_data", "r");

    // Store the content of the file
    uint16_t sensor_id;
    double sensor_temperature;
    time_t starttime;

    for (i = 0; i<7; i++) {
        // Read the content and store it inside buffer
        fread(&sensor_id, 2, 1, fp);
        fread(&sensor_temperature, 8, 1, fp);
        fread(&starttime, 8, 1, fp);

        // Print file content
        printf("%d %lf %ld\n", sensor_id, sensor_temperature, starttime);
    }



    // Close the file
    fclose(fp);

    return 0;
}


// none of this is right