/**
 * greedy.c
 * 
 * greedy cashier - exchange cash
 * using the minimum amount of coins possible.
 *
 * Problem Set 1
 * CS50
 *
 * by flxio
 */


#include <cs50.h>
#include <math.h>
#include <stdio.h>

#define QUARTER 25
#define DIME 10
#define NICKEL 5
#define PENNY 1

int GetPositiveFloat(void);

int main(void)
{
    // amount to exchange
    float exchange = GetPositiveFloat();
    
    // round amount to exchange
    int change = (int) round(exchange*100);
    
    // keep track of amount of coins used.
    int coins = 0;
    int quarter = 0; 
    int dime = 0;
    int nickel = 0; 
    int penny = 0;
    
    /** 
     * as long as amount is > 0, check which coins can be used to echange.
     * then keep track of how many coins were used.
     */
    do
    {
        // check if we can use quarters to exchange.
        while (change >= QUARTER)
        {
            coins++;
            quarter++;
            change -= QUARTER;
        }
        // same for dimes.
        while (change >= DIME)
        {
            coins++;
            dime++;
            change -= DIME;
        }
        // same for nickels.
        while (change >= NICKEL)
        {
            coins++;
            nickel++;
            change -= NICKEL;
        }
        // same for pennys.
        while (change >= PENNY)
        {
            coins++;
            penny++;
            change -= PENNY;
        }
    }
    while(change > 0);
    
    // which and how many coins used to exchange.
    printf("There you go! I gave you %i quarters, %i dimes, %i nickels and %i pennys.\n", 
            quarter, dime, nickel, penny);
            
    printf("That makes %i coins total!\n", coins);
}

int GetPositiveFloat(void)
{
    int n;
    do
    {
        printf("Amount to exchange: ");
        n = GetFloat();
    }
    while (n <= 0);
    
    return n;
}
