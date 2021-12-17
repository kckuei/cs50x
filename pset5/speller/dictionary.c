// Implements a dictionary's functionality
/*
Some comments on performance:

    using N = 1 for the hash table: time = 7.41s
    using N = 2000 with dbjb: time = 0.07s          x100 improvement
    using N = 10000 with dbjb: time = 0.05s         x150 improvement

*/

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table (for linked list representation)
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
// Can play with this value for optimizing/minimizing run time
const unsigned int N = 10000;

// Hash table
node *table[N];


// initialize word count;
unsigned int word_count = 0;


// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // implementation must be case insensitive
    // should only return true for words that are in the dictionary (match characters/length)
    // assume words upper/lower, alphabetical or apostrophes

    // hash word to obtain a hash value/index
    unsigned int index = hash(word);

    // access linked list at that index in the hash table (head)
    node *ptr = table[index];

    // loop on linked list until find word
    while (ptr != NULL)
    {
        // compare strings while ignoring case
        if (strcasecmp(word, ptr->word) == 0)
        {
            return true;
        }

        // advance pointer
        ptr = ptr->next;
    }

    // otherwise return false
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    // hash function must be insensitive to case
    // hash fcn takes a word, outputs a number index into an array to place the word

    // use djb2 by Dan Bernstein (http://www.cse.yorku.ca/~oz/hash.html)
    unsigned long hash = 5381;
    int c;
    while ((c = tolower(*word++)))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % N;

    // return 0;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    // Note: You may assume that dictionary will contain at least one word, that no word will be longer
    // than LENGTH (a constant defined in dictionary.h) characters, that no word will appear more than once,
    // that each word will contain only lowercase alphabetical characters and possibly apostrophes, and
    // that no word will start with an apostrophe.


    // open dictionary file
    FILE *file = fopen(dictionary, "r");

    // check if valid open
    if (file == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }

    // initialize string of max word length (+1 for '\0' at end of word)
    char word[LENGTH + 1];

    // read strings from one at a time from file (fscanf returns EOF once end of file reached)
    while (fscanf(file, "%s", word) != EOF)
    {
        // create a new node for each word and allocate memory
        node *n = malloc(sizeof(node));

        // check if memory allocation valid
        if (n == NULL)
        {
            printf("MALLOC: Memory allocation error.\n");
            return false;
        }

        // copy the word to the node and set the next pointer to NULL
        strcpy(n->word, word);
        n->next = NULL;

        // hash word to obtain a hash value/index
        int hash_index = hash(word);

        // if no node at the hash_index, insert the node pointer directly
        if (table[hash_index] == NULL)
        {
            table[hash_index] = n;
        }
        // otherwise add to existing linked list
        else
        {
            // set the pointer of the new node to the head
            n->next = table[hash_index];

            // reset head to new node
            table[hash_index] = n;
        }

        // increment word count
        word_count += 1;

        // printf("%s\n", word);
    }
    // close file
    fclose(file);

    // sucessful load
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    // return number of words in dictionary

    // // Uncomment to re-calculate size instead of using the word_count from load()
    // // reset word_count and initialize pointer tmp
    // word_count = 0;
    // node *tmp = NULL;
    // // loop on hash slots
    // for (int i = 0; i < N; i++)
    // {
    //     // check if hash points to node
    //     tmp = table[i];
    //     if (tmp->next != NULL)
    //     {
    //         word_count += 1;
    //     }
    //     // loop on linked list
    //     while ((tmp->next != NULL))
    //     {
    //         tmp = tmp->next;
    //         word_count +=1;
    //     }
    // }

    // printf("Word count: %i\n", word_count);

    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO

    // loop on hash table
    for (int i = 0; i < N; i++)
    {
        // initialize pointer to head of linked list
        node *pointer = table[i];

        // loop on linked list
        while (pointer != NULL)
        {
            // create temp pointer to current pointer
            node *temp = pointer;

            // advance current pointer to next node
            pointer = pointer->next;

            // free temp pointer
            free(temp);
        }

        // sucuessfully freed linked list nodes, exit and return true
        if (i == N - 1 && pointer == NULL)
        {
            return true;
        }
    }


    return false;
}
