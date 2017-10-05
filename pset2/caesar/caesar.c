#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int errorMessage(int);

int main(int argc, string argv[])
{
    //check for the proper number of arguments
    if (argc != 2)
    {
        return errorMessage(1);
    }

    //make sure the arg contains only digits
    int charcount = strlen(argv[1]);

    for (int i = 0; i < charcount; i++)
    {
        if (isalpha(argv[1][i]))
        {
            return errorMessage(2);
        }
    }

    //set string (that contains only digits) to int key
    int key = atoi(argv[1]);

    //make sure key is non-negative
    if (key < 0)
    {
        return errorMessage(2);
    }

    //get some plaintext to encrypt
    string plaintext = get_string("plaintext: ");

    printf("ciphertext: ");

    //iterate through input
    for (int i = 0, inputLength = strlen(plaintext); i < inputLength; i++)
    {
        char p = plaintext[i];
        char c;

        //operate only on lower or uppercase letters
        if (isalpha(p))
        {
            if (islower(p))
            {
                //get alpha index of lower case then add to ascii value 'A'
                int alphaIndex = (((int) p - 97) + key) % 26;
                c = alphaIndex + 97;
            }
            else
            {
                //get alpha index of upper case then add to ascii value 'a'
                int alphaIndex = (((int) p - 65) + key) % 26;
                c = alphaIndex + 65;
            }
        }
        else
        {
            c = p;
        }

        printf("%c", c);
    }

    printf("\n");

    return 0;
}

//return error message depending on input code
int errorMessage(int code)
{
    if (code == 1)
    {
        printf("Incorrect argument count. Must provide a single, non-negative integer.\n");
    }
    else
    {
        printf("Incorrect argument. Must provide a non-negative integer.\n");
    }

    return 1;
}