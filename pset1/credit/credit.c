#include <cs50.h>
#include <stdio.h>

bool isValid (long, int);
int summarize (long, int);
int sumDigits (int);
int ccLength (long);
int firstTwoDigits (long, int);

int main (void)
{
    long cc = get_long_long ("Credit Card Number: ");
    int l = ccLength (cc);

    //first and second digits of card number
    int nn = firstTwoDigits (cc, l);
    int n = nn / 10;

    //print card type if the number meets the specs
    if (isValid (cc, l))
    {
        if (n == 4 && (l == 13 || l == 16))
        {
            printf("VISA\n");
        }
        else if (l == 15 && (nn == 34 || nn == 37))
        {
            printf("AMEX\n");
        }
        else if ((nn > 50 && nn < 56) && l == 16)
        {
            printf("MASTERCARD\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }

    return 0;
}

bool isValid (long cc, int l)
{
    //returns true if summarized number has a zero as its last digit.
    int value = summarize(cc, l);
    return value % 10 == 0;
}

int summarize (long cc, int l)
{
    int sum1 = 0;
    int sum2 = 0;

    for (int i = 1; i <= l; i++)
    {
        //as long as we still have digits to process
        if (cc > 0)
        {
            if (i % 2 != 0)
            {
                //get last digit and add to odd sum
                sum1 += cc % 10;
                //throw it away
                cc = cc / 10;
            }
            else
            {
                //get last digit and double it.
                int a = sumDigits ((cc % 10) * 2);
                //add to even sum
                sum2 += a;
                //throw it away.
                cc = cc / 10;
            }
        }
    }

    //add odd sum and even sum
    return sum1 + sum2;
}

int ccLength (long cc)
{
    //count the number of digits in the credit card number
    int digits = 0;
    do
    {
        cc /= 10;
        digits++;
    }
    while (cc > 0);

    return digits;
}

int sumDigits (int n)
{
    //break an integer into individual digits and adds them.
    int sum = 0;
    while (n > 0)
    {
        int digit = n % 10;
        sum += digit;
        n /= 10;
    }

    return sum;
}

int firstTwoDigits (long cc, int l)
{
    //drop all the digits from the card number except the first two
    for (int i = 0; i < l - 2; i++)
    {
        cc /= 10;
    }

    return cc;
}