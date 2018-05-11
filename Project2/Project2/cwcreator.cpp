#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map> //<-------------------------------
#include <algorithm>
#include <Windows.h>

using namespace std;

class Dict {
private:
	map <string, vector <string> > wordSynonyms;
	ifstream f;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

public:
	string fileNameInput;

private:
	bool isHeadline(string line)
	{
		int doisPontos, virgula;
		doisPontos = line.find(":");
		virgula = line.find(",");
		if (doisPontos == -1) return false;
		else if (doisPontos < virgula) return true;
		else return false;
	}

	string singleWord(string &Line) //retirar a proxima palavra da linha
	{
		string delimiter = ":, "; //delimitadores
		//place where the delimiter will be
		//check if the first char is a letter
		//and if not cut the chars till we reach a letter, and then, a word
		while (!((Line[0] >= 65 && Line[0] <= 90) || (Line[0] >= 97 && Line[0] <= 122))) {
			Line.erase(0, 1);
			//cout << ".\n";
		}
		int found = Line.find_first_of(delimiter), lineSize = Line.size();
		//individual word
		string singleWord = Line.substr(0, found); 
		//erase the given words
		if (-1 == found) Line.erase(0, lineSize);
		else if (found < lineSize) Line.erase(0, found + 1);
		else Line.erase(0, found);
		
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
	void loadToProgram()
	{
		string completeLine;
		string word;
		string headline;

		f.open(fileNameInput);
		if (!f.is_open()) {
			cerr << "File " << fileNameInput << " could not be found!\n";
			system("pause");
			return;
		}

		cout << "extrating the words and synonyms...\n";
																			//do while aqui
		//first line, except lines with "[ (...) ]"
		do {
			getline(f, completeLine);
		} while (!validLine(completeLine));

		//line and headline analysis
		while (!f.eof()) {
			vector <string> synonyms;
			while (completeLine != "\0") {
				if (isHeadline(completeLine)) {
					//i++;
					//wordSynonyms.push_back(newEmptyVector);
					//wordSynonyms[i].push_back(singleWord(completeLine));
					headline = singleWord(completeLine);
					transform(headline.begin(), headline.end(), headline.begin(), ::toupper); //->uppercase
				}
				else {
					//wordSynonyms[i].push_back(singleWord(completeLine)); 
					word = singleWord(completeLine);
					transform(word.begin(), word.end(), word.begin(), ::toupper); //->uppercase
					synonyms.push_back(word);
				}
			}

		//	cout << endl << headline  << "  -  ";
		//	for (unsigned int i = 0; i < synonyms.size(); i++)
		//		cout << synonyms[i] << " ";
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
		SetConsoleTextAttribute(hConsole, 244);
		string errorMessage = "\nThe word doesn't belong to the dictionary!\n\n";
		map<string, vector<string>>::iterator it = wordSynonyms.begin();

		for (it = wordSynonyms.begin(); it != wordSynonyms.end(); it++) {
			if ((*it).first == word) {
				return true;
			}
		}
		cout << errorMessage;
		return false;
	}

};


class Board {
private:
	unsigned int lines, columns;
	vector <vector <char> > layout; //bigger vector "lines", smaller vectors are columns
	vector <char> newEmpty;
	vector <string> wordsPlaced;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	void addVertical(string word, int line, int column)
	{
		if (0 < line) layout[column].at(line - 1) = '#';
		for (unsigned int i = 0; i < word.size(); i++)
			layout[column][line + i] = word[i];
		if (line + word.size() < lines) layout[column].at(line + word.size()) = '#';
	}

	void addHorizontal(string word, int line, int column)
	{
		if (0 < column) layout.at(column - 1)[line] = '#';
		for (unsigned int i = 0; i < word.size(); i++)
			layout[column + i][line] = word[i];
		if (column + word.size() < columns) layout.at(column + word.size())[line] = '#';
	}

	bool crossedWordsVertical(unsigned int column, unsigned int line)
	{
		if (column > 0) {
			if (layout[column - 1][line] >= 65 && layout[column - 1][line] <= 90)
				return true;
		}
		if (column < columns) {
			if (layout[column + 1][line] >= 65 && layout[column + 1][line] <= 90)
				return true;
		}
		return false;
	}

	bool crossedWordsHorizontal(unsigned int column, unsigned int line)
	{
		if (line > 0) {
			if (layout[column][line - 1] >= 65 && layout[column][line - 1] <= 90)
				return true;
		}
		if (line < lines) {
			if (layout[column][line + 1] >= 65 && layout[column][line + 1] <= 90)
				return true;
		}
		return false;
	}

	void removeVertical(int line, int column)
	{
		if (0 < line) {
			if (layout[column].at(line - 1) == '#') layout[column].at(line - 1) = '.';
			else return;
		}
		else if (!(layout[column].at(line +1) >= 65 && layout[column].at(line + 1) <= 90)) return;

		int i = 0;
		while (true)
		{
			if (line + i == lines) return;
			if (layout[column][line + i] == '#')
			{
				layout[column][line + i] = '.';
				return;
			}
			if (crossedWordsVertical(column, line + i)) {
				i++; continue;
			}
			else layout[column][line + i] = '.';
			i++;
		}
	}

	void removeHorizontal(int line, int column)
	{
		if (0 < column) {
			if (layout.at(column - 1)[line] == '#') layout.at(column - 1)[line] = '.';
			else return;
		}
		else if (!(layout.at(column + 1)[line] >= 65 && layout.at(column + 1)[line] <= 90)) return;
		int i = 0;
		while (true)
		{
			if (column + i == columns) return;
			if (layout[column + i][line] == '#')
			{
				layout[column + i][line] = '.';
				return;
			}
			if (crossedWordsHorizontal(column + i, line)) {
				i++; continue;
			}
			else layout[column +i][line] = '.';
			i++;
		}
	}

public:
	Board()      
	{
		lines = 10;  //default size
		columns = 10; //default size
	}//constructor

	Board(unsigned int lines, unsigned int columns) 
	{
		this->lines = lines; 
		this->columns = columns;
	}	//constructor with arguments					

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
		int REDblack = 12; //red on black
		int BLACKsoftGray = 112; //black on soft gray
		int SOFTGRAYblack = 7; //soft gray on black
		int WHITEblack = 15;

		SetConsoleTextAttribute(hConsole, REDblack);
		char a = 'a';
		char A = 'A';
		cout << "   ";
		for (unsigned int i = 0; i < columns; i++) {
			
			char b = a + i;
			cout << " " << b;
		}
		cout << endl;
		for (unsigned int j = 0; j < lines; j++) {
			SetConsoleTextAttribute(hConsole, REDblack);
			char B = A + j;
			cout << " " << B << " ";
			for (unsigned int i = 0; i < columns; i++) {
				if (layout[i][j] == '#')
				{
					cout << " ";
					SetConsoleTextAttribute(hConsole, SOFTGRAYblack);
					cout << layout[i][j];
				}
				else
				{
					SetConsoleTextAttribute(hConsole, BLACKsoftGray);
					cout << " " << layout[i][j];
				}
				
			}
			cout << endl;
		}
		SetConsoleTextAttribute(hConsole, WHITEblack);
	}

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

	void addWord(string word, string position) {
		int lineNum = whichLine(position);
		int columnNum = whichColumn(position);
		char direction = position[2];

		if ('V' == direction || 'v' == direction) addVertical(word, lineNum, columnNum);
		if ('H' == direction || 'v' == direction) addHorizontal(word, lineNum, columnNum);

		wordsPlaced.push_back(word);
	}

	void removeWord(string position) {
		int line = whichLine(position);
		int column = whichColumn(position);
		char direction = position[2];

		if (layout[column][line] == '#' || layout[column][line] == '.') {
			cout << "\nThere aren't any word to remove in this position\n\n";
			return;
		}
		if ('V' == direction || 'v' == direction) removeVertical(line, column);
		if ('H' == direction || 'h' == direction) removeHorizontal(line, column);
	}

	bool checkSpace4Word(string word, unsigned int lineNum, unsigned int columnNum, char direction)
	{
		SetConsoleTextAttribute(hConsole, 244);
		unsigned int wSize = word.size();
		string errorMessage = "\nThe word you are trying to insert, doesn't fit in the board!\n\n";

		//check if the beginning is out of boundaries or not
		if (lineNum + 1 > lines || columnNum + 1 > columns) { cout << errorMessage; return false;  }
		//checks if there are space for the word, depending on the initial position, and the word size
		//checks several conditions
		if ('V' == direction || 'v' == direction) {
			if (0 == lineNum) {  //beginning of the column
				for (unsigned int i = 0; i < wSize; i++) //checks every position
					if (!(layout[columnNum][i] == '.' || layout[columnNum][i] == word[i])) {
						cout << errorMessage;  return false;
					}
				if (wSize < lines)
					if (!(layout[columnNum].at(wSize) == '#' || layout[columnNum].at(wSize) == '.')) { // checks the final position +1
						cout << errorMessage; return false; 
					}
			}
			else {
				if (lineNum + wSize > lines) {
					cout << errorMessage; return false;
				}
				for (unsigned int i = 0; i < wSize; i++) //all position checking
					if (!(layout[columnNum][i + lineNum] == '.' || layout[columnNum][i + lineNum] == word[i])) {
						cout << errorMessage; return false;
					}
				if (lineNum + wSize < lines) 
					if (!(layout[columnNum].at(wSize + lineNum) == '#' || layout[columnNum].at(wSize + lineNum) == '.')) { //empty space in the end
						cout << errorMessage; return false;
					} 
				if (!(layout[columnNum].at(lineNum - 1) == '#' || layout[columnNum].at(lineNum - 1) == '.')) { //empty space in the beginning
					cout << errorMessage; return false;
				}
			}
		}
		else if ('H' == direction || 'h' == direction)
		{
			if (0 == columnNum)
			{
				for (unsigned int i = 0; i < wSize; i++)
					if (!(layout[i][lineNum] == '.' || layout[i][lineNum] == word[i])) { //all positions
						cout << errorMessage; return false;
					}
				if (columnNum + wSize < columns) 
					if (!(layout.at(wSize)[lineNum] == '#' || layout.at(wSize)[lineNum] == '.')) { // checks the final position +1
						cout << errorMessage; return false;
					}
			}
			else {
				if (columnNum + wSize > columns) {
					cout << errorMessage; return false;
				}
				for (unsigned int i = 0; i < wSize; i++) //all position checking
					if (!(layout[i + columnNum][lineNum] == '.' || layout[i + columnNum][lineNum] == word[i])) {
						cout << errorMessage; return false;
					}
				if (columnNum + wSize < columns) 
					if (!(layout.at(wSize + columnNum)[lineNum] == '#' || layout.at(wSize + columnNum)[lineNum] == '.')) {  //empty space in the end
						cout << errorMessage; return false;
					}
				if (!(layout.at(columnNum - 1)[lineNum] == '#' || layout.at(columnNum - 1)[lineNum] == '.')) { //empty space in the beginning
					cout << errorMessage; return false;
				}
			}
		}
		return true;
	}

	bool unusedWord(string word)
	{
		SetConsoleTextAttribute(hConsole, 244);
		string errorMessage = "\nThe word was already used!\n\n";
		for (unsigned int i = 0; i < wordsPlaced.size(); i++)
			if (word == wordsPlaced[i])
			{
				cout << errorMessage;
				return false;
			}
		return true;
	}
};


 
bool checkValidity(Dict *dictP, Board *boardP, string word, string position) 
{
	//check the existance of the word
	if (!dictP->headlineExists(word)) return false;
	
	//process the position string into line number, column number and direction
	int lineNum = boardP->whichLine(position);
	int columnNum = boardP->whichColumn(position);
	char direction = position[2];

//	if (!boardP->clearSpace4Word(word.size(), lineNum, columnNum, direction)) return false;
	if (!boardP->unusedWord(word)) return false;
	if (!boardP->checkSpace4Word(word, lineNum, columnNum, direction)) return false;
	return true;
}

void puzzleCreate()
{
	//dictionary creation and extraction
	Dict dict;
	Dict *dictA = &dict;
	cout << "Dictionary file name ? "; cin >> dict.fileNameInput; 
	dict.loadToProgram();
					
	//creation of the board
	int lines, columns;												
	//ask the user how many lines and columns
	cout << "Board size (lines columns)? "; cin >> lines >> columns; //<===================================
																	 //como é int, tem de verificar o erro, cin.claer(), cin.ignore(100000, \n) e repetir
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
		cout << "Position ( LCD / CTRL-Z = stop ) ? "; cin >> position; //<======================= tornar mais robusto
		if (cin.eof()) { cin.clear(); break; } //CTRL-Z to get out of the loop

		cout << "Word ( - = remove / ? = help ) . ? "; cin >> word;
		transform(word.begin(), word.end(), word.begin(), ::toupper);
		//to remove a word
		if ("-" == word) {
			board.removeWord(position);
		}
		else if ("?" == word) {
			//TO BE COMPLETED <=========================================================================================
		}

		else if (checkValidity(dictA, boardA, word, position)) board.addWord(word, position);



	} 
	//if (board.AllWordsValidity()) {
	//cout << "All words are valid, the extraction will continue"; 
	//board.extraction
	//funçao para verificar a validade de todas as palavras, em todas as linhas e colunas
	//perguntar ao utilizador se quer continuar caso haja erro
	//ou prosseguir com a extraçao

}	

/*
void wordsTest()
{

} */

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
	cout << "      '-' to remove a word from the crossword board\n";
	cout << "      '?' gives you a list of possible words to add to the board\n\n";
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
