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
#include <algorithm>
#include <ctime>

using std::vector;		using std::string;			using std::cin;
using std::cerr;		using std::cout;			using std::endl;
using std::istream;		using std::ostream;			using std::ifstream;
using std::ofstream;	using std::pair;			using std::unordered_map;
using std::make_pair;

//Forward declare
void next_player();
bool is_out_of_bounds(int x, int y);
int square_sides_filled(int x, int y);
bool check_if_scoring(int playerID, int x, int y);
bool make_hor_move(int playerID, int x, int y);
bool make_ver_move(int playerID, int x, int y);
void make_random_move();
void run_game();
void end_game();
void init_game(int nr_players, int nr_rows, int nr_cols);

//Vars
int ROW_SIZE, COL_SIZE;

vector<vector<bool>> hor;
vector<vector<bool>> ver;
vector<vector<bool>> squares;
unordered_map<int, int> playerScore;
vector<int> players;
std::mt19937 mt_rand(std::time(0));
int cur_player;
int filled_squares;

/*
	Change turn to next player
*/
void next_player()
{
	if (players.size()-1 == cur_player)
		cur_player = 0;
	else
		cur_player++;
}

/*
	Check if a coordinate is out of bounds
*/
bool is_out_of_bounds(int x, int y)
{
	if (x < 0 || x >= ROW_SIZE || y < 0 || y >= COL_SIZE)
		return true;
	return false;
}

/*
	Count how many of the sides of a square has been filled
*/
int square_sides_filled(int x, int y)
{
	if (is_out_of_bounds(x, y))
		throw std::out_of_range("Square does not exist");

	int sides_filled = 0;
	if (hor[x][y])
		sides_filled++;
	if (hor[x + 1][y])
		sides_filled++;

	if (ver[x][y])
		sides_filled++;
	if (ver[x][y + 1])
		sides_filled++;

	return sides_filled;
}

/*
	Check if a square is filled and perform actions if it is
*/
bool check_if_scoring(int playerID, int x, int y)
{
	if (is_out_of_bounds(x, y))
		return false;
	if (!squares[x][y] && square_sides_filled(x, y) == 4)
	{
		playerScore[playerID]++;
		squares[x][y] = true;
		filled_squares++;
		return true;
	}
	return false;
}


/*
	Draw a horizontal line
*/
bool make_hor_move(int playerID, int x, int y)
{
	if (x < 0 || x >= ROW_SIZE+1 || y < 0 || y >= COL_SIZE)
		return false;

	if (!hor[x][y])
	{
		hor[x][y] = true;
		check_if_scoring(playerID,x-1, y);
		check_if_scoring(playerID,x, y);
	}
	return true;
}

/*
Draw a vertical line
*/
bool make_ver_move(int playerID, int x, int y)
{
	if (x < 0 || x >= ROW_SIZE || y < 0 || y >= COL_SIZE+1)
		return false;

	if (!ver[x][y])
	{
		ver[x][y] = true;
		check_if_scoring(playerID, x, y-1);
		check_if_scoring(playerID, x, y);
	}
	return true;
}

/*
	Make a completely pseudo-random move
*/
void make_random_move()
{
	bool is_hor_move = (bool) (mt_rand() % 2);
	bool move_made = false;
	while (!move_made)
	if (is_hor_move)
	{
		int x = mt_rand() % (ROW_SIZE + 1);
		int y = mt_rand() % COL_SIZE;
		move_made = make_hor_move(cur_player, x, y);
	}
	else
	{
		int x = mt_rand() % ROW_SIZE;
		int y = mt_rand() % (COL_SIZE+1);
		move_made = make_ver_move(cur_player, x, y);
	}
}

/*
Make a better pseudo-random move (fill square if possible otherwise random)
*/
void make_better_random_move()
{
	for (int x = 0; x < ROW_SIZE; ++x)
	{
		for (int y = 0; y < COL_SIZE; ++y)
		{
			if (square_sides_filled(x, y) == 3)
			{
				if (!ver[x][y])
				{
					make_ver_move(cur_player,x, y);
					return;
				}
				else if (!ver[x][y+1])
				{
					make_ver_move(cur_player, x, y+1);
					return;
				}
				else if (!hor[x][y])
				{
					make_hor_move(cur_player, x, y);
					return;
				}
				else if (!hor[x + 1][y])
				{
					make_hor_move(cur_player, x + 1, y);
					return;
				}
			}
		}
	}
	make_random_move();
}

/*
	Run the game
*/
void run_game()
{

	while (filled_squares != ROW_SIZE*COL_SIZE)
	{
		assert(filled_squares < ROW_SIZE*COL_SIZE);

		if (cur_player == players[0])
			make_better_random_move();
		else
			make_random_move();
		next_player();
	}

	end_game();
}

/*
	End the game
*/
void end_game()
{
	cout << "Game over" << endl;
	
	for (auto i : players)
	{
		cout << "Player " << i << " got score: " << playerScore[i] << endl;
	}
}

/*
	Initialize the game and run it
*/
void init_game(int nr_players, int nr_rows, int nr_cols)
{
	ROW_SIZE = nr_rows;
	COL_SIZE = nr_cols;
	hor.resize(ROW_SIZE + 1, vector<bool>(COL_SIZE, false));
	ver.resize(ROW_SIZE, vector<bool>(COL_SIZE + 1, false));
	squares.resize(ROW_SIZE, vector<bool>(COL_SIZE, false));
	for (int i = 0; i < nr_players; ++i)
		players.push_back(i);

	cur_player = players[0];
	run_game();
}

/*
	The main function initializes the game
*/
int main()
{
	init_game(2,4,4);
	system("pause");

	return EXIT_SUCCESS;
}
