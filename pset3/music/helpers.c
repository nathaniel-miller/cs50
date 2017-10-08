// Helper functions for music

#include <cs50.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include "helpers.h"

int semitones(string);
int octavesemitones(char);
int basesemitones(char);
int accidentalsemitones(string);

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    int x = fraction[0] - '0';
    int y = fraction[2] - '0';

    return (8 / y) * x;
}

// Calculates frequency (in Hz) of a note
int frequency(string note)
{
    double sts = semitones(note);
    double power = sts / 12.00;
    double x = pow(2, power);

    return round(x * 440);
}

// Determines whether a string represents a rest
bool is_rest(string s)
{
    if (s[0] == '\0' || s[0] == '\n')
    {
        return true;

    }
    else
    {
        return false;
    }
}

//calculate semitone distance from A440 for a given note
int semitones(string note)
{
    int length = strlen(note);
    int o = octavesemitones(note[length - 1]);
    int b = basesemitones(note[0]);
    int a = accidentalsemitones(note);

    return o + b + a;
}

//Add or subract 12 semitones depending on the octave
int octavesemitones(char octave)
{
    //convert octave difference into semitones
    int o = octave - '0';
    return (o - 4) * 12;
}

//Add or subract semitones based on relative distance from A to given note.
//C is the lowest, B is the highest.
int basesemitones(char note)
{
    //convert note difference (compared to A)
    //to semitones
    int basesemitones;
    switch (toupper(note))
    {
        case 'A':
            basesemitones = 0;
            break;
        case 'B':
            basesemitones = 2;
            break;
        case 'C':
            basesemitones = -9;
            break;
        case 'D':
            basesemitones = -7;
            break;
        case 'E':
            basesemitones = -5;
            break;
        case 'F':
            basesemitones = -4;
            break;
        case 'G':
            basesemitones = -2;
            break;
    }

    return basesemitones;
}

//Add 1 semitone for every '#' present. Subtract for every 'b' present.
int accidentalsemitones(string note)
{
    int length = strlen(note);
    int accidentalsemitones = 0;

    //evaluate for 1 or more accidentals.
    if (length > 2)
    {
        //look at each character between the base note and the octave.
        //Raise if '#', lower if 'b'
        for (int i = 1; i <= length - 2; i++)
        {
            if (note[i] == '#')
            {
                accidentalsemitones++;
            }
            else if (note[i] == 'b')
            {
                accidentalsemitones--;
            }
        }

    }

    return accidentalsemitones;
}