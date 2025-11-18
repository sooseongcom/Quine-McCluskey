#include "PITable.h"

PITable::PITable(vector<string> inpPI, vector<string> inpTM) {
	/**
	* Row: PIs
	* Column: true minterms
	* 
	* -1: deleted
	* 0: blank
	* 1: PI covers the minterm
	* 2: only 1 in the column
	*/

	pi = inpPI;
	tm = inpTM;
	lenOfTerm = pi[0].length();

	table.resize(pi.size());
	for (int i = 0; i < pi.size(); i++) {
		table[i].resize(tm.size());
	}

	//Input pi to PI table
	for (int i = 0; i < pi.size(); i++) {
		for (int j = 0; j < tm.size(); j++) {
			table[i][j] = piCoverTm(pi[i], tm[j], lenOfTerm);
		}
	}

#ifdef DEBUG
	cout << "[PI Table]" << endl;
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

PITable::~PITable(){}

int PITable::identifyEPI() {
	int cnt = 0;	//# of 1
	int complete = 0;	//# of column cnt==1
	int epiExist = 0;	//Is that epi already exist?

	for (int i = 0; i < tm.size(); i++) {	//i: row(true minterm)
		cnt = 0;

		for (int j = 0; j < pi.size(); j++) {	//j: column(PI)
			if (table[j][i] == 1) {
				cnt++;
			}
		}
		
		if (cnt == 1) {
			complete++;

			for (int j = 0; j < pi.size(); j++) {	//j: column(PI)
				if (table[j][i] == 1) {
					table[j][i] = 2;
					epiExist = 0;

					for (int k = 0; k < epi.size(); k++) {
						if (epi[k] == pi[j])	epiExist = 1;	//redundant
					}
					if(epiExist!=1)	epi.push_back(pi[j]);	//not redundant

					for (int k = 0; k < i; k++) {	//k: row(true minterm)
						if (table[j][k] > 0) {
							complete++;
							if (table[j][k] != 2)	table[j][k] = -1;

							for (int l = 0; l < pi.size(); l++) {	//l: column(PI)
								if (table[l][k] != 2)	table[l][k] = -1;
							}
						}
					}
				}
				else {
					table[j][i] = -1;
				}
			}
		}
	}

#ifdef DEBUG
	cout << "[PI Table]" << endl;
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

	return tm.size() - complete;
}

vector<vector<int>> PITable::getTable() {
	return table;
}

vector<string> PITable::getEPI() {
	return epi;
}

int piCoverTm(string inpPI, string inpTM, int length) {
	for (int i = 0; i < length; i++) {
		if (inpPI[i] != '-' && inpPI[i] != inpTM[i]) {
			return 0;
		}
	}

	return 1;
}