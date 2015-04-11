#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

#include "knapsack.hpp"
#include "trim.hpp"
using namespace knapsack;

#ifdef _WIN32
#define scanf scanf_s
#endif

struct StationItem {
  string name;
  int cost;
  int importance;
  unsigned int available;
  unsigned int used;
};

struct StationItemSorter {
  bool operator()(const StationItem& a, const StationItem& b) {
    return a.importance > b.importance;
  }
};

int main(int argc, char *argv[]) {
  //Make sure we have a filename
  if (argc <= 1) {
    cout << "Usage: spacestation.exe file" << endl;
    return 0;
  }
  
  vector<StationItem> items;
  ifstream datafile(argv[1], ios::in);
  int total_number_items = 0;
  string line, token;

  //Discard headers
  getline(datafile, line);

  while (getline(datafile, line))
  {
    //Create a new item and get its line
    items.push_back({});
    std::istringstream linestream(line);

    //Get name
    std::getline(linestream, token, ',');
    trim(token);
    items.back().name = token;

    //Get cost
    std::getline(linestream, token, ',');
    trim(token);
    items.back().cost = stoi(token);

    //Get importance
    std::getline(linestream, token, ',');
    trim(token);
    items.back().importance = stoi(token);

    //Get count
    std::getline(linestream, token, ',');
    trim(token);
    items.back().available = stoi(token);
    items.back().used = 0;
    total_number_items += items.back().available;
  }
  
  //Create list of item indices
  vector<knapsack::item> knapsack_items;
  for (size_t item_index = 0; item_index < items.size(); item_index++)
  {
    for (size_t c = 0; c < items[item_index].available; c++) {
      knapsack_items.push_back({ items[item_index].importance, items[item_index].cost, item_index });
    }
  }

  //Solve knapsack
  vector<size_t> result_indices = knapsack::knapsack(10 * 1000, knapsack_items);

  //Tally up resuts
  for (size_t i = 0; i < result_indices.size(); i++)
  {
    items[knapsack_items[result_indices[i]].item_index].used++;
  }

  //Sort by importance
  sort(items.begin(), items.end(), StationItemSorter());

  //Print result
  cout << "Name,Cost ($M) per piece,Importance,Amount used" << endl;
  for (size_t i = 0; i < items.size(); i++) {
    if (items[i].used > 0) {
      cout << items[i].name << "," << items[i].cost << "," << items[i].importance << "," << items[i].used << endl;
    }
  }

  return 0;
}