#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Get user name
    string name = get_string("What is your name?\n");

    // Say hello
    printf("hello, %s\n", name);
}