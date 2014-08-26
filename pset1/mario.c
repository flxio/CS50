/**
 * mario.c
 * Displays a half-pyramid of blocks (mario-style!)
 *
 * Problem Set 1
 * CS50
 *
 * by flxio
 */

#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    
    // check user input until user inputs a valid number.
    do
    {
        printf("Positive int up to 23 to display pyramide height: ");
        height = GetInt();
    }
    while (height < 0 || height > 23);
    
    
    // display the (half-)pyramid
    for (int i = 0; i < height; i++)
    {
        // display spaces
        for (int h = height - 1; h > i; h--)
        {
            printf(" ");
        }
        // display #
        for (int j = -2; j < i; j++)
        {
            printf("#");
        }
        printf("\n");        
    }
    
    return 0;
}
