#include "PetrickTable.h"

PetrickTable::PetrickTable(PITable* pi_table, vector<string> inpPI, vector<string> inpTM) {
	/**
	* Row: PIs
	* Column: true minterms
	*
	* -1: deleted
	* 0: blank
	* 1: PI covers the minterm
	* 2: only 1 in the column
	*/

	int isEPI = 0;

	tm = inpTM;
	epi = pi_table->getEPI();
	lenOfTerm = inpPI[0].length();

	//Remove essential PI from pi
	for (int i = 0; i < inpPI.size(); i++) {
		isEPI = 0;

		for (int j = 0; j < epi.size(); j++) {
			if (inpPI[i] == epi[j]) {
				isEPI = 1;
			}
		}

		if (isEPI != 1) {
			pi.push_back(inpPI[i]);
		}
	}

	table.resize(pi.size());
	for (int i = 0; i < pi.size(); i++) {
		table[i].resize(tm.size());
	}

	//Input pi to PI table
	for (int i = 0; i < pi.size(); i++) {
		for (int j = 0; j < inpPI.size(); j++) {
			if (pi[i] == inpPI[j]) {
				table[i] = pi_table->getTable()[j];
			}
		}
	}

#ifdef DEBUG
	cout << "[PI Table for Petrick's Method]" << endl;
	cout << '\t';
	for (int i = 0; i < tm.size(); i++) {
		cout << tm[i] << '\t';
	}
	cout << endl;

	for (int i = 0; i < pi.size(); i++) {
		cout << endl
			<< pi[i] << '\t';

		for (int j = 0; j < tm.size(); j++) {
			cout << table[i][j] << '\t';
		}
	}

	cout << endl;
#endif
}

PetrickTable::~PetrickTable() {
	for (int i = 0; i < pterms.size(); i++) {
		delete[] pterms[i];
	}
}

int PetrickTable::petrick() {
	vector<set<int*>> pfunc;
	set<int*> pset;
	int* p = 0;
	int vcnt = 0;
	int doPush = 0;

	p = new int[table.size()];
	for (int i = 0; i < table.size(); i++) {
		p[i] = 0;
	}

	for (int i = 0; i < tm.size(); i++) {	//i: row(true minterm)
		pset.clear();	doPush = 0;

		for (int j = 0; j < pi.size(); j++) {	//j: column(PI)
			if (table[j][i] == 1) {
				pset.insert(p + j);
				doPush = 1;
			}
		}

		if (doPush) {
			pfunc.push_back(pset);
		}
	}

	findAllSet(&pfunc, p, 0);

	delete[] p;
	return -801;
}

int PetrickTable::findAllSet(vector<set<int*>>* pfunc, int* perm, int currDepth) {
	int permLen = table.size();
	int* pterm = 0;
	int setfound = 0;
	int cover = 0;

	if (currDepth == table.size()) {
		for (int i = 0; i < pfunc->size(); i++) {
			setfound = 0;

			for (set<int*>::iterator it = (*pfunc)[i].begin(); it != (*pfunc)[i].end(); it++) {
				if (**it == 1) {
					setfound = 1;
				}
			}

			if (setfound == 1)	cover++;
		}

		if (cover == pfunc->size()) {
			pterm = new int[pi.size()];	//free at PetrickTable::~PetrickTable()

			for (int i = 0; i < pi.size(); i++) {
				pterm[i] = perm[i];
			}

			pterms.push_back(pterm);
		}

		return 0;
	}

	for (int i = 0; i < 2; i++) {
		perm[currDepth] = i;
		findAllSet(pfunc, perm, currDepth + 1);
	}
}

int PetrickTable::minset() {
	vector<int*> minimums;
	int num1 = 0, num2 = 0;	//# of 1
	vector<string> epi1, epi2;
	int cost1 = 0, cost2 = 0;

	minimums.push_back(pterms[0]);
	for (int i = 0; i < pi.size(); i++) {
		if (minimums[0][i] == 1)	num1++;
	}

	for (int i = 1; i < pterms.size(); i++) {
		for (int j = 0; j < pi.size(); j++) {
			if (pterms[i][j] == 1)	num2++;
		}

		if (num1 > num2) {
			minimums.clear();
			minimums.push_back(pterms[i]);
		}
		else if (num1 == num2) {
			minimums.push_back(pterms[i]);
		}
	}

	if (minimums.size() == 1) {
		for (int i = 0; i < pi.size(); i++) {
			if (minimums[0][i] == 1) {
				epi.push_back(pi[i]);
			}
		}
	}
	else {
		for (int i = 0; i < minimums.size(); i++) {
			cost1 = cost(minimums[i]);

			if (cost1 < cost2) {
				epi2 = epi1;
				cost2 = cost1;
			}
		}

		epi.insert(epi.end(), epi2.begin(), epi2.end());
	}

	return 0;
}

int PetrickTable::cost(int* pterms) {
	vector<string> epiTemp;
	int numOfEpi = 0, numOfVar = 0;
	int costResult = 0;

	for (int i = 0; i < pi.size(); i++) {
		if (pterms[i] == 1) {
			epiTemp.push_back(pi[i]);
		}
	}

	for (int i = 0; i < epiTemp.size(); i++) {
		numOfVar = 0;

		for (int j = 0; j < epi[i].length(); j++) {
			if (epi[i][j] == '0') {
				costResult += 2;
				numOfVar++;
			}
			else if (epi[i][j] == '1') {
				numOfVar++;
			}
		}

		if (numOfVar > 1) {
			costResult += numOfVar * 2;
		}
	}

	numOfEpi = epiTemp.size();

	if (numOfEpi > 1) {
		costResult += numOfEpi * 2;
	}

	return costResult;
}

vector<string> PetrickTable::getEPI() {
	return epi;
}