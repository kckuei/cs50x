/*
recover.c is a program that recovers JPGs from a forensic image

Assumes JPGs stored back to back, as 512 Byte blocks

The first four bytes of a JPG follows the pattern:
    First three bytes: 0xff 0xd8 0xff
    Last byte: 0xe0, 0xe1, 0xe2, ..., 0xef

Pseudocode:

    open memory card
    repeat until end of card
        read 512 bytes into a buffer
        if start of a new jpg
            If first JPEG
            ...
            Else
            ...
        Else
            If already found JPG
            ...
    Close any remaining files
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h> // for bools


int JPG_BLOCK = 512; // 512 bytes for jpg

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // check usage
    if (argc < 2 || argc > 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // open memory card and get file pointer, check if valid
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Image caannot be opened: NULL");
        return 1;
    }


    // initialize
    int jpg_counter = 0;
    char jpg_filename[8];
    FILE *image = NULL;
    BYTE buffer[JPG_BLOCK];

    // loop to continue reading into buffer while returning 512-byte sized blocks
    while (fread(buffer, sizeof(BYTE), JPG_BLOCK, file) == JPG_BLOCK)
    {
        // if start of the JPG sequence/pattern 0xff 0xd8 0xff 0xe*
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // close image on seubsequent start sequences (skips first image)
            if (jpg_counter > 0)
            {
                fclose(image);
            }

            // setup new output file
            sprintf(jpg_filename, "%03i.jpg", jpg_counter);
            image = fopen(jpg_filename, "w");

            // increment counter (once > 0, buffer always write to image)
            jpg_counter ++;
        }
        // write to image file
        if (jpg_counter > 0)
        {
            fwrite(buffer, sizeof(BYTE), JPG_BLOCK, image);
        }

    }

    // close file
    fclose(file);

    // Check number of jpgs identified
    printf("%i\n", jpg_counter);

    return 0;
}