/**
 * breakout.c
 *
 * Problem Set 4
 * CS50
 *
 * run ./breakout GOD
 * if you want no-hands mode. 
 *
 * by flxio.
 */
 
// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 6

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// paddle constants
#define PADDLE_HEIGHT 10
#define PADDLE_WIDTH 53
#define PADDLE_X (WIDTH / 2) - (PADDLE_WIDTH / 2)
#define PADDLE_Y 510

// brick constants
#define BRICK_HEIGHT 10
#define BRICK_WIDTH 35

// ball constants
#define BALL_X (WIDTH/2) - 10
#define BALL_Y HEIGHT/2
#define BALL_WIDTH 20
#define BALL_HEIGHT 20

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);
GLabel initLivesBoard(GWindow window);
void updateLivesboard(GWindow window, GLabel label, int points);

int main(int argc, char* argv[])
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);
    
    // instantiate LivesBoard, centered in middle of window, above Scoreboard.
    GLabel liveslabel = initLivesBoard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
    // ball movement 
    double ball_vertical = 2.0;   
    double ball_horizontal = 2 * drand48();
    
    bool godmode = false;
    if (argc > 2)
    {
        return 1;
    }
    // detect god mode    
    if ((argc == 2) && (strcmp(argv[1], "GOD") == 0))
    {
        godmode = true;
    }
    
    waitForClick();

    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        // call detectCollision func to check for collisions
        GObject object = detectCollision(window, ball);
        
        // ball collision with paddle
        if (object == paddle)
        {
            ball_vertical = -ball_vertical;
        }
        
        // detect collision with bricks
        if (object != NULL)
        {
        
            if (strcmp(getType(object), "GRect") == 0 && object != paddle)
            {
                removeGWindow(window, object);
                ball_vertical = -ball_vertical;
                points++;
                bricks--;
                updateScoreboard(window, label, points);
                if (bricks == 0)
                {
                    printf("YOU WON! :)\n");
                }
            }
        }
        
        // ball collision with bottom edge
        if (getY(ball) >= getHeight(window))
        {
            lives--;
            updateLivesboard(window, liveslabel, lives);
            setLocation(ball, BALL_X, BALL_Y);
            setLocation(paddle, PADDLE_X, PADDLE_Y);
            waitForClick();
            
            if (lives == 0)
            {
                printf("GAME OVER :(\n");
            }
        }
        // if godmode is on, set paddle-x equal to ball-x
        if (godmode == true)
        {
            setLocation(paddle, getX(ball) + (BALL_WIDTH / 2) - (PADDLE_WIDTH / 2), PADDLE_Y);
        }
        else
        {       
            // check for mouse event
            GEvent event = getNextEvent(MOUSE_EVENT);
            // if we heard one
            if (event != NULL)
            {
                // if the event was movement
                if (getEventType(event) == MOUSE_MOVED)
                {
                    // ensure circle follows cursor on x-axis
                    double x = getX(event) - getWidth(paddle) / 2;
                    setLocation(paddle, x, PADDLE_Y);
                }
            }
        }   
        
        move(ball, ball_horizontal, ball_vertical);
        
        // bounce off left and right
        if (getX(ball) + getWidth(ball) >= getWidth(window) || getX(ball) <= 0)
        {
            ball_horizontal = -ball_horizontal;
        }
        // bounce off top
        else if (getY(ball) <= 0)
        {
            ball_vertical = -ball_vertical;
        }
        
        pause(7);
    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    int x = 2, y = 5;
    
    // brick colors - changing per row
    string brickColor[] = {"BLUE", "GREEN", "YELLOW", "RED", "PINK", "ORANGE"};
    int colorIndex = 0;
    for (int i = 1; i <= ROWS; i++)
    {
        for (int j = 1; j <= COLS; j++)
        {
            GRect brick = newGRect(x, y, BRICK_WIDTH, BRICK_HEIGHT);
            setColor(brick, brickColor[colorIndex]);
            setFilled(brick, true);
            add(window, brick);
            x += (BRICK_WIDTH+5);
        }
        x = 2;
        y += (BRICK_HEIGHT+5);
        colorIndex++;
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    GOval ball = newGOval(BALL_X, BALL_Y, BALL_WIDTH, BALL_HEIGHT);
    setColor(ball, "BLACK");
    setFilled(ball, true);
    add(window, ball);
    
    return ball;
    
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    GRect paddle = newGRect(PADDLE_X, PADDLE_Y, PADDLE_WIDTH, PADDLE_HEIGHT);
    setFilled(paddle, true);
    setColor(paddle, "BLACK");
    add(window, paddle);
    
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel label;
    double x, y;
    
    label = newGLabel("Score: 0");
    setFont(label, "SansSerif-18");
    x = (getWidth(window) - getWidth(label)) / 2;
    y = (getHeight(window) - getFontAscent(label)) / 2;
    setLocation(label, x, y);
    add(window, label);
    
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "Score: %i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}

/**
 * Instantiates, configures, and returns lives.
 */
GLabel initLivesBoard(GWindow window)
{
    GLabel label;
    double x, y;
    
    label = newGLabel("Lives: 3");
    setFont(label, "SansSerif-18");
    x = (getWidth(window) - getWidth(label)) / 2;
    y = ((getHeight(window) - getFontAscent(label)) / 2) -18;
    setLocation(label, x, y);
    add(window, label);
    
    return label;
}

/**
 * Updates liveboard's label, keeping it centered in window.
 */
void updateLivesboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "Lives: %i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = ((getHeight(window) - getHeight(label)) / 2) - 18;
    setLocation(label, x, y);
}
