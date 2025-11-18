#pragma once
#include <string>
#include <vector>
#include <set>
#include "Minterm.h"
#include "PITable.h"

//#define DEBUG
#ifdef DEBUG
#include <iostream>
#endif

class PetrickTable {
private:
	vector<vector<int>> table;
	vector<string> tm;    //True minterms
	vector<string> pi;    //Prime Implicants
	vector<string> epi;	//Essential PI
	vector<int*> pterms;
	int lenOfTerm = 0;
public:
	PetrickTable(PITable* pi_table, vector<string> inpPI, vector<string> inpTM);
	~PetrickTable();
	int petrick();
	int findAllSet(vector<set<int*>>* pfunc, int* perm, int currDepth);
	int minset();
	int cost(int* pterms);	//Count # of transistor
	vector<string> getEPI();
};