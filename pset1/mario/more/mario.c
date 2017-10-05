#include <cs50.h>
#include <stdio.h>
void printBlocks(int n);

int main(void)
{
    int input;

    do
    {
        input = get_int("Integer from 0 to 23: ");
    }
    while (input < 0 || input > 23);

    for (int i = 0; i < input; i++)
    {
        //print leading spaces
        for (int space = 1; space < input - i; space++)
        {
            printf(" ");
        }

        //print blocks
        printBlocks(i + 1);

        //print gap
        printf("  ");

        //print blocks again
        printBlocks(i + 1);

        //print newline
        printf("\n");
    }
}

void printBlocks(n)
{
    for (int block = 0; block < n; block++)
    {
        printf("#");
    }
}