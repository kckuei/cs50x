#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// 2d boolean array representing candidate graph
// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    // look for a candidate called name
    // if candidate found, update ranks and return true. ranks[i] is the voters ith preference
    // if no candidate found, don't update any ranks and return false
    // note: use strcmp() to compare two strings; returns 0 if equal
    for (int k = 0; k < candidate_count; k++)
    {
        if (strcmp(name, candidates[k]) == 0)
        {
            ranks[rank] = k;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    // update the global preference array variable based on the current voter's ranks
    // loop on i rows
    for (int i = 0; i < candidate_count; i++)
    {
        // find index position of i in ranks array
        int index;
        for (int k = 0; k < candidate_count; k++)
        {
            if (ranks[k] == i)
            {
                index = k;
                break;
            }
        }

        // loop on j columns
        for (int j = 0; j < candidate_count; j++)
        {
            // loop on lower rank position and update preference if match column j
            for (int m = index + 1; m < candidate_count; m++)
            {
                if (ranks[m] == j)
                {
                    preferences[i][j] += 1;
                    // Check assignments
                    printf("i=%i, j=%i, value=%i\n", i, j, preferences[i][j]);
                }
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    // add each pair of candidates to pairs array if one candidate is preferred over the other
    // update global variable pair_count to be the total number of pairs
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    // Sort pairs in order by decreasing strength of victor
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    // Update locked to create the locked graphy by adding all edges in decreasing order of victory strength, as long as there is no cycle
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    // print out the winnnter of the election, who will be the source of the graph
    // you may assume there will not be more than one source
    return;
}
