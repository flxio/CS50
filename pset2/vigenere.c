/**
 * vigenere.c
 * vigenere cipher - uses a keyword to encrypt text.
 *
 * Problem Set 2
 * CS50
 *
 * by flxio.
 */

#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

char encrypt(char ptext, int key);

int main(int argc, string argv[])
{
    // check # of commandline arguments.
    if (argc != 2)
    {
        printf("Usage: vigenere <keyword>\n");
        return 1;
    }
    
    // check encryption key.
    for (int i = 0, length = strlen(argv[1]); i < length; i++)
    {
        if (!isalpha(argv[1][i]))
        {
            printf("Only alphabetical characters allowed as a key!\n");
            return 1;
        }
    }
    
    // convert <keyword> to encryptionkey, so we can use it.
    string key = argv[1];
    int keylength = strlen(key);
    int encryptionkey[keylength];
    int keycount = 0;
    
    for (int i = 0; i < keylength; i++)
    {
        encryptionkey[i] = (toupper(key[i]) -'A');
    }
   
    // get plaintext.
    string plaintext = GetString();
    
    // iterate over plaintext.
    for (int i = 0, plength = strlen(plaintext); i < plength; i++)
    {
        // check if char is alphabetical.
        if (isalpha(plaintext[i]))
        {
            // call encrypt func and print out each char.
            printf("%c", encrypt(plaintext[i], encryptionkey[keycount]));
            
            // keep track of position in encryptionkey[], reset if end of string. 
            keycount++;
            if (keycount > keylength -1)
            {
                keycount = 0;
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

/**
 * Function used to encrypt each char,
 * `key` based on chosen keyword.
 */
char encrypt(char ptext, int key)
{
    if (isupper(ptext))
    {
        return ((((ptext +key) - 'A') %26) +'A');
    }
    else
    {
        return ((((ptext +key) - 'a') %26) +'a');
    }
}
