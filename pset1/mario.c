#include <cs50.h>
#include <stdio.h>

// Left-oriented
// int main(void)
// {
//     // Get the height from user
//     int height;
//     do
//     {
//         height = get_int("Height: ");
//     }
//     while (height < 1 || height > 8);


//     // Make the blocks
//     for (int i = 0; i < height; i++)
//     {
//         for (int j = 0; j < height; j++)
//         {
//             // For LHS oriented
//             if (j < i)
//             {
//                 printf("#");
//             }

//         }
//         printf("\n");

//     }


// }


// Right-oriented
int main(void)
{
    // Get the height from user
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);


    // Make the blocks
    for (int i = 0; i < height; i++)
    {
        for (int j = height - 1; j >= 0 ; j--)
        {
            // For LHS oriented
            if (j <= i)
            {
                printf("#");
            }
            else
            {
                printf(" ");
            }

        }
        printf("\n");

    }

}