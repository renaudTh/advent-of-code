#include <stdio.h>
#include <stdlib.h>

void insert(int sum, int * table, int size){
    int i = 0;
    while(i < size && (sum <= table[i])) i++;
    if(i >= size) return;
    for(int j = size - 1; j > i; j--){
        table[j] = table[j-1];
    }
    table[i] = sum;
}

int sum(const int * table, int size){
    int sum = 0;
    for(int i = 0; i < size; i++){
        sum+=table[i];
    }
    return sum;
}

int main(){

    FILE * file = fopen("input.txt", "r");
    if(!file) return EXIT_FAILURE;
    
    int table[3] = {0};
    char c = 0;
    int number = 0;
    int current = 0;
    char gap = 0;
    
    while((c = fgetc(file)) != EOF){
     
        if(c != '\n')
        {
            number*=10;
            number+= (c-'0');
            gap = 0;
            continue;
        }

        gap++;
        if(gap == 1)
        {
            current+=number;
            number = 0;
        }
        else 
        {
            insert(current, table, 3);
            current = 0;
        }  
    }
    printf("%d\n", sum(table,3));
    return 0;
}


