#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

bool verifica(bool flag, string palavra);

int main() {
	string palavra;
	vector<string> lista;
	bool flag = true;
	ifstream theFile("29765-8.txt");

	while (theFile >> palavra) {
		flag = verifica(flag, palavra);

		if (flag) {
			lista.push_back(palavra);
			//lista.insert(palavra);
		}
	}

	for (int v = 0; v < lista.size(); v++) {
		cout << lista[v] << endl;
	}
	//dps aqui falta o sort

	return 0;
}

bool verifica(bool flag, string palavra) {
	flag = true;
	int i = 0;

	for (i; i < palavra.size(); i++) {
		if (isupper(palavra.at(i))) {
			flag = true;
		}
		else if (((palavra.at(i)) == ' ') || (palavra.at(i)) == ';' || (palavra.at(i)) == '-' || (palavra.at(i)) == 39){
		flag = true;
}
		else { flag = false;}
	}

	return flag;
}