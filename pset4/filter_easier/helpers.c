#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float avg = (image[i][j].rgbtBlue + image[i][j].rgbtRed + image[i][j].rgbtGreen)/3.;

            avg = round(avg);

            if (avg < 0)
            {
                avg = 0;
            }
            if (avg > 255)
            {
                avg = 255;
            }
            image[i][j].rgbtBlue = (int)avg;
            image[i][j].rgbtRed = (int)avg;
            image[i][j].rgbtGreen = (int)avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            float originalRed = image[i][j].rgbtRed;
            float originalBlue = image[i][j].rgbtBlue;
            float originalGreen = image[i][j].rgbtGreen;

            float sepiaRed = .393 * originalRed + .769 * originalGreen + .189 * originalBlue;
            float sepiaGreen = .349 * originalRed + .686 * originalGreen + .168 * originalBlue;
            float sepiaBlue = .272 * originalRed + .534 * originalGreen + .131 * originalBlue;

            sepiaRed = round(sepiaRed);
            sepiaGreen = round(sepiaGreen);
            sepiaBlue = round(sepiaBlue);

            if (sepiaRed > 255) {
                sepiaRed = 255;
            }
            if (sepiaRed < 0) {
                sepiaRed = 0;
            }

            if (sepiaBlue > 255) {
                sepiaBlue = 255;
            }
            if (sepiaBlue < 0) {
                sepiaBlue = 0;
            }

            if (sepiaGreen > 255) {
                sepiaGreen = 255;
            }
            if (sepiaGreen < 0) {
                sepiaGreen = 0;
            }

            image[i][j].rgbtBlue = (int)sepiaBlue;
            image[i][j].rgbtRed = (int)sepiaRed;
            image[i][j].rgbtGreen = (int)sepiaGreen;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width/2; j++)
        {
            RGBTRIPLE temp = image[i][j];

            image[i][j] = image[i][width - (j + 1)];

            image[i][width - (j + 1)] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Make a copy of original image
    RGBTRIPLE orig[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            orig[i][j] = image[i][j];
        }
    }

    // Loop through rows
    for (int i = 0; i < height; i++)
    {
        // Loop through columns
        for (int j = 0; j < width; j++)
        {
            // Declare/initialize value
            float sum_red = 0;
            float sum_blue = 0;
            float sum_green = 0;
            int counter = 0;

            // For each pixel, loop vertical and horizontal
            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    // Check if pixel is outside rows
                    if (i + k < 0 || i + k >= height)
                    {
                        continue; // skip to next iteration
                    }
                    // Check if pixel is outside columns
                    if (j + l < 0 || j + l >= width)
                    {
                        continue; // skip to next iteration
                    }
                    // Otherwise add to sums
                    sum_red += orig[i + k][j + l].rgbtRed;
                    sum_blue += orig[i + k][j + l].rgbtBlue;
                    sum_green += orig[i + k][j + l].rgbtGreen;
                    counter++;
                }
            }

            // Get average to blur image
            image[i][j].rgbtRed = round(sum_red / counter);
            image[i][j].rgbtGreen = round(sum_green / counter);
            image[i][j].rgbtBlue = round(sum_blue / counter);
        }
    }
    return;
}
