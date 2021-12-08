#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt for start size
    int start_size;
    do
    {
        start_size = get_int("Start size: ");
    }
    while (start_size < 9);


    // Prompt for end size
    int end_size;
    do
    {
        end_size = get_int("End size: ");
    }
    while (end_size < start_size);


    // Initialize
    int n = 0;
    int pop = start_size;

    // Calculate number of years until we reach threshold
    while (pop <  end_size)
    {
        // Update the population
        pop = pop + (pop / 3) - (pop / 4);

        // iIcrement counter
        n += 1;
    }

    // Print number of years
    printf("Years: %i\n", n);
}