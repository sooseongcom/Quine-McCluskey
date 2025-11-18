#include "Minterm.h"

Minterm::Minterm(string inp) {
	marked = 0;	//0: blank or stared, 1: marked.
	data = inp;
}

Minterm::~Minterm() {

}

int Minterm::setMarked(int mark) {
	marked = mark;
	return 0;
}

string Minterm::getData() {
	return data;
}

int Minterm::getMarked() {
	return marked;
}