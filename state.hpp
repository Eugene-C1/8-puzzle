#include <iostream>
#include <ctime>

#include <list>

using namespace std;

//forward declarations
extern bool informed_search;
extern bool uninformed_search;
extern int Goal[3][3];

class State
{
public:
    int A[3][3], g, h, total_cost, heuristic_select;
    int move;
    State *parent;
    struct State *next;

    State();
    bool is_goal();
    bool operator==(const State &) const;
    bool operator<(const State &) const;
    void heuristics(); //General Heuristic function - calls one of the heuristic functions listed below

    int manhattanHeuristic(); //Heuristic Function
};

/**
 * DESCRIPTION:
 *  defines the node structure used to create a search tree
 */
State::State()
{
    g = h = total_cost = 0;
    parent = NULL;
    move;
    next;
}

/**
 * DESCRIPTION:
 *  checks if the current state is equal to goal state
 */
bool State::is_goal()
{

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            // if any two items not equal - not goal
            if (A[i][j] != Goal[i][j])
                return false;
        }
    }
    //reaching this point means all items are equal - goal state found
    return true;
}

/**
 * DESCRIPTION:
 *  Operator overloading: when the operator '==' is used find the blank space, goal state, 
 *  and the solution. It's used to get an expected result.
 **/
bool State::operator==(const State &r) const
{

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            //if any two items not equal - not goal
            if (A[i][j] != r.A[i][j])
                return false;
        }
    }
    // states are equal
    return true;
}

/**
 * DESCRIPTION:
 *  Operator Overloading: when the operator '<' is used it returns the heuristic cost
 *  if it's a informed search or returns r.g/cost for normal search.
 * RETURN:
 *  returns heuristic cost if it's a informed search; returns cost for normal search.
 * 
 **/
bool State::operator<(const State &r) const
{

    if (informed_search)
    { // for heuristic based algorithms
        return total_cost < r.total_cost;
    }
    else
    {
        return g < r.g; // for normal search algorithms
    }
}

/**
 * DESCRIPTION:
 *  saves the heuristic value to a state variable 'h'
 **/
void State::heuristics()
{
    h = manhattanHeuristic(); //saves the heuristic value in h
}

/**
 * DESCRIPTION:
 *  heurisitic function that assigns h-cost to nodes. The lower the cost 
 *  indicates that we are closer to the goal state.
 * RETURNS:
 *  returns a heuristic value
 */
int State::manhattanHeuristic()
{
    int h_val = 0;
    bool found;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            found = false;
            for (int i2 = 0; i2 < 3; i2++)
            {
                for (int j2 = 0; j2 < 3; j2++)
                {

                    if (Goal[i][j] == A[i2][j2])
                    {
                        //dx + dy
                        h_val += abs(i - i2) + abs(j - j2);
                        found = true;
                    }
                    if (found)
                        break;
                }
                if (found)
                    break;
            }
        }
    }
    return h_val;
}

/**
 * DESCRIPTION:
 *  searches for the child in the closed list; returns true if successfull; 
 *  returns false if not.
 **/
bool InClosed(State &s, list<State> &closed_list)
{
    for (list<State>::iterator it = closed_list.begin(); it != closed_list.end(); ++it)
    {
        if ((*it) == s)
        {
            return true;
        }
    }
    return false;
}
