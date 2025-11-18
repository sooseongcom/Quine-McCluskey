#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "Minterm.h"

#define DEBUG

class ImpTable {
private:
	/**	the type of table
	* vector
	* multimap<int, Minterm*>*: vector 확장 시 multimap을 재생성하므로 dynamic allocation해야 함.
	* Minterm*: 생성 편의를 위해 dynamic allocation해야 함.
	*/
	vector<multimap<int, Minterm*>*> table;
	int lenOfTerm = 0;

public:
	ImpTable();
	~ImpTable();
	int readFile(string filepath, vector<string>* tm);
	int combine(int a);	//Combining variables(Column a to Column a+1). If combined once or more, return 1. If end, return 0.
	vector<string> getPI();
};

int cntOf1(string term);    //# of 1
int isHD1(string imp1, string imp2, int len);	//If hamming distance==1, return the order of don't care(-). If not, return -1.