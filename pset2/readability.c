/*
Computes the Coleman-Liau index for US reading level given text

index = 0.0588 * L - 0.296 * S - 15.8

L = average number of letters per 100 words in the text
S = average number of sentences per 100 words in the text

*/


#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


int main(void)
{
    // Prompt user for text
    string text = get_string("Text : ");
    int n = strlen(text);
    if (n == 0)
    {
        return 1;
    }

    // Count the number of letters, words, and sentences in the text
    int letter_count = 0;
    int word_count = 1;
    int sentence_count = 0;
    for (int i = 0; i < n; i++)
    {
        // sentences
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            sentence_count += 1;
        }
        // words
        if (text[i] == ' ')
        {
            word_count += 1;
        }
        // letters
        if ((text[i] >= 'a' && text[i] <= 'z') ||
            (text[i] >= 'A' && text[i] <= 'Z'))
        {
            printf("%c\n", text[i]);
            letter_count += 1;
        }
    }
    float L = 100. * (float)letter_count / (float)word_count;
    float S = 100. * (float)sentence_count / (float)word_count;

    // printf("%i letter(s)\n", letter_count);
    // printf("%i words(s)\n", word_count);


    // Coleman-Liau equation
    float grade = 0.0588 * L - 0.296 * S - 15.8;

    // Print grade
    if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", (int) round(grade));
    }
}