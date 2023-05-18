#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdbool.h>

struct  vars_info {
    int i;
    int j;
    int size;
};

int my_bsq(int ac, char** av);