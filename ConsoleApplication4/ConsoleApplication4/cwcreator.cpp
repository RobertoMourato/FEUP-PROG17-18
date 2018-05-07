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
			if (!((Line[i] >= 65 && Line[i] <= 90) || (Line[i] >= 97 && Line[i] <= 122) || Line[i] == 32 || Line[i] == 58 || Line[i] == 44 || Line[i]==45))
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

	bool headlineExists(string word)
	{
		return true;
	}

};


class Board {
private:
	unsigned int lines, columns;
	vector <vector <char> > layout;
	vector <char> newEmpty;

public:
	/*
	FAZER LINHAS E COLUNAS +1, PARA QUE OS IDENTIFICADORES DAS LINHAS ESTEJAM DENTRO DO VECTOR?
	OU NOS LOOP QUE FIZERMOS, CONTANDO DO INICIO E NA PRIMEIRA LINHA, METER OS EIXOS "A PARTE" DO VECTOR E DO LOOP DO VECTOR?
	*/

	// from here
	Board()      
	{
		lines = 10;  //default size
		columns = 10; //default size
	}//constructor

	Board(int lines, int columns) 
	{
		this->lines = lines; 
		this->columns = columns;
	}	//constructor with arguments					
	// to here

	void pointFill()
	{
		//fill the board with points
		char point = '.';
		//	layout:	outer vector columns
		//			inner vector lines
		for (unsigned int i = 0; i < columns; i++) {
			layout.push_back(newEmpty);
			for (unsigned int j = 0; j < lines; j++) {
				layout[i].push_back(point);
			}
		}
	}

	void show() {
		for (unsigned int j = 0; j < lines; j++) {
			for (unsigned int i = 0; i < columns; i++) {
				cout << " " << layout[i][j];
			}
			cout << endl;
		}
	}

	void addWord(...) {

	}

	void removeWord(...) {

	}

	bool clearSpace4Word(int wSize, int lineNum, int columnNum, char direction)
	{
		if ('V' == direction || 'v' == direction) {
			if (0 == lineNum) {
				for (unsigned int i = 0; i < wSize )
			}
			else {

			}
		}
		else if ('H' == direction || 'h' == direction)
		{
			if (0 == columnNum)
			{

			}
			else {

			}
		}
	}

};

int whichLine(string position)
{
	//first char of position
	char l = position[0];
	//char - A = position on vector (A-A=0, 1st line, position 0 on the vector)
	int lineNum = l - 65;
	return lineNum;
}

int whichColumn(string position)
{
	//second char of position
	char c = position[1];
	//char - a = position on vector
	int columnNum = c - 97;
	return columnNum;
}
 
bool checkValidity(Dict *dictP, Board *boardP, string word, string position) 
{
	//check the existance of the word
	if (!dictP->headlineExists(word)) return false;
	
	//process the position string into line number, column number and direction
	int lineNum = whichLine(position);
	int columnNum = whichColumn(position);
	char direction = position[2];

	if (!boardP->clearSpace4Word(word.size(), lineNum, columnNum, direction)) return false;

	return true;
}

void puzzleCreate()
{
	//dictionary creation and extraction
	Dict dict;
	Dict *dictA = &dict;
	cout << "Dictionary file name ? "; cin >> dict.fileNameInput; 
	dict.extraction();
																									//from here
	//creation of the board
	int lines, columns;												
	//ask the user how many lines and columns
	cout << "Board size (lines columns)? "; cin >> lines >> columns;
	//construct the board
	Board board(lines, columns);
	Board *boardA = &board;

	//fill it with a point ('.') in every position
	board.pointFill();

	//loop to fill in the board
	string position, word;
	while (true) {
		board.show(); //show the board
		//the user choose what to do
		cout << "Position ( LCD / CTRL-Z = stop ) ? "; cin >> position;
		if (cin.eof()) { cin.clear(); break; } //CTRL-Z to get out of the loop

		cout << "Word ( - = remove / ? = help ) . ? "; cin >> word;
		//to remove a word
		if ("-" == word) {
			//TO BE COMPLETED <=========================================================================================
		}
		else if ("?" == word) {
			//TO BE COMPLETED <=========================================================================================
		}

		else checkValidity(dictA, boardA, word, position);



	} 
	cout << "\n\n\nYOU GOT OUT!\n\n\n";
																									//to here
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
		while(true) {
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
