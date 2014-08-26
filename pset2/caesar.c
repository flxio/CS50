/**
 * caesar.c
 *
 * uses caesar cipher to encrypt
 * user input.
 *
 * Problem Set 2
 * CS50
 *
 * by flxio
 */

#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, string argv[])
{
    // check commandline arguments
    if (argc != 2)
    {
        printf("Usage: caesar <key>\n");
        return 1;
    }
  
    // convert encryption key to int
    int key = atoi(argv[1]);
    
    /**
     * check if encyption key is 0, if so
     * return error to user, because 0 won't 
     * encrypt text!
     */
    if (key == 0)
    {
        printf("ERROR: 0 is not a valid encryption key!\n");
        return 2;
    }
    
    int keylength = strlen(key);
    for (int i = 0; i < keylength; i++)
    {
        key[i] = argv[1][i];
    }
        
    // get plaintext from user
    printf("Enter text to encrypt: ");
    string plaintext = GetString();
           
    // iterate over plaintext
    for (int i = 0, plength = strlen(plaintext); i < plength; i++)
    {
        // check if char is alphabetical
        if (isalpha(plaintext[i]))
        {
            // uppercase
            if (isupper(plaintext[i]))
            {
                printf("%c", ((plaintext[i] + key) - 'A') %26 + 'A');
            }
            // lowercase
            else if (islower(plaintext[i]))
            {
                printf("%c", ((plaintext[i] + key) - 'a') %26 + 'a');
            }  
        }
        // if not alphabetical, just print it out.
        else
        {
            printf("%c", plaintext[i]);
        }
    }
    printf("\n");
        
}
