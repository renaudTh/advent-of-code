#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 1 << 13
#define PREFIX_LENGTH 14
#define get_index(c) c - 'a';

typedef unsigned char uint8_t;

int all_distinct(uint8_t *map, int size){
    int i = 0;
    while(i < size && map[i] <= 1) i++;
    return (i>=size);
}
int main(){

    FILE *file = fopen("input-d6.txt", "r");
        if (!file)
            return EXIT_FAILURE;

    char c = 0;
    char str[MAX_SIZE];
    int i = 0;
    int j = 0;
    int size = 0;
    uint8_t map[26];
    memset(map, 0, 26);

    while((c = fgetc(file)) != EOF){
        str[size] = c;
        size++;
    }
    
    int xj = 0;
    int xi = 0;
    int result = 0;

    while(j < size){     
        xj = get_index(str[j]);
        map[xj]++;
        j++;
        if(j - i >= PREFIX_LENGTH){
            if(all_distinct(map, 26)){
                result = j;
                break;
            }
            xi = get_index(str[i]);
            map[xi]--;
            i++;
        }
    }
    printf("result %d\n", result);
    return 0;
}