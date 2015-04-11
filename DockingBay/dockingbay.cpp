#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

const int SIZE = 8;

bool valid_board(const vector<int>& placements) {
  const size_t S = placements.size();
  
  //Create empty board
  vector<vector<bool>> board(S, vector<bool>(S, false));
  
  //Place ships
  for (size_t row = 0; row < S; row++)
  {
    //Can we place?
    if (board[row][placements[row]]) {
      return false;
    }

    //Mark board
    for (size_t i = 0; i < S; i++)
    {
      board[i][placements[row]] = true;
      board[row][i] = true;

      //Mark \ diagonal
      size_t diag_col = placements[row] - row + i;
      if (diag_col >= 0 && diag_col < S) {
        board[i][diag_col] = true;
      }

      //Mark / diagonal
      diag_col = placements[row] + row - i;
      if (diag_col >= 0 && diag_col < S) {
        board[i][diag_col] = true;
      }
    }
  }
  
  return true;
}

void print_board(const vector<int>& placements) {
  const int S = static_cast<int>(placements.size());
  for (int row = 0; row < S; row++)
  {
    for (int pre = 0; pre < placements[row]; pre++) {
      cout << "O";
    }
    cout << "#";
    for (int pre = placements[row]+1; pre < S; pre++) {
      cout << "O";
    }
    cout << endl;
  }
}

int main() {
  //Create default permutation
  vector<int> placements;
  for (size_t i = 0; i < SIZE; i++)
  {
    placements.push_back(i);
  }
  //vector<int> placements = { 5, 3, 6, 0, 7, 1, 4, 2 };
  //vector<int> placements = { 0, 2, 5, 7, 6, 3, 1, 4 };

  //int num_solutions = 0;
  do {
    if (valid_board(placements)) {
      print_board(placements);
      break;
      //num_solutions++;
    }
  } while (next_permutation(placements.begin(), placements.end()));

  //cout << num_solutions << endl;

  return 0;
}