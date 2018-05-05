#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map> //<-------------------------------

using namespace std;

class Dict {
private:
	//vector < vector <string> > wordSynonyms;
	map <string, vector <string> > wordSynonyms;
	//vector <string> newEmptyVector;
	ifstream f;

public:
	string fileNameInput;

private:
	bool isHeadline(string line)
	{
		int doisPontos, virgula;
		doisPontos = line.find(":");
		virgula = line.find(",");

		if (doisPontos < virgula) return true;
		else return false;
	}

	string singleWord(string& Line) //retirar a proxima palavra da linha
	{
		//check if the first char is a letter
		//and if not cut the chars till we reach a letter, and then, a word
		while (!((Line[0] >= 65 && Line[0] <= 90) || (Line[0] >= 97 && Line[0] <= 122))) {
			Line.erase(0, 1);
		}

		string delimiter = " ,:"; //delimitadores
		string singleWord = Line.substr(0, Line.find(delimiter)); //palavra individual
																  //erase the given words
		Line.erase(0, Line.find(delimiter));


		return singleWord;
	}

	bool validLine(string Line)
	{
		//check if a line is valid by comparing char by char, so lines with [] and {} are ignored
		for (unsigned int i = 0; i < Line.size(); i++)
		{
			if (!((Line[i] >= 65 && Line[i] <= 90) || (Line[i] >= 97 && Line[i] <= 122) || Line[i] == 32 || Line[i] == 58 || Line[i] == 44 || Line[i] == 45))
			{
				return false;
				break;
			}
		}
		return true;
	}

public:
	void extraction()
	{
		string completeLine;
		//int i = -1;

		f.open(fileNameInput);
		if (!f.is_open()) {
			cerr << "File " << fileNameInput << " could not be found!\n";
			system("pause");
			return;
		}

		cout << "extrating the words and synonyms...\n";

		//first line, except lines with "[ (...) ]"
		do {
			getline(f, completeLine);
		} while (!validLine(completeLine));

		//line and headline analysis
		while (!f.eof()) {
			string headline;
			vector <string> synonyms;
			while (completeLine != "\0") {
				if (isHeadline(completeLine)) {
					//i++;
					//wordSynonyms.push_back(newEmptyVector);
					//wordSynonyms[i].push_back(singleWord(completeLine));
					headline = singleWord(completeLine);
				}
				else
					//wordSynonyms[i].push_back(singleWord(completeLine)); 
					synonyms.push_back(singleWord(completeLine));
			}

			wordSynonyms[headline] = synonyms;

			//next line
			do {
				getline(f, completeLine);
			} while (!validLine(completeLine));
		}
		cout << "end of extraction\n";
		f.close();
	}

};

/*
class Board {
private:
unsigned int lines, columns;

void show() {

}

void addWord(...) {

}

void removeWord(...) {

}

};


*/
void puzzleCreate()
{
	Dict dict;
	cout << "Dictionary file name ? "; cin >> dict.fileNameInput;
	dict.extraction();
}


int main()
{
	//program layout
	cout << "         CROSSWORD PUZZLE CREATOR\n";
	cout << "==========================================\n\n";
	cout << "INSTRUCTIONS:\n\n";

	cout << "Position ( LCD / CTRL-Z = stop )\n";
	cout << "  LCD stands for Line Column and Direction\n";
	cout << "    Line and Column represented by its name\n";
	cout << "      upper case for lines\n";
	cout << "      lower case for columns\n";
	cout << "    Direction can be vertical (V) or horizontal (H)\n\n";
	cout << "Word ( - = remove / ? = help )\n";
	cout << "  there are several options here:\n";
	cout << "    typing a word to be added to the board\n";
	cout << "      - to remove a word from the crossword board\n";
	cout << "      ? gives you a list of possible words to add to the board\n\n";
	while (true) {
		cout << "-----------------------------------------------------\n\n";
		cout << "OPTIONS:\n";
		cout << " 1 - Create puzzle\n";
		cout << " 2 - Resume puzzle\n";
		cout << " 0 - Exit\n\n";
		//error-proof input 
		string chosenOption;
		while (true) {
			cout << "Option ? ";
			cin >> chosenOption;
			if ("0" == chosenOption) return 0;
			if ("1" == chosenOption) {
				puzzleCreate();
				break;
			}
			//if ("2" == chosenOption) { puzzleResume(); break; }
		}
	}
}
