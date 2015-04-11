#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

#include "trim.hpp"

struct record {
  long id;
  double diameter;
  string line;
};

struct record_sorter {
  bool operator()(const record& a, const record& b) {
    return a.diameter > b.diameter;
  }
};

ifstream open_datafile(const string& filename, bool header = false) {
  ifstream datafile(filename);

  //Discard header
  if (header) {
    string header;
    getline(datafile, header);
  }

  return datafile;
}

ofstream open_outfile(const string& filename) {
  ofstream datafile(filename);
  return datafile;
}

record read_record(ifstream& datafile, bool full = true) {
  record r;
  string line, token;
  getline(datafile, line);
  r.line = line;
  std::istringstream linestream(line);

  //Get id
  getline(linestream, token, ',');
  if (!datafile.good()) return r;

  trim(token);
  r.id = stol(token);

  //If full file, skip 20 columns
  if (full) {
    for (size_t i = 0; i < 20; i++)
    {
      getline(linestream, token, ',');
    }
  }

  //Get diameter
  getline(linestream, token, ',');
  trim(token);
  if (token != "") {
    try {
      r.diameter = stod(token);
    }
    catch (...) {
      r.diameter = 0;
    }
  }
  else {
    r.diameter = 0;
  }

  return r;
}

void write_record(ofstream& datafile, const record& r) {
  datafile << r.line << endl;
}

void merge_files(string infilename1, string infilename2, string outfilename) {
  ifstream infile1 = open_datafile(infilename1);
  ifstream infile2 = open_datafile(infilename2);
  ofstream outfile = open_outfile(outfilename);

  //Get first records
  record left, right;
  left = read_record(infile1);
  right = read_record(infile2);

  //Compare and write largest
  while (infile1.good() && infile2.good()) {
    if (left.diameter > right.diameter) {
      write_record(outfile, left);
      left = read_record(infile1);
    }
    else {
      write_record(outfile, right);
      right = read_record(infile2);
    }
  }

  //Write remaining of left
  while (infile1.good()) {
    write_record(outfile, left);
    left = read_record(infile1);
  }

  //Write remaining of right
  while (infile2.good()) {
    write_record(outfile, right);
    right = read_record(infile2);
  }

  outfile.flush();
  outfile.close();
  infile1.close();
  infile2.close();
}

string create_filename(int index) {
  stringstream namestream;
  namestream << "data/part_" << index << ".csv";
  return namestream.str();
}


void merge(int index) {
  merge_files(
    create_filename(2*index+1),
    create_filename(2*index+2),
    create_filename(index)
  );
}

void mergesort_small(ifstream& datafile, int index) {
  vector<record> records;
  record r = read_record(datafile);
  while (datafile.good() && records.size() < 999) {
    records.push_back(r);
    r = read_record(datafile);
  }
  if (datafile.good()) {
    records.push_back(r);
  }

  //Sort
  sort(records.begin(), records.end(), record_sorter());

  ofstream outfile = open_outfile(create_filename(index));
  for (const record& r : records) {
    outfile << r.line << endl;
  }
}


int main() {

  ifstream datafile = open_datafile("Test/results.csv", true);
  
  int parts = 0;
  while (datafile.good()) {
    mergesort_small(datafile, parts++);
  }
  //int parts = 682;

  int i = 0;
  while (i < parts) {
    cout << "Merging part " << i << endl;
    merge_files(create_filename(i), create_filename(i + 1), create_filename(parts));
    i += 2;
    parts += 1;
  }
  rename(create_filename(parts-1).c_str(), "Test/results_sorted.csv");

  return 0;
}