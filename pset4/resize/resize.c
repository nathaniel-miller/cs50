// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "bmp.h"

int check_args(int argc, char *argv[]);
int extras(int in, int out, float resize);
bool is_float_or_int(char *input);

int main (int argc, char *argv[])
{
    // check input argument validity.
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

    // setup line tracker to keep track of which lines are set to recieve an extra line write
    int next_extra_line = 0;

    if (abs(out_bi.biHeight) > 0 && out_bi.biWidth > 0)
    {

        // iterate over infile's scanlines
        for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
        {

            // setup variables for calculating if some lines and pixels need extra pixels
            int base_resize = floor(f);
            int extra_pixels = extras(bi.biWidth, out_bi.biWidth, f);
            int extra_lines = extras(abs(bi.biHeight), abs(out_bi.biHeight), f);
            int add_extra_pixel_every_n;
            int add_extra_line_every_n;


            // calculate the gap between pixels / lines that require an extra write.
            if (extra_pixels > 0)
            {
                add_extra_pixel_every_n = floor(bi.biWidth / extra_pixels);
            }

            if (extra_lines > 0)
            {
                add_extra_line_every_n = floor(biHeight / extra_lines);
            }

            // buffer for resized line
            RGBTRIPLE resized_line[out_bi.biWidth];
            int buffer_index = 0;

            //determine if current line should recieve an extra write to output
            bool needs_extra_line = false;

            if (extra_lines > 0)
            {
                if (i == next_extra_line)
                {
                    needs_extra_line = true;
                    next_extra_line += add_extra_line_every_n;
                    extra_lines--;
                }
            }

            // setup tracker for pixels that get are set to get an extra pixel write
            int next_extra_pixel = 0;

            // iterate over pixels in input scanline
            for (int k = 0; k < bi.biWidth; k++)
            {
                //determine if current pixel should recieve an extra write to output
                bool needs_extra_pixel = false;

                if (extra_pixels > 0)
                {
                    if (k == next_extra_pixel)
                    {
                        needs_extra_pixel = true;
                        next_extra_pixel += add_extra_pixel_every_n;
                        extra_pixels--;
                    }

                }

                // pixel buffer
                RGBTRIPLE triple;

                // calculate how much to resize current pixel - base value or base + extra
                int output_pixels = base_resize;
                if (needs_extra_pixel)
                {
                    output_pixels++;
                }

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                for (int l = 0; l < output_pixels; l++)
                {
                    // store expanded pixels into line buffer
                    resized_line[buffer_index] = triple;
                    buffer_index++;
                }

            }

            // calculate how many lines to write - base value or base + extra;
            int output_lines = base_resize;
            if (needs_extra_line)
            {
                output_lines++;
            }

            // write line buffer to output file m times
            for (int m = 0; m < output_lines; m++)
            {
                fwrite(resized_line, sizeof(resized_line), 1, outptr);

                // add necessary padding to output file
                for (int n = 0; n < out_padding; n++)
                {
                    char pad = 0x00;
                    fputc(pad, outptr);
                }
            }

            // skip over padding in input file, if any
            fseek(inptr, padding, SEEK_CUR);
        }

    }


    return 0;
}

int extras(int inputWidth, int outputWidth, float resizeValue)
{
    return outputWidth - (floor(resizeValue) * inputWidth);
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
    if ( !is_float_or_int(argv[1]))
    {
        fprintf(stderr, "%s is not a valid input. Please enter a float between 0.0 and 100.0.\n", argv[1]);
        return 1;
    }

    // ensure first argument is within range of 0.0 - 100.0
    float f = atof(argv[1]);
    if (f > 100.0 || f == 0)
    {
        fprintf(stderr, "%f is out of bounds. Range must be greater than 0.0 and not more than 100.0.\n", f);
        return 1;
    }

    return 0;
}

bool is_float_or_int(char *input)
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

    //remaining conditions to be considered a valid float or integer.
    bool is_a_float = (has_dot && ends_with_dig) ? true : false;
    bool is_an_integer = (!has_dot && ends_with_dig) ? true : false;

    if (is_a_float || is_an_integer)
    {
        return true;
    }

    return false;

}