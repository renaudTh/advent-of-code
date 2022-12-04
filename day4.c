#include <stdio.h>
#include <stdlib.h>

int fully_overlap(int * n){

    return (n[0] >= n[2] && n[1] <= n[3]) || (n[2] >= n[0] && n[3] <= n[1]);
}
int overlap(int *n){
    return ((n[3] >= n[0]) && (n[3] <= n[1])) || ((n[2] >= n[0]) && (n[2] <= n[1]));
}

int main()
{

    FILE *file = fopen("input-d4.txt", "r");
    if (!file)
        return EXIT_FAILURE;

    char c = 0;
    int numbers[4] = {0};
    int i = 0;
    int full_sum = 0;
    int sum = 0;

    while ((c = fgetc(file)) != EOF)
    {
        if (c == '-' || c == ',')
            i++;
        else if (c == '\n')
        {          
            sum += overlap(numbers) || fully_overlap(numbers);
            full_sum+= fully_overlap(numbers);
            i = 0;
            for (int i = 0; i < 4; i++)
                numbers[i] = 0;
        }
        else
        {
            numbers[i] *= 10;
            numbers[i] += c - '0';
        }
    }
    printf("Number of fully overlapping : %d\n", full_sum);
    printf("Number of overlapping : %d\n", sum);

    return 0;
}
