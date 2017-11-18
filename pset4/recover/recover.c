// Recover any forgotten JPEGS from a given forensic image

# include <stdio.h>
# include <stdbool.h>
# include "jpeg.h"

// declare helper functions
int check_args(int argc, char *argv[]);
bool signature_is_present(BYTE potential_sig[4], SIG jpeg_sig);
char * z_pad(int counter);

SIG set_jpeg_signature(void);

// main program
int main (int argc, char *argv[])
{
    // check input for validity
    if (check_args(argc, argv) == 1)
    {
        return 1;
    }

    // call function to set default jpeg signature
    SIG jpeg_sig = set_jpeg_signature();

    // open buffers and declare other necessary variables for later use
    BYTE FAT_block[64];
    BYTE first_byte[1];
    int signature_offset = -4;
    int counter = 0;
    bool write_file_open = false;

    // open forensic image
    FILE * inptr = fopen(argv[1], "r");
    // FILE * outptr;

    // loop through file searching for first '255'
    while (fread(first_byte, sizeof(BYTE), 1, inptr) == 1)
    {
        // check if byte is 255, the first byte of a jpeg signature
        if (*first_byte == jpeg_sig.byte_1)
        {
            // check for signature
            BYTE possible_sig[4];

            // back pointer up one byte to compensate for already discovered 255
            fseek(inptr, -1, SEEK_CUR);

            // read four bytes from file that could potentially be a signature.
            fread(possible_sig, sizeof(possible_sig), 1, inptr);

            if (signature_is_present(possible_sig, jpeg_sig))
            {
                // close previously open write file, if any.
                // if (write_file_open)
                // {
                //     fclose(outptr);
                //     write_file_open = false;
                // }


                printf("Signature Found. . .\n");

                //Increment counter for image signatures found.
                counter++;

                // // setup name for image file
                // char file_name[8] = {};
                // char * zero_padding = z_pad(counter);
                // snprintf(file_name, 8, "%s%i.jpg\n", zero_padding, counter);

                // move pointer back 4 bytes after checking what they contain
                fseek(inptr, signature_offset, SEEK_CUR);

                //read FAT block from forensic image into buffer
                fread(FAT_block, sizeof(FAT_block), 1, inptr);

                // // open new output file based on current signature
                // outptr = fopen(file_name, "w");
                write_file_open = true;

                // // write FAT block buffer to file
                // fwrite(FAT_block, sizeof(FAT_block), 1, outptr);

            }
            else // byte is 255 but not part of a signature
            {
                if (write_file_open)
                {
                    // back up one byte
                    // fseek(inptr, -1, SEEK_CUR);

                    // move pointer back 4 bytes after checking what they contain
                    fseek(inptr, signature_offset, SEEK_CUR);

                    //read FAT block from forensic image into buffer
                    fread(FAT_block, sizeof(FAT_block), 1, inptr);

                //     // write FAT block buffer to file
                //     fwrite(FAT_block, sizeof(FAT_block), 1, outptr);
                }

            }
        }
        else // if byte is not 255
        {
            if (write_file_open)
            {
                //back up one byte
                fseek(inptr, -1, SEEK_CUR);

                //read FAT block from forensic image into buffer
                fread(FAT_block, sizeof(FAT_block), 1, inptr);

            //     // write FAT block buffer to file
            //     fwrite(FAT_block, sizeof(FAT_block), 1, outptr);
            }

        }
    }

    // close files
    fclose(inptr);
    // fclose(outptr);
    write_file_open = false;
    printf("Sigs: %i\n", counter);

    return 0;
}

// helper functions
int check_args(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Invalid Input.\nForensic image file mist be provided.\n");
        return 1;
    }

    FILE *inptr = fopen(argv[1], "r");

    if (inptr == NULL)
    {
        fprintf(stderr, "File does not exist.\n");
        return 2;
    }

    fclose(inptr);


    return 0;
}

bool signature_is_present(BYTE potential_sig[4], SIG jpeg_sig)
{
    if( potential_sig[0] == jpeg_sig.byte_1 &&
        potential_sig[1] == jpeg_sig.byte_2 &&
        potential_sig[2] == jpeg_sig.byte_3 &&
        potential_sig[3] >= jpeg_sig.byte_4_min &&
        potential_sig[3] <= jpeg_sig.byte_4_max )
    {
        return true;
    }

    return false;
}

SIG set_jpeg_signature(void)
{
    SIG jpeg_sig;

    jpeg_sig.byte_1 = 255;
    jpeg_sig.byte_2 = 216;
    jpeg_sig.byte_3 = 255;
    jpeg_sig.byte_4_min = 224;
    jpeg_sig.byte_4_max = 239;

    return jpeg_sig;
}

// padding to turn the counter into a three digit string.
// Assumes nothing lower than 0 and nothing higher than 99.
char * z_pad(int counter)
{
    if (counter < 10)
    {
        return "00";
    }

    return "0";
}


// PLAN

// create buffer 512 bits (64 bytes) in size (FAT storage block)

// open forensic image file

  // open loop

      // read 64 bytes into buffer

      // check first 3 bytes

        // if they match jpeg signature

          // check if 4th byte falls within proper range

            // if they do:

                // check if an output file is open (variable is true). // Will be true all the time, except for the first time. Neccessary?

                // if one is open, CLOSE CURRENT OUTPUT FILE! Mark bool variable to false.

                // open new output file (pic_counter) // dynamic string names?
                // need to set some variable to true - signifying that there is one open.???

                // write buffer to output file

            // if they don't:

                // check if output file is open??

                // write buffer to current output file.

    // close loop

    // close current output file

    // close forensic image file.