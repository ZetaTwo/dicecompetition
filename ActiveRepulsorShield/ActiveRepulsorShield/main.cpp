/*
*	This class solves the Anagramatic problem by using a map
*
*/

#include <iostream> 
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <exception>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <climits>
#include <random>
#include<ctime>
#include <algorithm>

using std::vector;		using std::string;			using std::cin;
using std::cerr;		using std::cout;			using std::endl;
using std::istream;		using std::ostream;			using std::ifstream;
using std::ofstream;	using std::pair;			using std::unordered_map;
using std::make_pair;

std::mt19937 mt_rand((unsigned int) std::time(0));
vector<vector<bool>> matrix(3, vector<bool>(5, false));

const int ROWS = 3;
const int COLS = 5;
vector<int> players = { 0, 1 };
int curPlayer = 1;

int filled_spaces = 0;

/*
	Reinitalize variables
*/
void clear()
{
	matrix.clear();
	matrix.resize(3, vector<bool>(5, false));
	filled_spaces = 0;
	curPlayer = 1;
}

/*
	Mark all nodes to the right and down
*/
void mark_nodes(int x, int y)
{
	if (x >= matrix.size() || y >= matrix[x].size() || x < 0 || y < 0 || matrix[x][y])
		return;
	filled_spaces++;
	matrix[x][y] = true;
	mark_nodes(x + 1, y);
	mark_nodes(x, y+1);
}

/*
	Make a move
*/
bool make_move(int x, int y)
{
	if (!matrix[x][y])
	{
		mark_nodes(x, y);
		return true;
	}
	return false;
}

/*
	Make a random move
*/
void make_random_move()
{
	bool move_made = false;
	while (!move_made)
	{
		int row = mt_rand() % ROWS;
		int col = mt_rand() % COLS;
		if (make_move(row, col))
			move_made = true;
	}
}

/*
Make a better random move
*/
void make_better_move()
{
	bool move_made = false;
	while (!move_made)
	{
		int row = mt_rand() % ROWS;
		int col = mt_rand() % COLS;
		if (row == 0 && col == 0 && filled_spaces != (ROWS*COLS-1))
			continue;
		if (make_move(row, col))
			move_made = true;
	}
}


void next_player()
{
	if (curPlayer == 0)
		curPlayer = 1;
	else
		curPlayer = 0;
}

void run()
{
	int player0wins = 0, player1wins = 0;
	for (int i = 0; i < 10000; ++i)
	{
		while (!matrix[0][0])
		{
			int curmarked = filled_spaces;
			next_player();
			if (curPlayer == 0)
				make_better_move();
			else
				make_random_move();
		}
		if (curPlayer == 0)
			player1wins++;
		else
			player0wins++;

		clear();

	}
	cout << "Player 0 won " << player0wins << " times. Player 1 won " << player1wins << " times." << endl;
}


int main()
{
	run();
	system("pause");

	return EXIT_SUCCESS;
}
