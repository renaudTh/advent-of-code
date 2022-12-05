#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100
#define NUMBER_OF_PLATEFORM 9
#define INDEX(p) (p - 1) / 4

typedef unsigned short uint16_t;

typedef struct Queue_t
{

    uint16_t length;
    char *table;

} Queue;

Queue *queue_new()
{
    Queue *q = malloc(sizeof(Queue));
    q->length = 0;
    q->table = malloc(sizeof(char) * MAX_SIZE);
    return q;
}

void queue_destroy(Queue *q)
{
    if (q->table)
        free(q->table);
    free(q);
}
int is_empty(const Queue *q)
{
    return q->length == 0;
}
void queue_push_back(Queue *q, char elt)
{
    if (q->length + 1 >= MAX_SIZE)
        return;
    q->length++;
    q->table[q->length - 1] = elt;
}
void queue_push_front(Queue *q, char elt)
{
    if (q->length + 1 >= MAX_SIZE)
        return;
    q->length++;
    for (int i = q->length - 2; i >= 0; i--)
    {
        q->table[i + 1] = q->table[i];
    }
    q->table[0] = elt;
}
char queue_pop(Queue *q)
{
    if (is_empty(q))
        return -1;
    char ret = q->table[q->length - 1];
    q->length--;
    return ret;
}
void queue_reverse(Queue *q)
{
    int i = 0;
    int j = q->length - 1;
    char temp = 0;
    while (i < j)
    {
        temp = q->table[i];
        q->table[i] = q->table[j];
        q->table[j] = temp;
        i++;
        j--;
    }
}
void queue_print(Queue *q)
{
    for (int i = 0; i < q->length; i++)
    {
        printf("%c ", q->table[i]);
    }
    printf("\n");
}
Queue *queue_sub(Queue *q, uint16_t n)
{
    if (n > q->length)
        return NULL;
    Queue *new_q = queue_new();
    for (int i = 0; i < n; i++)
    {
        queue_push_front(new_q, queue_pop(q));
    }
    return new_q;
}
void queue_append(Queue *result, const Queue *other)
{
    if (result->length + other->length > MAX_SIZE)
        return;
    for (int i = 0; i < other->length; i++)
    {
        queue_push_back(result, other->table[i]);
    }
}
int is_capital(char c)
{
    return c >= 'A' && c <= 'Z';
}
int is_digit(char c)
{
    return c >= '0' && c <= '9';
}
void parse_initial_position(char c, int *p, int *gap)
{
    if (c == '\n')
    {
        *p = 0;
        *gap = *gap + 1;
    }
    else
    {
        *gap = 0;
        *p = *p + 1;
    }
}

void move_9000(Queue **plateform, int number, int from, int to)
{
    for (int i = 0; i < number; i++)
    {
        queue_push_back(plateform[to - 1], queue_pop(plateform[from - 1]));
    }
}
void move_9001(Queue **plateform, int number, int from, int to)
{
    Queue *temp = queue_sub(plateform[from-1], number);
    queue_append(plateform[to - 1], temp);
    queue_destroy(temp);
}

int main()
{
    FILE *file = fopen("input-d5.txt", "r");
    if (!file)
        return EXIT_FAILURE;
    char c = 0;
    int p = 0;
    int gap = 0;

    int index = 0;
    int digit = 0;
    int move[3] = {0};
    Queue *plateform[NUMBER_OF_PLATEFORM];

    for (int i = 0; i < NUMBER_OF_PLATEFORM; i++)
    {
        plateform[i] = queue_new();
    }
    while ((c = fgetc(file)) != EOF)
    {
        if (gap < 2)
        {
            if (is_capital(c))
            {
                queue_push_front(plateform[INDEX(p)], c);
            }
            parse_initial_position(c, &p, &gap);
        }
        else
        {

            if (is_digit(c))
            {
                move[index] *= 10;
                move[index] += c - '0';
                digit = 1;
            }
            else
            {
                if (digit)
                {
                    index++;
                    digit = 0;
                }
            }
            if (c == '\n')
            {
                index = 0;
                move_9000(plateform, move[0], move[1], move[2]);
                //move_9001(plateform, move[0], move[1], move[2]);

                for (int i = 0; i < 3; i++)
                {
                    move[i] = 0;
                }
            }
        }
    }

    for (int i = 0; i < NUMBER_OF_PLATEFORM; i++)
    {
        char c = queue_pop(plateform[i]);
        printf("%c", c);
        queue_destroy(plateform[i]);
    }
    printf("\n");
    return 0;
}