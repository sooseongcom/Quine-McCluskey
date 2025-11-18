#pragma once
#include <string>

using namespace std;

class Minterm {
private:
	string data;
	int marked = 0;	//0: blank or stared, 1: marked.

public:
	Minterm(string inp);
	~Minterm();
	int setMarked(int mark);
	string getData();
	int getMarked();
};