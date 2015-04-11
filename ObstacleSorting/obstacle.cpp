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

ifstream open_datafile(const string& filename) {
  ifstream datafile(filename);

  //Discard header
  //string header;
  //getline(datafile, header);

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
    r.diameter = stod(token);
  }
  else {
    r.diameter = 0;
  }

  return r;
}

void write_record(ofstream& datafile, const record& r) {
  datafile << r.line << endl;
}

void split_file(string infilename, string outfilename1, string outfilename2, long rows) {
  ifstream infile = open_datafile(infilename);
  ofstream outfile1 = open_outfile(outfilename1);
  ofstream outfile2 = open_outfile(outfilename2);

  //Write first [rows] lines to out file 1
  string line;
  for (long i = 0; i < rows; i++)
  {
    getline(infile, line);
    outfile1 << line << endl;
  }

  //Write rest of lines to out file 2
  while (getline(infile, line)) {
    outfile2 << line << endl;
  }
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
}

string create_filename(int index) {
  stringstream namestream;
  namestream << "data/part_" << index << ".csv";
  return namestream.str();
}

void split(int index, int rows) {

  split_file(
    create_filename(index),
    create_filename(2 * index+1),
    create_filename(2 * index+2), 
    rows/2
  );
}

void merge(int index) {
  merge_files(
    create_filename(2*index+1),
    create_filename(2*index+2),
    create_filename(index)
  );
}

void mergesort_small(int index) {
  vector<record> records;
  ifstream datafile = open_datafile(create_filename(index));
  record r = read_record(datafile);
  while (datafile.good()) {
    records.push_back(r);
  }
  datafile.close();

  //Sort
  sort(records.begin(), records.end(), record_sorter());

  ofstream outfile = open_outfile(create_filename(index));
  for (const record& r : records) {
    outfile << r.line << endl;
  }
}

void mergesort(int index, int rows) {
  if (rows == 1) {
    return;
  }

  if (rows > 1000) {
    mergesort_small(index);
    return;
  }

  if (rows > 1) {
    split(index, rows);
  }
  if (rows > 2) {
    mergesort(2*index+1, rows/2);
    mergesort(2 * index + 2, rows - (rows/2));
  }
  merge(index);
}

int main() {

  ifstream datafile = open_datafile("Test/results.csv");
  ofstream sortfile = open_outfile(create_filename(0));
  string line;
  int rows = 0;
  while (getline(datafile, line)) {
    rows++;
    if (rows > 1) {
      sortfile << line << endl;
    }
  }
  sortfile.close();
  datafile.close();

  mergesort(0, rows-1);

  datafile = open_datafile(create_filename(0));
  sortfile = open_outfile("Test/results_sorted.csv");
  sortfile << "spkid,full_name,neo,pha,spec_B,moid,full_name,a,e,i,om,w,q,ad,per_y,data_arc,condition_code,n_obs_used,n_del_obs_used,n_dop_obs_used,H,diameter,extent,albedo,rot_per,GM,BV,UB,IR,spec_B,spec_T" << endl;
  while (getline(datafile, line)) {
    sortfile << line << endl;
  }

  return 0;
}