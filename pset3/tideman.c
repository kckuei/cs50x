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
bool cycle(int end, int cycle_start);
void print_winner(void);
void print_preference(void);

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

    print_preference();

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
        // find index position of preferred candidate 'i' in ranks array
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
            // loop on the lower ranked candidates; note we start from the index + 1
            // only if the candidate rank == j do we increment to acknolwedge that i is preferred to j
            // b/c they are lower ranked
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

    // The function should add all pairs of candidates where one candidate is preferred
    // to the pairs array. A pair of candidates who are tied (one is not preferred over
    // the other) should not be added to the array.

    // The function should update the global variable pair_count to be the number of pairs
    // of candidates. (The pairs should thus all be stored between pairs[0] and
    // pairs[pair_count - 1], inclusive).

    int j_start = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = j_start; j < candidate_count; j++)
        {
            // compare the cross diaganols to check if a candidate is preferred for given pair
            pair p;
            if (preferences[i][j] > preferences[j][i])
            {
                p.winner = i;
                p.loser = j;
                pairs[pair_count] = p;
                printf("winner=%i [%s], loser=%i [%s]\n",  p.winner, candidates[p.winner], p.loser, candidates[p.loser]);
                pair_count += 1;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                p.winner = j;
                p.loser = i;
                pairs[pair_count] = p;
                printf("winner=%i [%s], loser=%i [%s]\n",  p.winner, candidates[p.winner], p.loser, candidates[p.loser]);
                pair_count += 1;
            }
        }

        j_start += 1;
    }
    printf("Number of pairs: %i\n",  pair_count);

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    // Sort pairs in order by decreasing strength of victor
    // strength of victor defined as number of voters who prefer the preferred candidate

    // Use bubblesort to sort pairs based on strength of victory
    // for each loop, decrement the number of positions checked
    for (int i = pair_count - 1; i >= 0 ; i--)
    {
        // each loop, only need check up to max positions
        for (int j = 0; j <= i - 1; j++)
        {
            // if current votes less than next pair, swap their position
            if ((preferences[pairs[j].winner][pairs[j].loser]) < (preferences[pairs[j + 1].winner][pairs[j + 1].loser]))
            {
                pair temp = pairs[j]
                pairs[j] = pairs[j + 1];
                pairs[j + 1] = temp;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    // Update locked to create the locked graphy by adding all edges in
    // decreasing order of victory strength, as long as there is no cycle

    for (int i = 0; i < pair_count; i++)
    {
        // lock the connection if it does not create a cycle
        if (!cycle(pairs[i].loser, pairs[i].winner))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}


// recursive function for checking connections to seeing if a cycle is created
bool cycle(int end, int cycle_start)
{
    // end is loser
    // cycle_start is the original winner

    // Return true if there is a cycle created (base case)
    if (end == cycle_start)
    {
        return true;
    }

    // Loop through candidates (recursive case)
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[end][i])
        {
            if (cycle(i, cycle_start))
            {
                return true;
            }
        }
    }
    return false;
}


// Print the winner of the election
void print_winner(void)
{
    // TODO
    // print out the winner of the election, who will be the source of the graph
    // you may assume there will not be more than one source

    // The source is whichever candidate that doesn't have anyone pointing at them,
    // all columns are false in preferences
    // loop on columns
    for (int i = 0; i < candidate_count; i++)
    {
        int counter = 0;
        // loop on rows
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == false)
            {
                counter += 1;
            }
        }
        if (counter == candidate_count)
        {
            printf("%s\n", candidates[i]);
            break;
        }
    }
    return;
}

void print_preference(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%i ", preferences[i][j]);
        }
        printf("\n");
    }
}