#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int getRandom(int min, int max) {
    // srand(time(0));
    return min + rand() % (max - min + 1);
}