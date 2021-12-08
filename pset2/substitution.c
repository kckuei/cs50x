/* Encrypts message using a substitution cipher.

7 December 2021
KCK

Specification:
1. Implement your program in a file called substitution.c in a directory called substitution.
2. Your program must accept a single command-line argument, the key to use for the substitution. The key itself should be case-insensitive, so whether any character in the key is uppercase or lowercase should not affect the behavior of your program.
3. If your program is executed without any command-line arguments or with more than one command-line argument, your program should print an error message of your choice (with printf) and return from main a value of 1 (which tends to signify an error) immediately.
4. If the key is invalid (as by not containing 26 characters, containing any character that is not an alphabetic character, or not containing each letter exactly once), your program should print an error message of your choice (with printf) and return from main a value of 1 immediately.
5. Your program must output plaintext: (without a newline) and then prompt the user for a string of plaintext (using get_string).
6. Your program must output ciphertext: (without a newline) followed by the plaintext’s corresponding ciphertext, with each alphabetical character in the plaintext substituted for the corresponding character in the ciphertext; non-alphabetical characters should be outputted unchanged.
7. Your program must preserve case: capitalized letters must remain capitalized letters; lowercase letters must remain lowercase letters.
8. After outputting ciphertext, you should print a newline. Your program should then exit by returning 0 from main.

ASCII codes:
https://www.asciitable.com/

Exampling usage:

./substitution VCHPRZGJNTLSKFBDQWAXEUYMOI

plaintext:     ABCDEFGHIJKLMNOPQRSTUVWXYZ
ciphertext:    VCHPRZGJNTLSKFBDQWAXEUYMOI

plaintext:     Hello There!
ciphertext:    Jrssb Xjrwr!

*/

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


// declare prototypes
int validate_key(string key);
int is_lowercase(char letter);
string make_lowercase(string expression);

// Number of cipher charactres (equal number of alphabet characters)
const int N = 26;


int main(int argc, string argv[])
{
    // Check proper syntax usage
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // Get the cipher key (2nd arg)
    string key = argv[1];

    // Validate the cipher key
    if (validate_key(key) == 1)
    {
        // exit
        return 1;
    }

    // Get plain text from user
    string plaintext = get_string("plaintext: ");

    // Create lowercase version of cipher key
    string key_lower = make_lowercase(key);
    // printf("lower case cipher key: %s\n", key_lower);


    // Encipher the plain text...

    // Initialize copy of ciphertext as plaintext
    string ciphertext = plaintext;

    // Loop on characters
    int n = strlen(plaintext);
    for (int i = 0; i < n; i++) {

        // Loop on key characters
        for (int j = 0; j < N; j++)
        {
            // Enters on index j when lower or upper case match found in plaintext
            // operate on characters directly (implicitly 'a' = 97, 'A' = 65)
            if (plaintext[i] == 'a' + j || plaintext[i] == 'a' - ('a'-'A') + j)
            {
                // assign cipher based on upper/lower case
                if (is_lowercase(plaintext[i]) == true)
                {
                    // user lowercase cipher
                    ciphertext[i] = key_lower[j];
                    break;
                }
                else
                {
                    // user uppercase cipher
                    ciphertext[i] = key_lower[j] - ('a'-'A');
                    break;
                }
                // printf("%c\n", ciphertext[i]);

            }
        }
    }

    // Echo ciphertext
    printf("ciphertext: %s\n", ciphertext);

    // No errors
    return 0;
}


string make_lowercase(string expression)
{
    string new_expression = expression;
    int n = strlen(expression);
    for (int i = 0; i < n; i++)
    {
        if (is_lowercase(expression[i]) == false)
        {
            new_expression[i] = expression[i] + ('a'-'A');
        }
    }
    return new_expression;
}


int is_lowercase(char letter)
{
    // Check if lower case
    if (letter >= 'a' && letter <= 'z')
        return true;
    // else upper case
    else
        return false;
}


int validate_key(string key)
{

    // Check the length of the key
    int n = strlen(key);
    if (n != N)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // Check all alphabetical characters
    for (int i = 0; i < n; i++)
    {
        // lowercase asci begin from 97 to 122, uppercase ascii begin from 65 to 90
        if (!((key[i] >= 'a' && key[i] <= 'z') ||
              (key[i] >= 'A' && key[i] <= 'Z')))
        {
            printf("Key must use alphabetic characters.\n");
            return 1;
        }
    }

    // Check for repeat alphabet chars
    for (int i = 0; i < N; i++)
    {
        int counter = 0;
        for (int j = 0; j < n; j++)
        {
            // increment count if upper or lower case alphabet char
            // operate on characters directly (implicitly 'a' = 97, 'A' = 65
            // upper/lower offset by 32 places
            if (key[j] == 'a' + i || key[j] == 'a' - ('a' - 'A') + i)
            {
                counter += 1;
            }
            // exit if repeat character
            if (counter > 1)
            {
                printf("Key must use no repeat characters.\n");
                return 1;
            }
        }
    }
    // pass all cipher validation checks
    return 0;
}

