#include "ImpTable.h"

ImpTable::ImpTable() {
	multimap<int, Minterm*>* column = new multimap<int, Minterm*>;	//free at ~ImpTable()

	table.push_back(column);
}

ImpTable::~ImpTable(){
	multimap<int, Minterm*>::iterator begin;
	multimap<int, Minterm*>::iterator end;
	multimap<int, Minterm*>::iterator currIter, nextIter;

	for (int i = 0; i < table.size(); i++) {
		begin = table[i]->begin();
		end = table[i]->end();

		for (currIter = begin; currIter != end; currIter++) {
			delete currIter->second;
		}
		delete table[i];
	}
}

int ImpTable::readFile(string filepath, vector<string>* tm) {
	ifstream in;
	int len;
	string temp, termStr;
	Minterm* Mintermtmp = 0;

	in.open(filepath);
	if (!in) {
		cout << "Error - Unable to open input_minterm.txt" << endl;
		cout << "The program will be closed." << endl;
		system("pause");
		return -101;
	}

	getline(in, temp);
	len = atoi(temp.c_str());
	lenOfTerm = len;

	while (!in.eof()) {
		getline(in, temp);

		for (int i = 1; ; i++) {
			if (temp[i] != ' ') {
				termStr = temp.substr(i);
				break;
			}
		}

		if (temp[0] == 'm') {
			tm->push_back(termStr);
		}
		else if (temp[0] == 'd') {
			//Nothing
		}
		else {
			return -102;
		}

		Mintermtmp = new Minterm(termStr); //free at ImpTable::~ImpTable()
		table[0]->insert(pair<int, Minterm*>(cntOf1(termStr), Mintermtmp));
	}

	in.close();
	return 0;
}

int ImpTable::combine(int a) {
	int combined = 0;	//if combined once or more, combined=1;
	multimap<int, Minterm*>::iterator begin = table[a]->begin();
	multimap<int, Minterm*>::iterator end = table[a]->end();
	multimap<int, Minterm*>::iterator currIter = begin;	//Init
	
	multimap<int, Minterm*>* newColumn = new multimap<int, Minterm*>;
	table.push_back(newColumn);

	for (;;) {	//one column
		int currKey = currIter->first;

#ifdef DEBUG
		cout << '[' << currKey << ']' << endl;
#endif
		
		multimap<int, Minterm*>::iterator nextIter = table[a]->upper_bound(currKey);

		if (nextIter == end)	break;

		int nextKey = nextIter->first;
		pair<multimap<int, Minterm*>::iterator, multimap<int, Minterm*>::iterator> currIterRange, nextIterRange, nextColRange;
		int dOrder = 0;	//the order of don't care(-)

		string newStr;
		Minterm* newMinterm = 0;

		if (nextKey - currKey == 1) {
			currIterRange = table[a]->equal_range(currKey);
			nextIterRange = table[a]->equal_range(nextKey);

			for (multimap<int, Minterm*>::iterator i = currIterRange.first; i != currIterRange.second; i++) {	//Combining two adjacent groups
				for (multimap<int, Minterm*>::iterator j = nextIterRange.first; j != nextIterRange.second; j++) {
					dOrder = isHD1(i->second->getData(), j->second->getData(), lenOfTerm);
					if (dOrder != -1) {
						newStr = i->second->getData();
						newStr[dOrder] = '-';

						nextColRange = table[a + 1]->equal_range(currKey);
						for (multimap<int, Minterm*>::iterator k = nextColRange.first; k != nextColRange.second; k++) {
							if (newStr == k->second->getData()) {
								newStr = "";	//redundant
							}
						}

						if (newStr != "") {
#ifdef DEBUG
							cout << newStr << endl;
#endif				
							
							newMinterm = new Minterm(newStr);
							table[a + 1]->insert(pair<int, Minterm*>(currKey, newMinterm));
							combined = 1;
						}

						i->second->setMarked(1);	j->second->setMarked(1);
					}
				}
			}
		}

		currIter = nextIter;
	}

#ifdef DEBUG
	cout << '\n' << endl;
#endif

	return combined;
}

vector<string> ImpTable::getPI() {
	vector<string> result;
	multimap<int, Minterm*>::iterator begin;
	multimap<int, Minterm*>::iterator end;
	multimap<int, Minterm*>::iterator currIter, nextIter;

	for (int i = 0; i < table.size(); i++) {
		begin = table[i]->begin();
		end = table[i]->end();

		for (currIter = begin; currIter != end; currIter++) {
			if (currIter->second->getMarked() == 0) {
				result.push_back(currIter->second->getData());
			}
		}
	}

	return result;
}

int cntOf1(string term) {
	int result = 0;

	for (int i = 0; i < term.length(); i++) {
		if (term[i] == '1')	result++;
	}

	return result;
}

int isHD1(string imp1, string imp2, int len) {
	int hammingDistance = 0;
	int order = 0;	//the order of don't care(-)

	for (int i = 0; i < len; i++) {
		if ((imp1[i] == '-') != (imp2[i] == '-')) {
			return -1;
		}
		else if (imp1[i] != imp2[i]) {
			hammingDistance++;
			order = i;
		}
	}

	if (hammingDistance != 1) {
		return -1;
	}

	return order;
}