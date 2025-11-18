#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "ImpTable.h"
#include "PITable.h"
#include "PetrickTable.h"

using namespace std;

int printToFile(string filepath, vector<string> epi);

int main()
{
    string input_path = "input_minterm.txt";
    string output_path = "result.txt";
    ImpTable* imp_table = 0;
    vector<string> tm;    //True minterms
    vector<string> pi;    //Prime Implicants
    int combined = 1;
    PITable* pi_table = 0;
    vector<string> epi;   //Essential PIs
    vector<string> nepi;    //Not essential PIs
    int epiCompleted = 0;
    int inputCompleted = 0;
    int printCompleted = 0;
    PetrickTable* petrick_table = 0;

    //Step 1
    imp_table = new ImpTable;
    inputCompleted = -imp_table->readFile(input_path, &tm);
    cout << "minterms function code: " << inputCompleted << endl;

    //Step 2~3
    for (int i = 0; combined == 1; i++) {
        combined = imp_table->combine(i);
    }
    pi = imp_table->getPI();

    delete imp_table;

    //Step 4
    pi_table = new PITable(pi, tm);

    //Step 5~6
    epiCompleted = pi_table->identifyEPI();
    epi = pi_table->getEPI();
    
    //Step 7
    if (epiCompleted > 0) {
        for (int i = 0; i < pi.size(); i++) {
            int isEPI = 0;

            for (int j = 0; j < epi.size(); j++) {
                if (pi[i] == epi[j]) {
                    isEPI = 1;
                }
            }

            if (isEPI != 1) {
                nepi.push_back(pi[i]);
            }
        }

        petrick_table = new PetrickTable(pi_table, pi, tm);
        petrick_table->petrick();
        petrick_table->minset();

        epi = petrick_table->getEPI();

        delete petrick_table;
    }

    delete pi_table;

    //Print to file
    printCompleted = -printToFile(output_path, epi);
    if (printCompleted == 0) {
        cout << "Essential PIs are printed at " << output_path << endl;
    }
    else {
        cout << "Print error: " << printCompleted << endl;
    }

    return 0;
}

int printToFile(string filepath, vector<string> epi) {
    int numOfEpi = 0, numOfVar = 0, cost = 0;
    ofstream out;

    out.open(filepath);
    if (!out) {
        return -901;
    }

    for (int i = 0; i < epi.size(); i++) {
        out << epi[i] << endl;
        numOfVar = 0;

        for (int j = 0; j < epi[i].length(); j++) {
            if (epi[i][j] == '0') {
                cost += 2;
                numOfVar++;
            }
            else if (epi[i][j] == '1') {
                numOfVar++;
            }
        }

        if (numOfVar > 1) {
            cost += numOfVar * 2;
        }
    }

    out << endl;

    numOfEpi = epi.size();

    if (numOfEpi > 1) {
        cost += numOfEpi * 2;
    }

    out << "Cost (# of transistors): " << cost << endl;

    out.close();
    return 0;
}