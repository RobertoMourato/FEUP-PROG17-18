#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <Windows.h>
using namespace std;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

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

	bool wildcardMatch(const char *str, const char *strWild)
	{
		// We have a special case where string is empty ("") and the mask is "*".
		// We need to handle this too. So we can't test on !*str here.
		// The loop breaks when the match string is exhausted.
		while (*strWild)
		{
			// Single wildcard character
			if (*strWild == '?')
			{
				// Matches any character except empty string
				if (!*str)
					return false;
				// OK next
				++str;
				++strWild;
			}
			else if (*strWild == '*')
			{
				// Need to do some tricks.
				// 1. The wildcard * is ignored.
				// So just an empty string matches. This is done by recursion.
				// Because we eat one character from the match string,
				// the recursion will stop.
				if (wildcardMatch(str, strWild + 1))
					// we have a match and the * replaces no other character
					return true;
				// 2. Chance we eat the next character and try it again,
				// with a wildcard * match. This is done by recursion.
				// Because we eat one character from the string,
				// the recursion will stop.
				if (*str && wildcardMatch(str + 1, strWild))
					return true;
				// Nothing worked with this wildcard.
				return false;
			}
			else
			{
				// Standard compare of 2 chars. Note that *str might be 0 here,
				// but then we never get a match on *strWild
				// that has always a value while inside this loop.
				if (toupper(*str++) != toupper(*strWild++))
					return false;
			}
		}
		// Have a match? Only if both are at the end...
		return !*str && !*strWild;
	}

public:
	bool loadToProgram()
	{
		string completeLine;
		string word;
		string headline;

		f.open(fileNameInput);
		if (!f.is_open()) {
			SetConsoleTextAttribute(hConsole, 244);
			cerr << "File " << fileNameInput << " could not be found!\nTry again!\n";
			SetConsoleTextAttribute(hConsole, 15);
			return false;
		}

		cout << "loading...\n";
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
		cout << "end of loading of words and synonyms\n";
		f.close();
		return true;
	}

	bool headlineExists(string word) 
	{
		SetConsoleTextAttribute(hConsole, 244);
		string errorMessage = "\nThe word doesn't belong to the dictionary!\n\n";
		map<string, vector<string>>::iterator it = wordSynonyms.begin();

		for (it; it != wordSynonyms.end(); it++) {
			if ((*it).first == word) {
				return true;
			}
		}
		cout << errorMessage;
		return false;
	}

	vector<string> matchingWords(string wildCard)
	{
		vector<string> resultWord;
		map<string, vector<string>>::iterator it = wordSynonyms.begin();
		int maxNumber = 10;
		
		for (it = wordSynonyms.begin(); it != wordSynonyms.end(); it++) {
			if (maxNumber == 0) break;
			if (wildcardMatch(it->first.c_str(),wildCard.c_str())) {
				resultWord.push_back(it->first);
				maxNumber--;
			}
		}
		return resultWord;
	}

};


class Board {
private:
	unsigned int lines, columns;
	vector <vector <char> > layout; //bigger vector "lines", smaller vectors are columns
	vector <char> newEmpty; //needed to fill the vector
	//vector <string> wordsPlaced; //to be erased
	map <string, string> positionWordsPlaced; //to handle add and remove words, check on repeated words and output/input for/from files
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //console colors

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
		//else if (!(layout[column].at(line +1) >= 65 && layout[column].at(line + 1) <= 90)) return;

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
		//else if (!(layout.at(column + 1)[line] >= 65 && layout.at(column + 1)[line] <= 90)) return;
	
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

	void printInFile(fstream *f) {
		//the board
		for (unsigned int i = 0; i < columns; i++) {
			for (unsigned int j = 0; j < lines; i++) {
				*f << layout[i][j] << " ";
			}
			*f << endl;
		}
		//the position and word
		*f << endl << endl;
		map<string, string>::iterator it = positionWordsPlaced.begin();

		for (it = positionWordsPlaced.begin(); it != positionWordsPlaced.end(); it++) {
			*f << (*it).first << " " << (*it).second;
			*f << endl;
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
		cout << "\n   ";
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

		positionWordsPlaced[position] = word;
	//	wordsPlaced.push_back(word);
	}

	void removeWord(string position) {
		int line = whichLine(position);
		int column = whichColumn(position);
		char direction = position[2];
		// verificar se a posiçao no map é valida
		// retirar a palavra do map
		// retirar a palavra do board
		string errorMessage = "\n\nThere aren't any words to remove in that position!\n";
		map<string, string>::iterator it = positionWordsPlaced.find(position);
		if (it != positionWordsPlaced.end()) {
			positionWordsPlaced.erase(it);
			if ('V' == direction || 'v' == direction) removeVertical(line, column);
			else if ('H' == direction || 'h' == direction) removeHorizontal(line, column);
		}
		else {
			SetConsoleTextAttribute(hConsole, 244);
			cout << errorMessage;
			SetConsoleTextAttribute(hConsole, 15);
		}
	}

	bool checkSpace4Word(string word, string position)
	{
		//process the position string into line number, column number and direction
		unsigned int lineNum = whichLine(position);
		unsigned int columnNum = whichColumn(position);
		char direction = position[2];

		//change color
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
					if (!(layout[columnNum][i] == '.' || layout[columnNum][i] == word[i] || word[i] == '?')) {
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
					if (!(layout[columnNum][i + lineNum] == '.' || layout[columnNum][i + lineNum] == word[i] || word[i] == '?')) {
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
					if (!(layout[i][lineNum] == '.' || layout[i][lineNum] == word[i] || word[i] == '?')) { //all positions
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
					if (!(layout[i + columnNum][lineNum] == '.' || layout[i + columnNum][lineNum] == word[i] || word[i] == '?')) {
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

	//implementar o loop no map em vez de no vector
	bool unusedWord(string word)
	{
		SetConsoleTextAttribute(hConsole, 244);
		string errorMessage = "\nThe word was already used!\n\n";
		map<string, string>::iterator it = positionWordsPlaced.begin();

		for (it; it != positionWordsPlaced.end(); it++)
			if (it->second == word) {
				cout << errorMessage;
				return false;
			}
		return true;
	}

	bool validPosition(string position) {
		SetConsoleTextAttribute(hConsole, 244);
		char line = position[0];
		char column = position[1];
		char direction = position[2];
		unsigned int lineN = whichLine(position);
		unsigned int columnN = whichColumn(position);
		string errorMessageChar = "\nOnly upper case and down case letters are, respectively, allowed in the first and second place!\n";
		string errorMessageOutofBounds = "\nIt looks like the position %s doesn't exist in this board!\n";
		string errorMessageDirection = "\nThat direction doesn't exist. Choose between vertical 'V' or horizontal 'H'!\n";

		if (!(direction == 'v' || direction == 'V' || direction == 'h' || direction == 'H')) {
			cout << errorMessageDirection;
			return false;
		}
		else if (!(line >= 65 && line <= 90)) {
			cout << errorMessageChar;
			return false;
		}
		else if (!(column >= 97 && column <= 122)) {
			cout << errorMessageChar;
			return false;
		}
		else if (lineN > lines) {
			printf(errorMessageOutofBounds.c_str(), position.c_str());
			return false;
		}
		else if (columnN > columns) {
			printf(errorMessageOutofBounds.c_str(), position.c_str());
			return false;
		}
		else return true;
	}

	void getLettersRight(string &word, string position)
	{
		unsigned int line = whichLine(position);
		unsigned int column = whichColumn(position);
		char direction = position[2];

		if ('v' == direction || 'V' == direction)
			for (unsigned int i = 0; i < word.size(); i++) {
				if (layout[column][line + i] != '.') word[i] = layout[column][line +i];
			}
		else if ('h' == direction || 'H' == direction)
			for (unsigned int i = 0; i < word.size(); i++) {
				if (layout[column + i][line] != '.') word[i] = layout[column + i][line];
			}				
	}

	void extraction() {
		int n = -1;
		string fileOutput, docType;
		bool existingFile;
		fstream f;
		fstream *fA = &f;
		do {
			n++;
			fileOutput = "b";
			docType = ".txt";
			existingFile = false;
			if (n < 10) fileOutput += "00" + to_string(n) + docType;
			else if (n < 100) fileOutput += "0" + to_string(n) + docType;
			else fileOutput += to_string(n) + docType;
			cout << fileOutput;

			f.open(fileOutput);
			if (f.is_open()) {
				existingFile = true;
				f.close();
			}
			else {
				f.open(fileOutput, ios::out | ios::in | ios::trunc);
			}
		} while (existingFile);

		printInFile(fA);
		cout << "The extraction was successfully made to " << fileOutput << " file!";
	}
};

//vvvvvvvvvvvv TO BE MADE
bool allWordsValidity() {
	return true;
}
 
bool checkValidity(Dict *dictP, Board *boardP, string word, string position) 
{
	//check the existance of the word
	if (!dictP->headlineExists(word)) return false;

//	if (!boardP->clearSpace4Word(word.size(), lineNum, columnNum, direction)) return false;
	if (!boardP->unusedWord(word)) return false;
	if (!boardP->checkSpace4Word(word, position)) return false;
	return true;
}

void helpInsertWord(string position, Board *boardP, Dict *dictP) {
	bool repeatNoMatchingWords = true;
	while (repeatNoMatchingWords) {
		repeatNoMatchingWords = false;
		string wordToCreate;
		string errorMessageLength = "A word with so many letters don't fit!\n\nIf the help is no longer needed, press CTRL-Z";
		string errorMessageInput = "Only a numerical value is valid in here! Try again";
		string errorMessageBoolInput = "Only a 0 or a 1 is a valid option here! Try again";
		string returnMessage = "You chose to leave the help!";
		string warningMessageNoWords = "I'm sorry but no words were found that match the place you chose...\nDo you want to try again?\n";
		string errorInputWord = "You miswrote the word you chose!\nTry Again: ";
		bool errorInWordSize, errorInput;
		//input number of letters, input error robust (checks for space in the board and the right input in first question) and creates word and checks for space
		do {
			//inicialize flags
			errorInWordSize = false;
			errorInput = false;
			SetConsoleTextAttribute(hConsole, 15);
			//size of the word
			cout << "Insert the number of letters you want the word to have (CTRL-Z to leave the help) : ";
			int lettersNumber; cin >> lettersNumber;
			SetConsoleTextAttribute(hConsole, 244);
			if (cin.eof()) {
				cin.clear();
				cout << returnMessage;
				SetConsoleTextAttribute(hConsole, 15);
				return;
			}
			if (cin.fail()) {
				cin.clear();
				cin.ignore(1000000, '\n');
				cout << errorMessageInput;
				SetConsoleTextAttribute(hConsole, 15);
				errorInput = true;
				continue;
			}
			
			SetConsoleTextAttribute(hConsole, 15);
			//word with the asked size
			for (int i = 0; i < lettersNumber; i++)
				wordToCreate += "?";
			//is there any space available?
			if (!boardP->checkSpace4Word(wordToCreate, position)) errorInWordSize = true;
		} while (errorInput || errorInWordSize);

		boardP->getLettersRight(wordToCreate, position);
		vector<string> resultWord = dictP->matchingWords(wordToCreate);

		if (resultWord.size() == 0) {
			//waring message
			SetConsoleTextAttribute(hConsole, 244);
			cout << warningMessageNoWords;
			SetConsoleTextAttribute(hConsole, 15);
			//choose what to do
			bool repeat;
			cout << "0 - Finish help\n1 - Try again\n\tAnswer -> ";
			cin >> repeat;
			//input error robustness
			while (cin.fail()) {
				cin.clear();
				cin.ignore(1000000, '\n');
				SetConsoleTextAttribute(hConsole, 244);
				cout << errorMessageBoolInput;
				SetConsoleTextAttribute(hConsole, 15);
				cout << "\n\tAnswer -> ";
				cin >> repeat;
			}
			//repeat the funciton
			if (repeat) {
				repeatNoMatchingWords = true; continue;
			}
			//leave the function
			else if (!repeat) {
				cout << returnMessage; return;
			}
		}
		else {
			//show the content of the vector with the resulting words
			cout << endl;
			for (unsigned int i = 0; i < resultWord.size(); i++)
				cout << "\t" << resultWord[i];
			cout << "\n\nChoose one of them to add to the board [CTRL-Z if you want to cancel help]: ";
			bool wordMatchesVector;
			string answer;
			do {
				cin.clear();
				wordMatchesVector = false;
				cin >> answer;
				if (cin.fail()) {
					cin.clear();
					cin.ignore(1000000, '\n');
					SetConsoleTextAttribute(hConsole, 244);
					cout << errorInputWord;
					SetConsoleTextAttribute(hConsole, 15);
					continue;
				}
				transform(answer.begin(), answer.end(), answer.begin(), ::toupper); //upper case
				for (unsigned int i = 0; i < resultWord.size(); i++)
					if (resultWord[i] == answer) {	//searches for the word in the vector, because all the checking was done before 
						wordMatchesVector = true;	//and a mistaken word here would break the "security" of the board
						break;
					}
				if (cin.eof()) {
					cin.clear(); cout << returnMessage;  return;
				}
			} while (cin.fail() || !wordMatchesVector);
			boardP->addWord(answer, position);
		}
	}
}

void puzzleCreate()
{
	//dictionary creation and extraction
	Dict dict;
	Dict *dictA = &dict;
	bool errorOpeningFile;
	do {
		errorOpeningFile = false;
		cin.clear();
		string errorMessageFileInput = "That input is not valid! Try again\n";
		cout << "Dictionary file name ? ";
		cin >> dict.fileNameInput;
		if (cin.fail()) {
			cin.ignore(1000000, '\n');
			SetConsoleTextAttribute(hConsole, 244);
			cout << errorMessageFileInput;
			SetConsoleTextAttribute(hConsole, 15);
			continue;
		}
		if (cin.eof())
		{
			cin.ignore(100, '\n');
			SetConsoleTextAttribute(hConsole, 244);
			cout << errorMessageFileInput;
			SetConsoleTextAttribute(hConsole, 15);
			continue;
		}
		if (!dict.loadToProgram()) errorOpeningFile = true;

	} while (cin.fail() || errorOpeningFile);

			
	//creation of the board
	int lines, columns;

	//ask the user how many lines and columns
	//error proof, the do while loop will work when there are errors in the input and the user has more chances

	bool cinError;
	do {									//<=========== robusto o suficiente???
		string errorMessage26 = "\nI'm sorry but that number is too big! Write that again.\n";
		string errorMessageCinFail = "\nThose aren't valid sizes for the board! Try again...\n";
		cinError = false;
		cout << "Board size[max size=26] (lines columns)? "; cin >> lines >> columns; 

		SetConsoleTextAttribute(hConsole, 244);
		if (cin.fail()) {
			cin.clear();
			cin.ignore(1000000, '\n');
			cout << errorMessageCinFail;
			cinError = true;
		}
		else if (lines > 26 || columns > 26) {
			cout << errorMessage26;
			cinError = true;
		}
		SetConsoleTextAttribute(hConsole, 15);
	} while (cinError);	


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
		transform(word.begin(), word.end(), word.begin(), ::toupper); //upper case the word

		//check if position input is correct
		if (!board.validPosition(position)) {
			SetConsoleTextAttribute(hConsole, 244);
			cout << "That position is not valid... Try Again!\n";
			SetConsoleTextAttribute(hConsole, 15);
		}
		//to remove a word
		else if ("-" == word) {
			board.removeWord(position);
		}
		//to help the user
		else if ("?" == word) {
			helpInsertWord(position, boardA, dictA);
		}
		//to add the respective word
		else if (checkValidity(dictA, boardA, word, position)) board.addWord(word, position);

	} 
	if (allWordsValidity()) {
		cout << "\nAll words are valid, the extraction will continue\n";
		//board.extraction();
	}
	//funçao para verificar a validade de todas as palavras, em todas as linhas e colunas
	//perguntar ao utilizador se quer continuar caso haja erro
	//ou prosseguir com a extraçao

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
	cout << "    Direction can be vertical (V) or horizontal (H)\n";
	cout << "  CTRL-Z will end the board creation and start the file extraction process";
	cout << "Word ( - = remove / ? = help )\n";
	cout << "  there are several options here:\n";
	cout << "    typing a word to be added to the board\n";
	cout << "    '-' to remove a word from the crossword board\n";
	cout << "    '?' gives you a list of possible words to add to the board\n\n";
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
			if (cin.eof()) {
				cin.clear();
				cin.ignore(1000, '\n');
				continue;
			}
			if ("0" == chosenOption) return 0;
			if ("1" == chosenOption) {
				puzzleCreate();
				break;
			}
			//if ("2" == chosenOption) { puzzleResume(); break; }
		} 
	}
}
