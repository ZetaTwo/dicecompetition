#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <bitset>
#include <map>

using namespace std;

typedef bitset<70> visited;
typedef map<char, vector<pair<string, int> > > graph;

vector<int> bestpath(graph& pokemon, char node, visited state) {
  vector<int> best;
  for (const auto& p : pokemon[node])
  {
    int pokemon_id = p.second;
    if (state.test(pokemon_id)) {
      continue;
    }

    visited newstate(state);
    newstate.set(pokemon_id);

    vector<int> newbest = bestpath(pokemon, p.first.back(), newstate);
    newbest.push_back(pokemon_id);
    if (newbest.size() > best.size()) {
      best = newbest;
    }
  }

  return best;
}

void print_path(const vector<string>& pokedex, const vector<int> path) {
  bool first = true;
  for (auto pid = path.rbegin(); pid != path.rend(); pid++)
  {
    if (first) {
      first = false;
    }
    else {
      cout << " => ";
    }
    cout << pokedex[*pid];
  }
  cout << endl;
}

int main() {

  graph g;
  string pokemon;
  int i = 0;
  vector<string> pokedex;
  while (cin >> pokemon) {
    pokedex.push_back(pokemon);
    g[pokemon.front()].push_back(make_pair(pokemon, i++));
  }

  visited start;
  vector<int> best;
  for (char c = 'a'; c <= 'z'; c++) {
    vector<int> newbest = bestpath(g, c, start);
    if (newbest.size() > best.size()) {
      best = newbest;
    }
  }

  cout << "Length of longest path: " << best.size() << endl;
  print_path(pokedex, best);

  return 0;
}