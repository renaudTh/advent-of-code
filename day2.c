#include <stdio.h>
#include <stdlib.h>

int main()
{

    FILE *file = fopen("input-d2.txt", "r");
    if (!file)
        return EXIT_FAILURE;

    int score[3][3] = {{4, 8, 3}, {1, 5, 9}, {7, 2, 6}};
    int choice[3][3] = {{2,0,1},{0,1,2},{1,2,0}};
    char c = 0;
    int elf;
    int you;
    int your_choice;
    int second_sum = 0;
    int sum = 0;
    char gap = 0;

    while ((c = fgetc(file)) != EOF)
    {
        if (c == ' ')
            continue;
        if (c == '\n')
        {
            sum += score[elf][you];
            second_sum += score[elf][your_choice];
            continue;
        }
        if (gap == 0)
        {
            elf = c - 'A';
            gap++;
        }
        else
        {
            you = c - 'X';
            your_choice = choice[elf][you];
            gap--;
        }
    }
    printf("your score for the first is %d\n", sum);
    printf("your score for the second is %d\n", second_sum);

    return 0;
}
