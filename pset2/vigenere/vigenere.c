#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int errorMessage(int);
bool validArgCount(int);
bool validKeyword(string);

int main(int argc, string argv[])
{
    //check arg count
    if (!validArgCount(argc))
    {
        return errorMessage(1);
    }

    //check keyword validity
    if (!validKeyword(argv[1]))
    {
        return errorMessage(2);
    }

    //set string validated input to keyword
    string keyword = argv[1];

    //set initial key index and keylength
    int j = 0;
    int keylength = strlen(keyword);

    //get some plaintext to encrypt
    string plaintext = get_string("plaintext: ");

    //begin output
    printf("ciphertext: ");

    //iterate through plaintext
    for (int i = 0, inputLength = strlen(plaintext); i < inputLength; i++)
    {

        //get key from keyword and translate to alphaindex
        int key = toupper(keyword[j]);
        int k = key - 65;

        //get character to be encrypted, ready variable to hold new value
        char p = plaintext[i];
        char c;

        //operate only on lower or uppercase letters
        if (isalpha(p))
        {

            if (islower(p))
            {
                //get alpha index of lower case then add to ascii value 'A'
                int alphaIndex = (p + k - 97) % 26;
                c = alphaIndex + 97;
            }
            else
            {
                //get alpha index of upper case then add to ascii value 'a'
                int alphaIndex = (p + k - 65) % 26;
                c = alphaIndex + 65;
            }
        }
        else
        {
            //for non-alpha characters that don't change
            c = p;
        }

        //output encrypted character
        printf("%c", c);

        //cycle to next keyword character
        if (j < keylength - 1)
        {
            j++;
        }
        else
        {
            j = 0;
        }
    }

    //line break at end of output
    printf("\n");

    return 0;
}

//validate correct argument count
bool validArgCount(int argc)
{
    if (argc != 2)
    {
        errorMessage(1);
        return false;
    }

    return true;
}

//validate keyword
bool validKeyword(string keyword)
{
    //make sure the arg contains only letters
    int charcount = strlen(keyword);

    for (int i = 0; i < charcount; i++)
    {
        if (!isalpha(keyword[i]))
        {
            errorMessage(2);
            return false;
        }
    }

    return true;
}


//return error message depending on input code
int errorMessage(int code)
{
    if (code == 1)
    {
        printf("Incorrect argument count. Must provide a single keyword.\n");
    }
    else
    {
        printf("Invalid keyword. Must provide an alpha based keyword.\n");
    }

    return 1;
}