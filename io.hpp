#include <iostream>

using namespace std;

/**
 * DESCRIPTION:
 *  Prints the instruction for the user.
 */
void printInstructions()
{
    printf("-------------------------------------------------------------\n"
           "Instructions:\n"
           "        Enter the Initial and Goal state of the 8-puzzle board\n"
           "    from 0-8 integers, where 0 represents the blank space on\n"
           "    the board.\n"
           "-------------------------------------------------------------\n\n");

    cout << "8-Puzzle Solver" << endl;
    cout << "---------------" << endl;
    cout << "  Goal State  " << endl;
    cout << "-------------" << endl;
    cout << "| 1 | 2 | 3 |" << endl;
    cout << "-------------" << endl;
    cout << "| 8 | 0 | 4 |" << endl;
    cout << "-------------" << endl;
    cout << "| 7 | 6 | 5 |" << endl;
    cout << "------------|" << endl
         << endl;
}

/**
 * DESCRIPTION:
 *  gets input from the user.
 **/
void inputBoard(State *start_state)
{
    //State start_state;
    //User inputs board configuration
    int temp[9] = {0};
    int num;
    cout << "Enter Puzzle Configuration: \n";
    cout << "---------------------------\n";
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            printf("board[%i][%i]: ", row, col);
            cin >> num;

            if (num >= 0 && num < 9) //checks if the input is a blank character
            {
                if (!temp[num]) //checks if the input is repeated
                {
                    start_state->A[row][col] = num;
                    temp[num] = 1;
                }
                else
                {
                    printf("    Error: Number %i is already used. Try again!\n", num);
                    col--;
                }
            }
            else
            {
                printf("    Error: Invalid input. Enter a number from 0-8\n");
                col--;
            }
        }
    }
    printf("\n");
}

/**
 * DESCRIPTION:
 *  displays the content of board
 **/
void printBoard(State *start_state)
{
    cout << "Board Configuration" << endl;
    cout << "-------------------" << endl;
    cout << "Initial State   " << endl;
    for (int row = 0; row < 3; row++)
    {
        printf("------------\n");
        for (int col = 0; col < 3; col++)
        {
            printf("| %i ", start_state->A[row][col]);
        }
        printf("|\n");
    }
    printf("------------\n");
}

/**
 * DESCRIPTION:
 *  interprets and prints the moves.
 * PARAMETER:
 *  s - contains the list of nodes from the root to the goal
 * 
 **/
void PrintPath(State *s)
{
    int counter = 0;
    const char *action[4] = {"Up", "Down", "Left", "Right"};
    int num = s->g;
    int temp[num];

    for (s; s; s = s->parent, counter++)
    {
        temp[counter] = s->move;
    }

    for (int i = 1; i < num + 1; i++)
    {
        int tmp = temp[num - i];
        if (i + 1 != num + 1)
        {
            printf("%s - ", action[tmp]);
        }
        else
        {
            printf("%s ", action[tmp]);
        }
    }
    printf("\n");
}