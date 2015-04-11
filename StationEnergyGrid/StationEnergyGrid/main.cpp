/*
*	This class implements the Station Energy Grid game (generalized solution!)
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

//Forward declarations
void next_player();
bool is_out_of_bounds(int x, int y);
int square_sides_filled(int x, int y);
bool check_if_scoring(int playerID, int x, int y);
bool make_hor_move(int playerID, int x, int y);
bool make_ver_move(int playerID, int x, int y);
void make_random_move(bool&);
void make_better_random_move(bool&);
void run_game();
void end_game();
void init_game(int nr_players, int nr_rows, int nr_cols);

//Consts
const int MATCHES_TO_PLAY = 1000;

//Vars
vector<vector<bool>> hor, ver, squares;
unordered_map<int, int> playerScore;
unordered_map<int, int> playerWins;
vector<int> players;
std::mt19937 mt_rand((unsigned int)std::time(0));
int cur_player, better_player, filled_squares, ROW_SIZE, COL_SIZE;
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
	Check if a coordinate is out of bounds for the squares
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
bool make_hor_move(int playerID, int x, int y, bool& scored)
{
	if (x < 0 || x >= ROW_SIZE+1 || y < 0 || y >= COL_SIZE)
		return false;

	if (!hor[x][y])
	{
		hor[x][y] = true;
		if(check_if_scoring(playerID,x-1, y))
			scored = true;
		if(check_if_scoring(playerID,x, y))
			scored = true;
	}
	return true;
}

/*
	Draw a vertical line
*/
bool make_ver_move(int playerID, int x, int y, bool& scored)
{
	if (x < 0 || x >= ROW_SIZE || y < 0 || y >= COL_SIZE+1)
		return false;

	if (!ver[x][y])
	{
		ver[x][y] = true;
		if (check_if_scoring(playerID, x, y - 1))
			scored = true;
		if (check_if_scoring(playerID, x, y))
			scored = true;	
	}
	return true;
}

/*
	Make a completely pseudo-random move
*/
void make_random_move(bool& scored)
{
	bool is_hor_move = (bool) (mt_rand() % 2);
	bool move_made = false;
	while (!move_made)
	{
		if (is_hor_move)
		{
			int x = mt_rand() % (ROW_SIZE + 1);
			int y = mt_rand() % COL_SIZE;
			move_made = make_hor_move(cur_player, x, y,scored);
		}
		else
		{
			int x = mt_rand() % ROW_SIZE;
			int y = mt_rand() % (COL_SIZE + 1);
			move_made = make_ver_move(cur_player, x, y,scored);
		}
	}
}

/*
	Make a better pseudo-random move (fill square if possible otherwise random)
*/
void make_better_random_move(bool& scored)
{
	for (int x = 0; x < ROW_SIZE; ++x)
	{
		for (int y = 0; y < COL_SIZE; ++y)
		{
			if (square_sides_filled(x, y) == 3)
			{
				if (!ver[x][y])
				{
					make_ver_move(cur_player, x, y, scored);
					return;
				}
				else if (!ver[x][y+1])
				{
					make_ver_move(cur_player, x, y + 1, scored);
					return;
				}
				else if (!hor[x][y])
				{
					make_hor_move(cur_player, x, y, scored);
					return;
				}
				else if (!hor[x + 1][y])
				{
					make_hor_move(cur_player, x + 1, y, scored);
					return;
				}
			}
		}
	}

	make_random_move(scored);
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
	if (players.size() != nr_players)
	{
		for (int i = 0; i < nr_players; ++i)
		{
			players.push_back(i);
			if (playerWins.find(i) == playerWins.end())
				playerWins[i] = 0;
		}
	}
	cur_player = players[0];
	better_player = players[0];
	run_game();
}

/*
	Run the game by letting each player make a move
*/
void run_game()
{
	while (filled_squares != ROW_SIZE*COL_SIZE)
	{
		assert(filled_squares < ROW_SIZE*COL_SIZE);
		bool scored = false;
		if (cur_player == 234)
			make_better_random_move(scored);
		else
			make_random_move(scored);
		if (!scored)
			next_player();
	}

	end_game();

}


/*
End the game
*/
void end_game()
{
	//cout << "Game over" << endl;
	int curScore = -1;
	int winner = -1;
	for (auto i : players)
	{
		if (playerScore[i] > curScore)
		{
			winner = i;
			curScore = playerScore[i];
		}
		//cout << "Player " << i << " has score: " << playerScore[i] << endl;
	}

	playerWins[winner]++;
	hor.clear();
	ver.clear();
	squares.clear();
	playerScore.clear();
	filled_squares = 0;
}

/*
	The main function initializes the game
*/
int main()
{
	cout << "Playing " << MATCHES_TO_PLAY << " matches!" << endl;
	for (int iterations = 0; iterations < MATCHES_TO_PLAY; ++iterations)
	{
		init_game(4, 10, 10); // Num players, Num rows, num cols
	}

	for (auto i : players)
	{
		cout << "Player " << i << " won "<< playerWins[i] << " games and got an average score of: " << double(playerScore[i])/double(MATCHES_TO_PLAY) << endl;
	}
	system("pause");

	return EXIT_SUCCESS;
}
