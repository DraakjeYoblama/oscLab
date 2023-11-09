#include <stdio.h>
#include <string.h>
#include "stringsmain.h"

char first[] = "Firstname";
char second[] = "Lastname";
int MAX = 20;


int main() {
    char name[MAX];
    char str[MAX];

    printf("Type name\n");
    scanf("%s", first);
    scanf("%s", second);


    printf("%s %s\n", first, convertUpper(second));
    //printf("%s %s\n", first, str);
    //printf("%i", strcmp(second, str));

    return 0;
}

const char* convertUpper(char input[]) {
    char output[sizeof(input)-1];
    for (int i=0; i<sizeof(input); i++) {
        if (input[i] >= 'a' && input[i] <= 'z') {
            output[i] = input[i] -32;
        } else {
            output[i] = input[i];
        }
    }
    return output;
}


