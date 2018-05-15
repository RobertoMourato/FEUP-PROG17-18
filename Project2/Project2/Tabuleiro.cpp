#include "Tabuleiro.h"



Tabuleiro::Tabuleiro()
{
}


Tabuleiro::~Tabuleiro()
{
}

bool Tabuleiro::ResumeBoard() {
	cout << "File Name ?\n";
	string input, line;
	cin >> input;
	ifstream f;
	f.open((input + ".txt").c_str());

	if (!f.is_open()) {

		return false;
		
	}
	else {

		getline(f, line, '\n');
		getline(f, line, '\n');
		
		while (getline(f, line, '\n')) {
			if (line.empty()) {
				break;
			}
			for (int i = 0; i < ; i++) {
				for (int k =0; k < line.


			}

		}
	}



}