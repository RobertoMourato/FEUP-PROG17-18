#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

bool verifica(bool flag, string palavra);

int main(){
	string palavra;
	vector<string> lista;
	bool flag;
	ifstream theFile("29765-8.txt");

	while (theFile >> palavra) {
		verifica(flag, palavra);

		if (flag) {
			lista.insert(palavra);
		}
	}

	//dps aqui falta o sort

    return 0;
}

bool verifica(bool flag, string palavra) {
	bool flag=true;
	int i = 0;

	//falta ainda separa as palavras ligadas por ";"
	
	for (i; i < palavra.length; i++) {
		if (isupper (palavra.at(i))) {
			flag = true;
		}
		else { flag = false;}
	}

	return flag;
}