// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "bmp.h"

bool isfloat(char *);
int every_nth_pixel (char *);

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "error - Usage: ./resize float(0.0, 100.0) infile outfile\n");
        return 1;
    }

    // ensure first agrument is a float
    if ( !isfloat(argv[1]))
    {
        fprintf(stderr, "%s is not a valid input. Please enter a float between 0.0 and 100.0.\n", argv[1]);
        return 1;
    }

    // ensure first argument is within range of 0.0 - 100.0
    float f = atof(argv[1]);
    if (f > 100.0)
    {
        fprintf(stderr, "%f is too large. Range must be from 0.0 to 100.0.\n", f);
        return 1;
    }


    // remember arguments
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }



    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // copy infile's headers for modification
    BITMAPFILEHEADER out_bf = bf;
    BITMAPINFOHEADER out_bi = bi;

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // resize biWidth according to input
    out_bi.biWidth = bi.biWidth * f;
    fprintf(stderr, "Width Original: %i\n", bi.biWidth);
    fprintf(stderr, "Width Resized: %i\n", out_bi.biWidth);

    // resize biHeight according to input
    out_bi.biHeight = bi.biHeight * f;
    fprintf(stderr, "Height Original: %i\n", bi.biHeight);
    fprintf(stderr, "Height Resized: %i\n", out_bi.biHeight);

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int out_padding = (4 - (out_bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // resize biSizeImage according to input
    out_bi.biSizeImage = ((sizeof(RGBTRIPLE) * out_bi.biWidth) + out_padding) * abs(out_bi.biHeight);
    fprintf(stderr, "BiSizeImage Original: %i\n", bi.biSizeImage);
    fprintf(stderr, "BiSizeImage Resized: %i\n", out_bi.biSizeImage);

    // resize bf.bfsize
    out_bf.bfSize = out_bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    fprintf(stderr, "bfSize Original: %i\n", bf.bfSize);
    fprintf(stderr, "bfSize Resized: %i\n", out_bf.bfSize);


    // write outfile's BITMAPFILEHEADER
    fwrite(&out_bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&out_bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // every_nth_pixel to get an 'extra' pixel due to decimals
    int nth = every_nth_pixel(argv[1]);
    int counter_til_extra_pixel = 0;
    int counter_til_extra_line = 0;
    int resize = floor(atof(argv[1]));
    int resize_pixels = resize;
    int resize_lines = resize;


    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // add one to pixel resize count if current pixel falls an 'extra pixel mode'.
        // then, reset counter. Else decrement counter.
        if (counter_til_extra_line == 0 && nth != 0){
            resize_lines += 1;
            counter_til_extra_line = nth - 1;
        }
        else
        {
            counter_til_extra_line--;
        }

        for (int l = 0; l < resize_lines; l++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);


                // add one to pixel resize count if current pixel falls an 'extra pixel mode'.
                // then, reset counter. Else decrement counter.
                if (counter_til_extra_pixel == 0 && nth != 0)
                {
                    resize_pixels += 1;
                    counter_til_extra_pixel = nth - 1;
                }
                else
                {
                    counter_til_extra_pixel--;
                }

                // write current RGB triple to outfile n times.
                for (int k = 0; k < resize_pixels; k++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }


            }

            // skip over padding, if any
            fseek(inptr, padding, SEEK_CUR);

            // then add it back (to demonstrate how)
            for (int m = 0; m < padding; m++)
            {
                fputc(0x00, outptr);
            }
        }



        // write the same generated scanline to outfile n times

    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}

bool isfloat(char *input)
{

    //check characters to make sure they are either digits or (one) dot.
    bool has_dot = false;
    bool ends_with_dig = false;

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
                ends_with_dig = false;
            }
        }
        else
        {
          ends_with_dig = true;
        }
    }

    //remaining conditions to be considered a valid float.
    bool is_a_float = (has_dot && ends_with_dig) ? true : false;

    return is_a_float;

}

int every_nth_pixel (char * f)
{
    int counter = 0;
    bool dot = false;
    char *decimal = malloc(80);

    //count number of decimal points and extract everything after the decimal point.
    for (int i = 0; i < strlen(f); i++)
    {
        if (dot)
        {
            decimal[counter] = f[i];
            counter++;
        }

        if (f[i] == '.')
        {
            dot = true;
        }
    }

    decimal[counter + 1] = '\0';

    //convert extracted decimal points into an actual float. Free temp storage.
    float decimal_float = atof(decimal);
    free(decimal);

    if (decimal_float == 0)
    {
        return 0;
    }

    float ff = 1.0 / decimal_float;

    for (int j = 0; j < counter; j++)
    {
        ff *= 10.0;
    }


    return round(ff);
}