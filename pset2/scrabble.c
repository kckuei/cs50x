#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // TODO: Print the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!");
    }
    else if (score2 > score1)
    {
        printf("Player 2 wins!");
    }
    else
    {
        printf("Tie!");
    }

}

int compute_score(string word)
{
    // TODO: Compute and return score for string
    int starter_index = 97;
    int case_offset = 32;
    int n = strlen(word);
    int result = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < 26; j++)
        {
            // check if ascii is lower or upper case
            if (word[i] == starter_index + j || word[i] == starter_index - case_offset + j)
            {
                // echo the scores by letter
                // printf("%c is worth %i points\n", word[i], POINTS[j]);

                // sum the points
                result += POINTS[j];
            }
        }
    }
    return result;
}