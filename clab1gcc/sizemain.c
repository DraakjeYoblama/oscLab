#include <stdio.h>
#include "sizemain.h"

int main()
{
    printf("%lu\n", sizeof(int));
    printf("%lu\n", sizeof(float));
    printf("%lu\n", sizeof(double));
    printf("%lu\n", sizeof(void));
    printf("%lu\n", sizeof(int*));
    return 0;
}
