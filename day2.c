#include <stdio.h>
#include <stdlib.h>

int main(){

    FILE * file = fopen("input-d2.txt", "r");
    if(!file) return EXIT_FAILURE;
    
    int score[3][3] = {{4,8,3}, {1,5,9}, {7,2,6}};
    char c = 0;
    int first;
    int second;
    int sum = 0;
    char gap=0;
    while((c = fgetc(file)) != EOF){
        if(c == ' ') continue;           
        if(c == '\n'){
            sum+= score[first][second];
            continue;
        }    
        if(gap == 0){
            first = c - 'A';
            gap++;
        }
        else{
            second = c - 'X';
            gap--;
        }
    }
    printf("your score is %d\n", sum);
  
    return 0;
}


