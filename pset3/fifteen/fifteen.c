/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements the Game of Fifteen (generalized to d x d).
 *
 * Usage: ./fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [MIN,MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// board's minimal dimension
#define MIN 3

// board's maximal dimension
#define MAX 9

// board, whereby board[i][j] represents row i and column j
int board[MAX][MAX];

// board's dimension
int d;

// dashes to draw board
int dashes;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void save(void);
bool tileSpace();
bool findTile(int tile);
void printDashes(int n);

// init some vars
int temp = 0;
int findRow = 0, findCol = 0;
int moveRow = 0, moveCol = 0;
int spaceRow = 0, spaceCol = 0;

int main(int argc, string argv[])
{
    // greet player
    greet();

    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < MIN || d > MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            MIN, MIN, MAX, MAX);
        return 2;
    }

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // saves the current state of the board (for testing)
        save();

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep for animation's sake
        usleep(500000);
    }

    // that's all folks
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("LET THE GAMES BEGIN! GAME OF FIFTEEN\n");
    usleep(4000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1,
 * (i.e., fills board with values but does not actually print them),
 * whereby board[i][j] represents row i and column j.
 */
void init(void)
{
    // TODO
    int tile = d*d - 1;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            board[i][j] = tile--;
        }
    }
    if (d % 2 == 0)
    {
        board[d - 1][d - 3] = 1;
        board[d - 1][d - 2] = 2;
    }
    dashes = d * 7;
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for (int i = 0; i < d; i++)
    {
        printDashes(dashes);
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == 0)
            {
                printf("\033[22;34m   _  |\033[0m");
            }
            else
            {
                printf("\033[22;32m %3d\033[0m \033[22;34m |\033[0m", board[i][j]);
            }
        }
        printf("\n");
    }
    printDashes(dashes);

}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    if (findTile(tile))
    {
        moveRow = findRow;
        moveCol = findCol;
        
        if (tileSpace())
        {
            temp = board[moveRow][moveCol];
            board[moveRow][moveCol] = board[spaceRow][spaceCol];
            board[spaceRow][spaceCol] = temp;
            return true;
        }
    }
    return false;
}

/**
 * looks for spaces around tiles and assigns them.
 */
 
 bool tileSpace()
 {
    findTile(0);
    spaceRow = findRow;
    spaceCol = findCol;
    
    if (spaceCol == moveCol)
    {
        if (spaceRow == moveRow + 1 || spaceRow == moveRow - 1)
        {
            return true;
        }
    }
    if (spaceRow == moveRow)
    {
        if (spaceCol == moveCol + 1 || spaceCol == moveCol - 1)
        {
            return true;
        }
    }
    
    return false; 
 }

/**
 * finds row&col of tile user asked for.
 */
 
 bool findTile(int tile)
 {
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == tile)
            {
                findRow = i;
                findCol = j;
                return true;
            }
        }
    }
    return false;
 }

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    int n = 1;
    
    if (board[d - 1][d - 1] != 0)
    {
        return false;
    }
    
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == n)
            {
                n++;
                if ((n == d*d) && (board[d-1][d-1] == 0))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

/**
 * prints dashes to draw the board.
 */
 
 void printDashes(int n)
 {
    for (; n > 0; n--)
    {
        printf("\033[22;34m-\033[0m");
    }
    printf("\n");
 }

/**
 * Saves the current state of the board to disk (for testing).
 */
void save(void)
{
    // log
    const string log = "log.txt";

    // delete existing log, if any, before first save
    static bool saved = false;
    if (!saved)
    {
        unlink(log);
        saved = true;
    }

    // open log
    FILE* p = fopen(log, "a");
    if (p == NULL)
    {
        return;
    }

    // log board
    fprintf(p, "{");
    for (int i = 0; i < d; i++)
    {
        fprintf(p, "{");
        for (int j = 0; j < d; j++)
        {
            fprintf(p, "%i", board[i][j]);
            if (j < d - 1)
            {
                fprintf(p, ",");
            }
        }
        fprintf(p, "}");
        if (i < d - 1)
        {
            fprintf(p, ",");
        }
    }
    fprintf(p, "}\n");

    // close log
    fclose(p);
}
