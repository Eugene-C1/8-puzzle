/*
CS421 Assignment 2

This program solves the 8-Puzzle using the following search algorithms:
Bounded Depth-First Search,
Iterative Deepening Search,
Breadth-First Search,
Best-First Search (A*)

*/
#include <iostream>
#include <list>
#include <ctime>

#include "state.hpp"
#include "io.hpp"

using namespace std;

/*
Goal Configuration
	-------------
	| 1 | 2 | 3 |
	-------------
	| 8 | 0 | 4 |
	-------------
	| 7 | 6 | 5 |
	-------------
*/
int Goal[3][3] = {{1, 2, 3}, {8, 0, 4}, {7, 6, 5}}; //tracks generated nodes in search process
unsigned int nodes_expanded;						//used to track nodes expanded
bool informed_search = false;						//used in state compare operator function
bool bfs_search = false;							//used in Expand function
double runtime;										//tracks runtime of search									//8-Puzzle is a 3X3 matrix

list<State> closed_list, active_list, active_list1;
list<string> path;
State start_state, current_state, temp_state, initial_state;

void PrintPath(State *s);
bool InClosed(State &s, list<State> &closed_list);
void Expand(State *current_state);
void IterativeDepthFirstSearch();
void Astar();

int main()
{

	printInstructions(); //prints instruction

	inputBoard(&start_state); //inputs board

	printBoard(&start_state); //print board

	//initialize start state
	initial_state = start_state;
	start_state.g = 0;										//start at root node
	start_state.heuristics();								//set heuristic value
	start_state.total_cost = start_state.g + start_state.h; //total cost
	start_state.parent = NULL;								//root node

	// running iterative depth first search algorithm
	IterativeDepthFirstSearch();

	//Restart States and lilst
	start_state = initial_state;							//set start_state to initial_state
	active_list.clear();									//empties active_list
	closed_list.clear();									//empties closed_list
	start_state.g = 0;										//start at root node
	start_state.heuristics();								//set heuristic value
	start_state.total_cost = start_state.g + start_state.h; //total cost
	start_state.parent = NULL;								//root node
	informed_search = true;									//set informed search to true

	//running iterative depth first search algorithm
	Astar();

	return 0;
}

/**
 * DESCRIPTION:
 * 	Iterative Depth First Search Implementation. Run a limited-depth first search
 * 	up to a fixed "limted depth". Keep on incrementing the depth limit until we find
 *  the goal node.
 **/
void IterativeDepthFirstSearch()
{

	int depth = 0;
	nodes_expanded = 0;
	clock_t start = clock();
	cout << "------------------------------------\n";
	cout << "Iterative Deepening Search Algorithm\n";
	cout << "------------------------------------\n";
	while (true)
	{
		current_state = start_state;
		active_list.push_front(current_state);
		while (!active_list.empty())
		{

			current_state = active_list.front();

			if (current_state.is_goal())
			{
				// calculate and print the search costs
				runtime = (double)(clock() - start) / CLOCKS_PER_SEC;
				cout << "Path:\n";
				PrintPath(&current_state); //prints solution path

				cout << "\nNodes Expanded= " << nodes_expanded << "\n";
				cout << "Solution Cost = " << current_state.g << "\n";
				cout << "Running Time = " << runtime << "ms\n";
				return;
			}
			else if (depth > current_state.g) //continue searching child nodes if depth not reached
			{
				nodes_expanded++;

				Expand(&current_state);
			}
			else
			{
				active_list.pop_front();
			}
		}
		// clear both lists for the next round
		active_list.clear();
		closed_list.clear();
		// increase the search depth
		depth++;
	}
}

/**
 * DESCRIPTION:
 * 	A Star search finds the least expensive path from a 
 * starting node to a goal node
 **/
void Astar()
{
	nodes_expanded = 0;

	cout << "-----------------------\n";
	cout << "A Star Search Algorithm\n";
	cout << "-----------------------\n";
	current_state = start_state;
	clock_t start = clock();

	active_list.push_front(current_state);
	while (true)
	{

		current_state = active_list.front();
		for (list<State>::iterator it = active_list.begin(); it != active_list.end(); ++it)
		{
			// find state with minimum total cost
			if ((*it) < current_state)
			{
				current_state = (*it);
			}
		}

		if (current_state.is_goal())
		{
			// calculate and print the search costs
			runtime = (double)(clock() - start) / CLOCKS_PER_SEC;
			cout << "Path:\n";
			PrintPath(&current_state); //prints solution path

			cout << "\nNodes Expanded= " << nodes_expanded << "\n";
			cout << "Solution Cost = " << current_state.g << "\n";
			cout << "Running Time = " << runtime << "ms\n";
			return;
		}
		else
		{ //continue searching child nodes
			nodes_expanded++;
			Expand(&current_state);
		}
	}
}

void Expand(State *current_state)
{
	closed_list.push_back(*current_state);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			//find blank tile in state array
			if (current_state->A[i][j] == 0)
			{
				// if not in the first row
				if (i > 0)
				{
					//set child
					temp_state = *current_state;
					temp_state.parent = &(closed_list.back());
					// shift blank tile UP
					swap(temp_state.A[i][j], temp_state.A[i - 1][j]);
					// search for the child in the closed list
					// if the child solutionnot found in the closed list
					if (!InClosed(temp_state, closed_list))
					{
						temp_state.g += 1;
						temp_state.heuristics();
						temp_state.total_cost = temp_state.g + temp_state.h;
						temp_state.move = 0;
						//BFS requires FIFO
						if (bfs_search)
						{
							active_list.push_back(temp_state);
						}
						else
						{
							active_list.push_front(temp_state);
						}
					}
				}
				//if not in last row
				if (i < 2)
				{
					temp_state = *current_state;
					temp_state.parent = &(closed_list.back());
					//shift the blank tile DOWN
					swap(temp_state.A[i][j], temp_state.A[i + 1][j]);
					if (!InClosed(temp_state, closed_list))
					{
						temp_state.g += 1;
						temp_state.heuristics();
						temp_state.total_cost = temp_state.g + temp_state.h;
						temp_state.move = 1;

						//BFS requires FIFO
						if (bfs_search)
						{
							active_list.push_back(temp_state);
						}
						else
						{
							active_list.push_front(temp_state);
						}
					}
				}
				// if not in the first column
				if (j > 0)
				{
					temp_state = *current_state;
					temp_state.parent = &(closed_list.back());
					// shift blank tile LEFT
					swap(temp_state.A[i][j], temp_state.A[i][j - 1]);
					if (!InClosed(temp_state, closed_list))
					{
						temp_state.g += 1;
						temp_state.heuristics();
						temp_state.total_cost = temp_state.g + temp_state.h;
						temp_state.move = 2;

						//BFS requires FIFO
						if (bfs_search)
						{
							active_list.push_back(temp_state);
						}
						else
						{
							active_list.push_front(temp_state);
						}
					}
				} // if not in the last column
				if (j < 2)
				{
					temp_state = *current_state;
					temp_state.parent = &(closed_list.back());
					// shift blank tile RIGHT
					swap(temp_state.A[i][j], temp_state.A[i][j + 1]);
					if (!InClosed(temp_state, closed_list))
					{
						temp_state.g += 1;
						temp_state.heuristics();
						temp_state.total_cost = temp_state.g + temp_state.h;
						temp_state.move = 3;

						//temp = "RIGHT";
						//BFS require FIFO
						if (bfs_search)
						{
							active_list.push_back(temp_state);
						}
						else
						{
							active_list.push_front(temp_state);
						}
					}
				}
			}
		}
	}
	active_list.remove(*current_state);
}
