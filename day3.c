#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char uint8_t;

typedef struct _LetterMap {

    uint8_t * map;

} LetterMap;


int get_index(char c)
{

    if (c >= 'a' && c <= 'z')
        return c - 'a';
    else if (c >= 'A' && c <= 'Z')
        return 26 + (c - 'A');
    else
        return -1;
}
LetterMap * letter_map_new(uint8_t init){

    LetterMap * m = malloc(sizeof(LetterMap));
    m->map = malloc(sizeof(unsigned char)*52);
    memset(m->map, init, 52);
    return m;
}
void letter_map_destroy(LetterMap *m){
    if(m->map) free(m->map);
    free(m);
}
void letter_map_from_string(LetterMap * m, const char * str){
    
    for(int i = 0; i < strlen(str); i++){
        int xi = get_index(str[i]);
        if (xi != -1)
        {
            m->map[xi] |= 1;
        }
    }
}
void letter_map_add(LetterMap *result, const LetterMap *toAdd){
    for(int i = 0; i < 52; i++){
        result->map[i] &= toAdd->map[i];
    }
}
void letter_map_reset(LetterMap *m, uint8_t init){
    memset(m->map, init, 52);
}
int get_common_char(const char *str)
{

    int map[52] = {0};
    int len = strlen(str);
    int i = 0;
    int j = len - 2;

    while (i < j)
    {

        int xi = get_index(str[i]);
        int xj = get_index(str[j]);

        if (map[xi] == 0)
        {
            map[xi] = 1;
        }
        else if (map[xi] == 2)
        {
            return xi + 1;
        }
        if (map[xj] == 0)
        {
            map[xj] = 2;
        }
        else if (map[xj] == 1)
        {
            return xj + 1;
        }
        i++;
        j--;
    }
    return 0;
}
int get_badge_score(LetterMap * m)
{      
    int i = 0;
    while (i < 52 && !m->map[i]){
        i++;
    }
    return (i < 52) ? (i+1) : 0;
}
int main()
{

    FILE *file = fopen("input-d3.txt", "r");
    if (!file)
        return EXIT_FAILURE;

    char str[100];
    int sum = 0;
    
    LetterMap * resultMap = letter_map_new(1);
    LetterMap * current = letter_map_new(0);
    int count = 1;
    int badge_sum = 0;

    while (fgets(str, 100, file) != NULL)
    {

        letter_map_from_string(current, str);
        letter_map_add(resultMap, current);
        if (count % 3 == 0)
        {
            badge_sum += get_badge_score(resultMap);
            letter_map_reset(resultMap, 1);
        }

        sum += get_common_char(str);
        count++;
        letter_map_reset(current, 0);
    }

    printf("the sum is %d\n", sum);
    printf("the badge sum is %d\n", badge_sum);
    letter_map_destroy(current);
    letter_map_destroy(resultMap);
    return 0;
}
