#include "my_bsq.h"

int my_size(char* str){
    int i = 0;
    while(str[i]){
        i++;
    }
    return i;
}

int size_map;

char* read_line(int fd, int size)
{
    char *temp = (char*)malloc(sizeof(char)*(size+1));
    read(fd, temp, size);
    return temp;
}

char* find_last(char* map){
    char* str = (char*)malloc(sizeof(char*)*my_size(map)-5);
    for(int i = 0; map[i]; i++){
        if(map[i] == '\n'){
            for(int j = i+1, ind = 0; map[j]; j++){
                str[ind++] = map[j];
            }
            break;
        }
    }
    free(map);
    return str;
}

char** fill_null(char* map)
{
    const int fd = open(map, O_RDONLY);
    struct stat fs;
    stat(map, &fs);
    char*  row = read_line(fd, fs.st_size);
    close(fd);
    row = find_last(row);
    char** matrix = (char**)malloc(size_map * sizeof(char*));
    matrix[0] = (char*)malloc(sizeof(char)*size_map+1);
    int ind = 0;
    int j = 0;
    for(int i = 0; row[i]; i++){
        if(row[i] == '\n'){
            matrix[ind][j] = '\0';
            ind++;
            matrix[ind] = (char*)malloc(sizeof(char)*size_map+1);
            i++;
            j = 0;
        }
        matrix[ind][j++] = row[i];
    }
    matrix[ind][j] = '\0';
    return matrix;
}

char* read_file(char*file) {
    int fd = open(file, O_RDONLY);
    struct stat fs;
    stat(file, &fs);
    char* map = read_line(fd, fs.st_size);
    close(fd);
    return map;
}

unsigned short** fill_to_binary(char* my_map, int size)  {
    unsigned short **map = malloc(sizeof(unsigned short *) * size);
    for(int l=0; l < size; l++){  // init short array
        map[l] = malloc(sizeof(unsigned short) * size+1);
    }
     int l = 0;
     int j=0;
     for(int i=0; my_map[i]!='\0'; i++){
         
         if (my_map[i] == '\n'){
            l++;
            j=0;
        }else{
            map[l][j++] = (my_map[i] == '.'); // fill array
        }
            
     }
    return map;
}

void fill_to_null(unsigned short **temp, int size){
    int y = 0;
    while( y<size){
        temp[0][y++] = 0;
    }
    y = 0;
    while(y<size){
        temp[y++][0] = 0;
    }
}

int find_minimal_num(unsigned short **buf, int x, int y) { // set minimum
    unsigned short min = buf[y][x-1];
    if (min > buf[y-1][x-1]){
        min = buf[y-1][x-1];
    }
    if (min > buf[y-1][x]){
        min = buf[y-1][x];
    }
    if(min != 0) {
        buf[y][x] = min+1;
        return (int)min;
    }
    else{
        buf[y][x] = 1;
    }
    return 0;
}

struct vars_info* check_map(unsigned short** matrix,  int size) {
    struct  vars_info* result = malloc(sizeof(struct vars_info));
    result->i = 0;
    result->j = 0;
    result->size = 0;
    unsigned short **temp = malloc(sizeof(unsigned short*) * (size));
    for(int i = 0; i < size + 1; i++){
        temp[i] = malloc(sizeof(unsigned short) * (size+1));
    }
    fill_to_null(temp, size);
        int len;
    for(int i=0; i < size; i++) {
        for(int j=0; j<size; j++) {
            if(matrix[i][j] != 0) {
                len = find_minimal_num(temp, j+1, i+1);
                if(len > result->size) {
                    result->i = j;
                    result->j = i;
                    result->size = len;
                }
            }
            else{
                temp[i+1][j+1] = matrix[i][j];
            }
        }
    }
    return result;
}

void print_map(char** map){
    for(int i = 0; i < size_map; i++){
        printf("%s\n", map[i]);
    }
}

int my_bsq(int ac, char** av){
    int fd;
    if (ac < 2) {
        printf("less argument\n");
        exit(1);
    } else if (!(fd = open(av[1], O_RDONLY))){
        printf("Map not found\n");
        exit(1);
    }
    close(fd);
    char* map = read_file(av[1]);
    fd = open(av[1], O_RDONLY);
    char* sz = read_line(fd, 5);
    size_map = atoi(sz);
    free(sz);
    close(fd);
    map = find_last(map);
    if(size_map == 1){
        printf("wrong map!\n");
        return 0;
    }

    unsigned short** full_map =  fill_to_binary(map, size_map);

    struct  vars_info* info = check_map(full_map, size_map);

    free(full_map);
    
    char** char_map = fill_null(av[1]);
    for(int i=0; i <= info->size; i++) {
        for(int j=0; j <= info->size; j++) {
            char_map[info->j-i][info->i-j] = 'x';
        }
    }
    print_map(char_map);
    return 0;
}