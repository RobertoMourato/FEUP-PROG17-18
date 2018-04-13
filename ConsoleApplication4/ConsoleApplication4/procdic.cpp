#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

bool verifica(bool flag, string palavra);

int main() {
	int dot_counter, counter = 0;
	string palavra, word;
	vector<string> lista;
	bool flag = true;
	ifstream theFile("29765-8.txt");
	ofstream theFile_out("29765-8_w.txt");
	string fileNameInput = "29765-8.txt";
	string fileNameOutput = "29765-8_w.txt";

	cout << "EXTRACTION OF WORD LIST FROM DICTIONARY" << endl;
	cout << "=======================================" << endl;

	cout << "Dictionary file : " << fileNameInput << endl;
	cout << "Words list file : " << fileNameOutput << endl;

	cout << "extracting words..." << endl;

	while (theFile >> palavra) {
		flag = verifica(flag, palavra);

		if (flag) {
			if (palavra[(palavra.size() - 1)] == ';') {
				palavra.erase((palavra.size() - 1), 1);
			}
			lista.push_back(palavra);
		}
	}

	theFile.close();

	sort(lista.begin(), lista.end());

	//This next "for" was supposed to show the letters and the points but somehow there's an error
	/*for (int k = 65; k <= 90; k++) {
		dot_counter = 0;
		word = k;
		for (counter; counter < lista.size(); counter++) {
			if ((lista[counter]).at(0) == word.at(0)) {
				dot_counter++;}
			else if (dot_counter % 100 == 0) {
				cout << ".";}
			else if ((lista[counter]).at(0) != word.at(0)) {
				cout << "\n";
				break;}
		}
	}*/

	cout << "removing non-words..." << endl;

	cout << "number of simple words: " << lista.size() << endl;

	cout << "sorting the words..." << endl;

	cout << "removing duplicated words..." << endl;
	lista.erase(unique(lista.begin(), lista.end()), lista.end());

	cout << "saving words into file..." << endl;
	for (int v = 0; v < lista.size(); v++) {
		theFile_out << lista[v] << endl;
	}

	cout << endl << "number of non-duplicated words: " << lista.size() << endl;

	cout << "end of precessing." << endl;
	return 0;
}

bool verifica(bool flag, string palavra) {
	int i = 0;

	for (i; i < palavra.size(); i++) {
		if (palavra[i] > 64 && palavra[i] < 91) {
			flag = true;
		}
		else if ((palavra[i]) == ';') {
			flag = true;
		}
		else {
			flag = false;
			break;
		}
	}

	return flag;
}