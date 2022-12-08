#include <stdio.h>
#include <stdlib.h>
#define SIZE 99

typedef unsigned char uint8_t;

int is_edge(int i, int j){

    return (i == 0) || (j == 0) || (i == SIZE -1) || (j == SIZE - 1);
}
int is_visible(uint8_t matrix[][SIZE], int i, int j){

    int visible = 1;
    for(int l = 0; l < SIZE; l++){ 
        if(l != i){
            visible = visible && (matrix[l][j] < matrix[i][j]);
        }
        else{
            if(visible) return 1;
            visible = 1;
            continue;
        }
    }
    if(visible) return 1;
    visible = 1;
    for(int c = 0; c < SIZE; c++){
        if(c != j){
            visible = visible && (matrix[i][c] < matrix[i][j]); 
        }
        else{
            if(visible) return 1;
            visible = 1;
            continue;
        }
    }
    return visible;
}
int compute_score(uint8_t matrix[][SIZE], int i, int j){

    int score[4];
    if(is_edge(i,j)) return 0;
    for(int i = 0; i < 4; i++){
        score[i] = 0;
    }
    for(int n = j-1; n >= 0; n--){
        if(matrix[i][n] < matrix[i][j]){
            score[0]++;
            continue;
        }
        else{
            score[0]++;
            break;
        }
    }
    for(int s = j+1; s < SIZE; s++){
        if(matrix[i][s] < matrix[i][j]){
            score[1]++;
            continue;
        }
        else{
            score[1]++;
            break;
        }
    }
    for(int e = i+1; e < SIZE; e++){
        if(matrix[e][j] < matrix[i][j]){
            score[2]++;
            continue;
        }
        else{
            score[2]++;
            break;
        }
    }
    for(int w = i-1; w >= 0; w--){
        if(matrix[w][j] < matrix[i][j]){
            score[3]++;
            continue;
        }
        else{
            score[3]++;
            break;
        }
    }
    return score[0]*score[1]*score[2]*score[3];
}


int main()
{


    FILE *file = fopen("input-d8.txt", "r");
    if (!file)
        return EXIT_FAILURE;
    
    char c = 0;
    uint8_t matrix[SIZE][SIZE];
    int row = 0, col = 0;

    while ((c = fgetc(file)) != EOF)
    {
        if(c != '\n'){
            matrix[col][row] = c - '0';
            col++;
        }
        else{
            col = 0;
            row++;
        }
    }
    int visible = 0;
    int highest = 0;
    int score = 0;
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){

            if(is_edge(i,j)){
                visible++;
                continue;
            } 
            score = compute_score(matrix, i, j);
            highest = (score > highest) ? score : highest;
            visible += is_visible(matrix, i,j);
        }
    }
    printf("number of visible : %d\n", visible);
    printf("The highest score is : %d\n", highest);

    return 0;
}
