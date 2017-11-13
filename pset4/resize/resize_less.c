// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "bmp.h"


int check_args(int argc, char *argv[]);
bool isfloat(char *input);
int every_nth_pixel(char *f);


int main(int argc, char *argv[])
{

    if (check_args(argc, argv) == 1)
    {
        return 1;
    }


    // remember args
    float f = atof(argv[1]);
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
    printf("Width Original: %i\n", bi.biWidth);
    printf("Width Resized: %i\n", out_bi.biWidth);

    // resize biHeight according to input
    out_bi.biHeight = bi.biHeight * f;
    printf("Height Original: %i\n", bi.biHeight);
    printf("Height Resized: %i\n", out_bi.biHeight);

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int out_padding = (4 - (out_bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // resize biSizeImage according to input
    out_bi.biSizeImage = ((sizeof(RGBTRIPLE) * out_bi.biWidth) + out_padding) * abs(out_bi.biHeight);
    printf("BiSizeImage Original: %i\n", bi.biSizeImage);
    printf("BiSizeImage Resized: %i\n", out_bi.biSizeImage);

    // resize bf.bfsize
    out_bf.bfSize = out_bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    printf("bfSize Original: %i\n", bf.bfSize);
    printf("bfSize Resized: %i\n", out_bf.bfSize);


    // write outfile's BITMAPFILEHEADER
    fwrite(&out_bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&out_bi, sizeof(BITMAPINFOHEADER), 1, outptr);













    int resize = floor(atof(argv[1]));


    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {

        // repeat the current scanline j times depending on the resize value.
        for (int j = 0; j < resize; j++)
        {
            // track bytes to return input pointer to beginning of scanline;
            int offset = 0;


            // iterate over pixels in input scanline
            for (int k = 0; k < bi.biWidth; k++)
            {
                // buffer
                RGBTRIPLE triple;


                // read RGB triple from infile and track pointer movement
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                offset += (sizeof(RGBTRIPLE));


                // write current RGB triple to outfile k times.
                for (int l = 0; l < resize; l++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }

            }

            // skip over padding in input file, if any
            fseek(inptr, padding, SEEK_CUR);
            offset += (padding * sizeof(char));

            // add necessary padding to output file
            for (int m = 0; m < out_padding; m++)
            {
                char pad = 0x00;
                fputc(pad, outptr);

            }

            // move pointer to beginning of input only if resize value has not yet been met.
            if (j < resize - 1){
                // send pointer back to beginning of scanline
                fseek(inptr, (-1 * offset), SEEK_CUR);
            }
        }
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

int every_nth_pixel (char *f)
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


int check_args(int argc, char *argv[])
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

    return 0;
}