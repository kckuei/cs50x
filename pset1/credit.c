/*
CS50x, Luhn's Algorithm

Multiply every other digit by 2, starting with the number’s second-to-last digit, and then add those products’ digits together.
Add the sum to the sum of the digits that weren’t multiplied by 2.
If the total’s last digit is 0 (or, put more formally, if the total modulo 10 is congruent to 0), the number is valid!

Program should print card based on:

    AMEX, 15 digits, starts wth 34 or 37
    MASTERCARD, 16 digits, starts with 51, 52, 53, 54, or 55
    VISA, 13 or 16 digits, starts with 4
    INVALID

https://docs.google.com/spreadsheets/d/1bHOLPk0wn8ktHDnkdsuzJkVkMnJxSThAUleYQ7iObEw/edit#gid=0
*/

#include <cs50.h>
#include <stdio.h>


int main(void)
{
    // Get the card number
    long card_number;
    do
    {
        card_number = get_long("Card Number: ");
    }
    while (card_number < 0);

    // // Stand in for testing
    // long card_number = 4003600000000014;
    // long card_number = 378282246310005 ;
    // printf("Card number: %li\n",card_number);


    // First pass;
    // Sum every other number digit starting from the second to last number;
    // Double that value, then some the individual digits
    long j = card_number;
    int sum1 = 0;
    int R, dig1, dig2;
    while (j > 0)
    {
        // Get every other digit
        R = ((j % 100) / 10);

        // Double it
        R *= 2;

        // Split R into seperate digits for summing
        dig1 = R % 10;      // tens pos
        dig2 = R / 10;      // ones pos

        // Sum the digits
        sum1 += dig1 + dig2 ;

        // Reduce j by 2 places
        j = j / 100;
    }
    printf("First sum: %i\n", sum1);


    // Second pass;
    // Sum every other digit that was not doubled in first pass starting from last number;
    j = card_number;
    int sum2 = 0;
    while (j > 0)
    {
        // Get every other digit
        R = j % 10;

        // Sum the digits
        sum2 += R;

        // Reduce j by 2 places
        j = j / 100;
    }
    printf("Second sum: %i\n", sum2);

    // Combine the sums
    int check_sum = (sum1 + sum2) % 10;
    printf("Checksum (should equal zero): %i\n", check_sum);

    // Get the first two digits
    long first_two_digits = card_number;
    while (first_two_digits >= 100)
    {
        first_two_digits = first_two_digits / 10 ;
    }
    printf("First two digits: %li\n", first_two_digits);


    // Get the length of the card number
    long i = card_number;
    long digits = 0;
    while (i > 0)
    {
        i = i / 10 ;
        digits ++;
    }
    printf("Card length: %li\n", digits);


    // Initialize check sum bool
    bool valid_check_sum = false;
    if (check_sum == 0)
    {
        valid_check_sum = true;
    }

    // Compare card digits and length
    if ((digits == 15 && (first_two_digits == 34 || first_two_digits == 37)) * valid_check_sum)
    {
        printf("AMEX\n");
    }
    else if ((digits == 16 && (first_two_digits > 50 && first_two_digits < 56)) * valid_check_sum)
    {
        printf("MASTERCARD\n");
    }
    else if (((digits == 13 || digits == 16) && (first_two_digits / 10) == 4) * valid_check_sum)
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }

}