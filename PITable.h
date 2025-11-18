#pragma once
#include <string>
#include <vector>
#include <set>
#include "Minterm.h"

//#define DEBUG
#ifdef DEBUG
#include <iostream>
#endif

class PITable {
private:
	vector<vector<int>> table;
	vector<string> tm;    //True minterms
	vector<string> pi;    //Prime Implicants
	vector<string> epi;	//Essential PI
	int lenOfTerm = 0;
public:
	PITable(vector<string> inpPI, vector<string> inpTM);
	~PITable();
	int identifyEPI();	//Identify essential PIs.
	vector<vector<int>> getTable();
	vector<string> getEPI();
};

int piCoverTm(string inpPI, string inpTM, int length);