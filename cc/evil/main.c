#include <stdio.h>

#define true ((__LINE__ % 2) == 0)

int main(int argc, char **argv) {
    printf("line 1: %d\n", true);
    printf("line 1: %d\n", true);
    
    int b = true;

    printf("line 1: %d\n", b == true);
    printf("line 1: %d\n", b == true);

    return true;
}
