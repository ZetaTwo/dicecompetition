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
#include <algorithm>

using std::vector;		using std::string;			using std::cin;
using std::cerr;		using std::cout;			using std::endl;
using std::istream;		using std::ostream;			using std::ifstream;
using std::ofstream;	using std::pair;			using std::unordered_map;
using std::make_pair;

vector<string> strings;
unordered_map<string, vector<pair<string, bool>>> string_map;

// Sort based on the length of a string
struct length_compare {
	bool operator()(const string& first, const string& second) {
		return first.size() > second.size();
	}
};

/*
Add each string to the map.
*/
void add_to_map(string& st)
{
	string sorted_st = st;
	std::sort(sorted_st.begin(), sorted_st.end()); // Sort string to find/build map

	if (string_map[sorted_st].size() == 1) //1 element means  first collision has been found
		string_map[sorted_st].back().second = true;

	if (!string_map[sorted_st].empty()) //Not empty means current string has been found
		string_map[sorted_st].push_back(make_pair(st, true));
	else
		string_map[sorted_st].push_back(make_pair(st, false));
}

/*
Sort the anagrams according to letter length
*/
void sort_results(ostream & out)
{
	vector<string> sorted_strings;

	for (auto v : string_map)
	{
		for (auto p : v.second)
			if (p.second)
				sorted_strings.push_back(p.first);
	}

	const length_compare comp;
	sort(sorted_strings.begin(), sorted_strings.end(), comp);
	for (auto s : sorted_strings)
		out << s << endl;
}

/*
Reads the problem from instream and prints the solutions to outstream.
*/
void run(istream &instream, ostream &outstream)
{
	string S;
	while (instream >> S)
	{
		strings.push_back(S);
	}

	for (auto p : strings)
		add_to_map(p);
	sort_results(outstream);
}

/*
The main function runs the program. Reads from unixdict.in and writes to output.out
*/
int main()
{
	ofstream out("output.out");
	ifstream in("unixdict.txt");
	if (out.fail() || in.fail())
		cout << "FAILED IN READING STREAMS" << endl;

	while (!out.fail() && !in.fail())
	{
		run(in, out);
		out.close(); in.close();
	}
	system("pause");

	return EXIT_SUCCESS;
}