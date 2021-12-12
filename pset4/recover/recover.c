/*
recover.c is a program that recovers JPEGs from a forensic image

Assumes JPEGs stored back to back, as 512 Byte blocks

The first four bytes of a JPEG follows the pattern:
    First three bytes: 0xff 0xd8 0xff
    Last byte: 0xe0, 0xe1, 0xe2, ..., 0xef

Pseudocode:

    open memory card
    repeat until end of card
        read 512 bytes into a buffer
        if start of a new jpeg
            If first JPEG
            ...
            Else
            ...
        Else
            If already found JPEG
            ...
    Close any remaining files
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


int JPEG_BLOCK = 512; // 512 bytes for jpeg

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // check usage
    if (argc < 2 || argc > 2) {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // open memory card and get file pointer, check if valid
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Image caannot be opened: NULL");
        return 1;
    }




    // repeat until end of card
        // read 512 bytes into a buffer
        // if start of a new jpeg
            // If first JPEG
            // ... start writing first file
            // Else
            // ... already found jpeg, close file we have been writing to
        // Else
            // If already found JPEG, keep writing to it
            // ...
    // Close any remaining files



    // declare/initialize
    int size_t;
    int counter = 0;
    int JPEG_counter = 0;
    // loop while getting 512 block-sized chunks
    do
    {
        // read to buffer
        BYTE buffer[JPEG_BLOCK];
        size_t = fread(buffer, sizeof(BYTE), JPEG_BLOCK, file);

        // printf("%i, %i\n", counter, size_t);

        if (size_t == JPEG_BLOCK)
        {
            // check if start of JPEG; bitwise & clears last 4 bits so can compare directly to 0xe0
            if (buffer[0] == 0xff && buffer[1] == 0xd8 &&
                buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
            {
                JPEG_counter += 1;
                printf("%i\n", JPEG_counter);
            }

        }




        counter += 1;
    }
    while (size_t == JPEG_BLOCK);


    // printf("%hhu\n", buffer[0] );


    // sprintf(filename, "%03i.jpg", jpeg_counter);
    // FILE *img = fopen(filename, "w");
    // fwrite(data, size, number, inptr);


    return 0;
}