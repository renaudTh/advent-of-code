#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_SIZE 1 << 13
#define PREFIX_LENGTH 4
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
    uint8_t map[26];
    memset(map, 0, 26);

    while((c = fgetc(file)) != EOF){
        str[i] = c;
        i++;
    }
    
    i = 0;
    j = i;
    int xj = 0;
    int xi = 0;
    int result = 0;

    while(j < strlen(str)){
        
        if(j - i >= PREFIX_LENGTH){
            if(all_distinct(map, 26)){
                result = j + 1;
                break;
            }
            xi = get_index(str[i]);
            map[xi]--;
            i++;
            xi = get_index(str[i]);
            map[xi]++;
        }
        xj = get_index(str[j]);
        map[xj]++;
        j++;

    }
    printf("result %d\n", result);

    return 0;
}