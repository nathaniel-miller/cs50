#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

bool isfloat(char *);

int main(int argc, char **argv)
{



}

bool isfloat(char *input)
{

    //check characters to make sure they are either digits or (one) dot.
    bool has_dot = false;

    for (int i = 0; i < strlen(input); i++)
    {
        //if current character is not a digit ...
        if (!isdigit(input[i]))
        {
            //..., a dot, or a second dot, this is not a valid float.
            if (input[i] != '.' || has_dot)
            {
                return false;
            }
            else
            {
                has_dot = true;
            }
        }
    }

    return true;
}