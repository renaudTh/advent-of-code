#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAXSIZE 1000000

typedef unsigned char uint8_t;

typedef struct Coordinates_t{
    int x;
    int y;
} Coordinates;

void left(Coordinates *c){
    c->x--;
}
void right(Coordinates *c){
    c->x++;
}
void top(Coordinates *c){
    c->y++;
}
void down(Coordinates *c){
    c->y--;
}
int distance(const Coordinates *c1, const Coordinates *c2){

    return (c2->x-c1->x)*(c2->x-c1->x)+(c2->y-c1->y)*(c2->y-c1->y);
}

void move(char direction, Coordinates *head, Coordinates *tail){

    Coordinates new_head = {head->x, head->y};
    switch (direction)
    {
    case 'R':
        right(&new_head);
        break;
    case 'L':
        left(&new_head);
        break;
    case 'U':
        top(&new_head);
        break;
    case 'D':
        down(&new_head);
        break;
    default:
        printf("Unknown direction\n");
        return;
        break;
    }
    if(distance(&new_head, tail) > 2){
        tail->x = head->x;
        tail->y = head->y;
    }
    head->x = new_head.x;
    head->y = new_head.y;
}

int is_digit(char c){
    return c >= '0' && c<='9';
}
int is_direction(char c){
    return c >= 'A' && c<='Z';
}

int get_hash(const Coordinates * c){
    return  (c->x +c->y) * (c->x + c->y + 1) / 2 + c->y;
}
int main(){

    Coordinates head = {0,0};
    Coordinates tail = {0,0};
    
    uint8_t * visited = malloc(sizeof(uint8_t)*MAXSIZE);
    memset(visited, 0, MAXSIZE);

    FILE *file = fopen("input-d9.txt", "r");
    if (!file)
        return EXIT_FAILURE;

    char c = 0;
    int number = 0;
    char direction = 0;
    int result = 0;
    while((c = fgetc(file)) != EOF){
        if(is_direction(c)){
            direction = c;
        }
        else if(is_digit(c)){
            number *= 10;
            number += c - '0';
        }
        else if(c == '\n'){
            for(int i = 0; i <number; i++){
                move(direction, &head, &tail);
                if(!visited[get_hash(&tail)]){
                    visited[get_hash(&tail)] = 1;
                    result++;
                }
            }
            number = 0;
        }
    }
    printf("Result : %d\n", result);

    fclose(file);
    return 0;
}