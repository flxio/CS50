/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    int beginning = 0;
    int ending = n - 1;
    // while length of list > 0
    while (ending >= beginning)
    {
        // look at middle of list, if number found, return true
        int middle = (beginning + ending) / 2;
        
        if (values[middle] == value)
        {
            return true;
        }   
        // else if number higher, search left
        else if (values[middle] > value)
        {
            ending = middle - 1;
        }
        // else if number lower, search right
        else
        {
            beginning = middle + 1;
        }
    
    }   
    // return false
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // implement an O(n^2) sorting algorithm
    int counter;
    do
    {
        counter = 0;
        int temp;
        for (int i = 0; i < n; i++)
        {
            if (values[i] > values[i+1])
            {
                temp = values[i];
                values[i] = values[i+1];
                values[i+1] = temp;
                counter++;
            }
        }
    }
    while (counter > 0);
}
