 #include "Board.h"


Board::Board()
{
	lines = 10;  //default size
	columns = 10; //default size
}//constructor 

Board::Board(unsigned int lines, unsigned int columns)
{
	this->lines = lines; //saves number of lines
	this->columns = columns; //saves number of columns
}	//constructor with arguments	

Board::~Board()
{

}

void Board::addVertical(string word, int line, int column)
{
	if (0 < line) layout[column].at(line - 1) = '#'; //if word doesnt start in the first line, add a '#' before

	for (unsigned int i = 0; i < word.size(); i++)
		layout[column][line + i] = word[i]; //adds the word to the positions
	if (line + word.size() < lines) layout[column].at(line + word.size()) = '#'; //if the word doesnt finish in the last line, add a '#' after
}

void Board::addHorizontal(string word, int line, int column)
{
	if (0 < column) layout.at(column - 1)[line] = '#';//if word doesnt start in the first column, add a '#' before
	for (unsigned int i = 0; i < word.size(); i++)
		layout[column + i][line] = word[i];//adds the word to the positions
	if (column + word.size() < columns) layout.at(column + word.size())[line] = '#';//if the word doesnt finish in the last column, add a '#' after
}

bool Board::crossedWordsVertical(unsigned int column, unsigned int line)
{
	if (column > 0) //if word is not in the first column
	{
		if (layout[column - 1][line] >= 65 && layout[column - 1][line] <= 90) //checks for words in the boundaries
			return true;
	}
	if (column < columns) //if the word is not in the last column
	{
		if (layout[column + 1][line] >= 65 && layout[column + 1][line] <= 90) //checks for words in the boundaries
			return true;
	}
	return false; //false if no word in the position
}

bool Board::crossedWordsHorizontal(unsigned int column, unsigned int line)
{
	if (line > 0)//if word is not in the first line
	{
		if (layout[column][line - 1] >= 65 && layout[column][line - 1] <= 90) //checks for words in the boundaries
			return true;
	}
	if (line < lines)//if the word is not in the last line
	{
		if (layout[column][line + 1] >= 65 && layout[column][line + 1] <= 90) //checks for words in the boundaries
			return true;
	}
	return false;//false if no word in the position
}

void Board::printInFile(fstream *f, string dictFile)
{
	//dictionary name and space
	*f << dictFile << endl << endl;

	//the board
	for (unsigned int j = 0; j < lines; j++)
	{
		for (unsigned int i = 0; i < columns; i++)
		{
			*f << layout[i][j] << " "; //saves every position of the board
		}
		*f << endl; //next line
	}
	//the position and word
	*f << endl; //next line
	map<string, string>::iterator it; //map iterator

	for (it = positionWordsPlaced.begin(); it != positionWordsPlaced.end(); it++) //loop in map
	{
		*f << (*it).first << " " << (*it).second; //saves every position and word into the file
		*f << endl;
	}
}

void Board::pointFill()
{
	//fill the board with points
	char point = '.';
	//	layout:	outer vector columns
	//			inner vector lines
	for (unsigned int i = 0; i < columns; i++) //all columns
	{
		layout.push_back(newEmpty); //opens a space in the vector for the next column
		for (unsigned int j = 0; j < lines; j++) //all lines
		{
			layout[i].push_back(point); //more points
		}
	}
}

void Board::show()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int REDblack = 12; //red on black
	int BLACKsoftGray = 112; //black on soft gray
	int SOFTGRAYblack = 7; //soft gray on black
	int WHITEblack = 15; //white on black

	SetConsoleTextAttribute(hConsole, REDblack);
	char a = 'a'; 
	char A = 'A';
	cout << "\n   ";
	for (unsigned int i = 0; i < columns; i++) //first line identification (horizontal)
	{

		char b = a + i;
		cout << " " << b;
	}
	cout << endl;
	for (unsigned int j = 0; j < lines; j++) 
	{
		SetConsoleTextAttribute(hConsole, REDblack);
		char B = A + j;
		cout << " " << B << " ";//first column identification (vertical)
		for (unsigned int i = 0; i < columns; i++)
		{
			if (layout[i][j] == '#') //if '#', diferent color
			{
				cout << " ";
				SetConsoleTextAttribute(hConsole, SOFTGRAYblack);
				cout << layout[i][j];
			}
			else //letter, "normal" color
			{
				SetConsoleTextAttribute(hConsole, BLACKsoftGray);
				cout << " " << layout[i][j];
			}

		}
		cout << endl;
	}
	SetConsoleTextAttribute(hConsole, WHITEblack); //normal color
}

int Board::whichLine(string position)
{
	//first char of position
	char l = position[0];
	//char - A = position on vector (A-A=0, 1st line, position 0 on the vector)
	int lineNum = l - 65;
	return lineNum;
}

int Board::whichColumn(string position)
{
	//second char of position
	char c = position[1];
	//char - a = position on vector
	int columnNum = c - 97;
	return columnNum;
}

void Board::addWord(string word, string position)
{
	int lineNum = whichLine(position); //converts the char to line number
	int columnNum = whichColumn(position); //converts the char to column number
	char direction = position[2]; //direction char

	//depending on the direction char, calls addvertical or addhorizontal
	if ('V' == direction || 'v' == direction) addVertical(word, lineNum, columnNum); 
	if ('H' == direction || 'h' == direction) addHorizontal(word, lineNum, columnNum);

	positionWordsPlaced[position] = word; //adds the position and word in the map
}

void Board::removeWord(string position)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	string errorMessage = "\n\nThere aren't any words to remove in that position!\n";

	map<string, string>::iterator it = positionWordsPlaced.find(position);
	if (it == positionWordsPlaced.end()) //if position doesnt belong to the map
	{
		SetConsoleTextAttribute(hConsole, 244);
		cout << errorMessage; //error
		SetConsoleTextAttribute(hConsole, 15);
		return;
	}
	else {
		positionWordsPlaced.erase(it); //erase from map
		boardReset(); //board reset to '.'
		it = positionWordsPlaced.begin();
		while (it != positionWordsPlaced.end()) //adds every word again to the board
		{
			addWord(it->second, it->first);
			it++;
		}
	}
	
}

void Board::boardReset()
{
	//every position reseted to '.'
	for (unsigned int i = 0; i < columns; i++)
		for (unsigned int j = 0; j < lines; j++)
			layout[i][j] = '.';
}

bool Board::checkSpace4Word(string word, string position)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//process the position string into line number, column number and direction
	unsigned int lineNum = whichLine(position);
	unsigned int columnNum = whichColumn(position);
	char direction = position[2];

	//change color
	SetConsoleTextAttribute(hConsole, 244);
	unsigned int wSize = word.size();
	string errorMessage = "\nThe word you are trying to insert, doesn't fit in the board!\n\n";

	//check if the beginning is out of boundaries or not
	if (lineNum + 1 > lines || columnNum + 1 > columns)
	{
		cout << errorMessage; return false;
	}
	//checks if there are space for the word, depending on the initial position, and the word size
	//checks several conditions
	if ('V' == direction || 'v' == direction)
	{
		if (0 == lineNum)
		{  //beginning of the column
			for (unsigned int i = 0; i < wSize; i++) //checks every position
				if (!(layout[columnNum][i] == '.' || layout[columnNum][i] == word[i] || word[i] == '?'))
				{
					cout << errorMessage;  return false;
				}
			if (wSize < lines)
				if (!(layout[columnNum].at(wSize) == '#' || layout[columnNum].at(wSize) == '.')) // checks the final position +1
				{
					cout << errorMessage; return false;
				}
		}
		else
		{
			if (lineNum + wSize > lines)
			{
				cout << errorMessage; return false;
			}
			for (unsigned int i = 0; i < wSize; i++) //all position checking
				if (!(layout[columnNum][i + lineNum] == '.' || layout[columnNum][i + lineNum] == word[i] || word[i] == '?'))
				{
					cout << errorMessage; return false;
				}
			if (lineNum + wSize < lines)
				if (!(layout[columnNum].at(wSize + lineNum) == '#' || layout[columnNum].at(wSize + lineNum) == '.')) //empty space in the end
				{
					cout << errorMessage; return false;
				}
			if (!(layout[columnNum].at(lineNum - 1) == '#' || layout[columnNum].at(lineNum - 1) == '.'))  //empty space in the beginning
			{
				cout << errorMessage; return false;
			}
		}
	}
	else if ('H' == direction || 'h' == direction)
	{
		if (0 == columnNum)
		{
			for (unsigned int i = 0; i < wSize; i++)
				if (!(layout[i][lineNum] == '.' || layout[i][lineNum] == word[i] || word[i] == '?')) //all positions
				{
					cout << errorMessage; return false;
				}
			if (columnNum + wSize < columns)
				if (!(layout.at(wSize)[lineNum] == '#' || layout.at(wSize)[lineNum] == '.')) // checks the final position +1
				{
					cout << errorMessage; return false;
				}
		}
		else
		{
			if (columnNum + wSize > columns)
			{
				cout << errorMessage; return false;
			}
			for (unsigned int i = 0; i < wSize; i++) //all position checking
				if (!(layout[i + columnNum][lineNum] == '.' || layout[i + columnNum][lineNum] == word[i] || word[i] == '?'))
				{
					cout << errorMessage; return false;
				}
			if (columnNum + wSize < columns)
				if (!(layout.at(wSize + columnNum)[lineNum] == '#' || layout.at(wSize + columnNum)[lineNum] == '.')) //empty space in the end
				{
					cout << errorMessage; return false;
				}
			if (!(layout.at(columnNum - 1)[lineNum] == '#' || layout.at(columnNum - 1)[lineNum] == '.'))  //empty space in the beginning
			{
				cout << errorMessage; return false;
			}
		}
	}
	return true;
}

bool Board::unusedWord(string word)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 244); //error color
	string errorMessage = "\nThe word was already used!\n\n"; //error message
	map<string, string>::iterator it = positionWordsPlaced.begin(); //iterator

	for (it; it != positionWordsPlaced.end(); it++)
	{
		if (it->second == word) //if word ==  word being added
		{
			cout << errorMessage;
			return false; 
		}
	}
	return true;
}

bool Board::validPosition(string position)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 244);
	char line = position[0];
	char column = position[1];
	char direction = position[2];
	unsigned int lineN = whichLine(position);
	unsigned int columnN = whichColumn(position);
	string errorMessageChar = "\nOnly upper case and down case letters are, respectively, allowed in the first and second place!\n"; //error message in case the line/column identification is wrong
	string errorMessageOutofBounds = "\nIt looks like the position %s doesn't exist in this board!\n"; //out of boundaries error
	string errorMessageDirection = "\nThat direction doesn't exist. Choose between vertical 'V' or horizontal 'H'!\n"; //not v or h used for direction

	if (!(direction == 'v' || direction == 'V' || direction == 'h' || direction == 'H')) //not V or H used for direction
	{
		cout << errorMessageDirection;
		return false;
	}
	else if (!(line >= 65 && line <= 90)) //not between A and Z
	{
		cout << errorMessageChar;
		return false;
	}
	else if (!(column >= 97 && column <= 122)) //not between a and z
	{
		cout << errorMessageChar;
		return false;
	}
	else if (lineN > lines) //more then existing lines
	{
		printf(errorMessageOutofBounds.c_str(), position.c_str());
		return false;
	}
	else if (columnN > columns) //more than existing columns
	{
		printf(errorMessageOutofBounds.c_str(), position.c_str());
		return false;
	}
	else return true;
}

void Board::getLettersRight(string &word, string position)
{
	unsigned int line = whichLine(position);
	unsigned int column = whichColumn(position);
	char direction = position[2];

	if ('v' == direction || 'V' == direction)
		for (unsigned int i = 0; i < word.size(); i++) //loop in every position that the word would fit
		{
			if (layout[column][line + i] != '.') word[i] = layout[column][line + i]; //checks if there is anything more than ., if true, substitute in the wildcard
		}
	else if ('h' == direction || 'H' == direction)
		for (unsigned int i = 0; i < word.size(); i++) //loop in every position that the word would fit
		{
			if (layout[column + i][line] != '.') word[i] = layout[column + i][line]; //checks if there is anything more than ., if true, substitute in the wildcard
		}
}

vector<string> Board::verifyHorizontal()
{

	vector<string> palavras;
	string palavra;

	for (unsigned int i = 0; i < layout[0].size(); i++)
	{
		for (unsigned int k = 0; k < layout.size(); k++)
		{
			if (layout[k][i] > 64 && layout[k][i] < 91)
			{
				palavra += layout[k][i];
			}
			else if (k == 0)
			{
				continue;
			}
			else if (palavra.size() == 1)
			{
				palavra = "";
			}
			else if (layout[k - 1][i] > 64 && layout[k - 1][i] < 91)
			{
				palavras.push_back(palavra);
				palavra = "";
			}
		}
		if (palavra.size() == 1)
		{
			palavra = "";
		}
		if (palavra.size() != 0)
		{
			palavras.push_back(palavra);
			palavra = "";
		}
	}
	return palavras;
}

vector<string> Board::verifyVertical()
{
	vector<string> palavras;
	string palavra;

	for (unsigned int i = 0; i < layout.size(); i++)
	{
		for (unsigned int k = 0; k < layout[0].size(); k++)
		{
			if (layout[i][k] > 64 && layout[i][k] < 91)
			{
				palavra += layout[i][k];
			}
			else if (k == 0)
			{
				continue;
			}
			else if (palavra.size() == 1)
			{
				palavra = "";
			}
			else if (layout[i][k - 1] > 64 && layout[i][k - 1] < 91)
			{
				palavras.push_back(palavra);
				palavra = "";
			}
		}
		if (palavra.size() == 1)
		{
			palavra = "";
		}
		else if (palavra.size() != 0)
		{
			palavras.push_back(palavra);
			palavra = "";
		}
	}
	return palavras;
}

void Board::extraction(string dictFile)
{
	int n = -1;
	string fileOutput, docType;
	bool existingFile;
	fstream f;
	fstream *fA = &f;
	do { //looks for existing file
		n++;
		fileOutput = "b";
		docType = ".txt";
		existingFile = false;
		//creating file name, depending on the i
		if (n < 10) fileOutput += "00" + to_string(n) + docType;
		else if (n < 100) fileOutput += "0" + to_string(n) + docType;
		else fileOutput += to_string(n) + docType;

		f.open(fileOutput);
		if (f.is_open()) //file existes
		{
			existingFile = true; //next loop
			f.close(); //close
		}
		else {
			f.open(fileOutput, ios::out | ios::in | ios::trunc); //if not opened, creates file
		}
	} while (existingFile); //next loop

	printInFile(fA, dictFile); //prints everything
	cout << "The extraction was successfully made to " << fileOutput << " file!";
	f.close(); //close
}

void Board::hashtagFill()
{
	for (unsigned int i = 0; i < columns; i++)
		for (unsigned int j = 0; j < lines; j++)
			if (layout[i][j] == '.') //if there is '.'
				layout[i][j] = '#'; //switches for '#'
}

void Board::loadFromFile(fstream *f)
{
	string line; //complete lines
	string word, position;
	getline(*f, line); //second line, blank
	getline(*f, line); //first board 
	lines = line.size() / 2; //number of board lines
	int i = 0; //columns counter
	while (getline(*f, line))
	{
		i++; //counter
		if (line.empty()) break; //when it reaches the line under the grid, pull out
	}
	columns = i; //columns definition
	pointFill(); //fill the vector with points

	while (!f->eof())
	{
		*f >> position >> word; //input of position and word from file
		addWord(word, position); //as they already were verified, add the word automatically
	}


}

void Board::reExtraction(string dictFile, string outputFile)
{
	fstream f;
	fstream *fA = &f;

	f.open(outputFile); //opensthe same file that it was loaded

	printInFile(fA, dictFile); //prints everything
	cout << "The extraction was successfully made to " << outputFile << " file!";
	f.close(); //close
}

void Board::grid()
{
	//shows adapted to the player program
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int REDblack = 12; //red on black
	int BLACKsoftGray = 112; //black on soft gray
	int SOFTGRAYblack = 7; //soft gray on black
	int WHITEblack = 15;
	int WHITEwhite = 119;
	int BLACKblack = 0;


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
			if (layout[i][j] == '#' || layout[i][j] == '.') //if '#' or '.', shows in black
			{
				cout << " ";
				SetConsoleTextAttribute(hConsole, BLACKblack);
				cout << layout[i][j];
			}
			else if (layout[i][j] == '$') //if '$', shows in gray
			{
				SetConsoleTextAttribute(hConsole, WHITEwhite);
				cout << " " << layout[i][j];
			}
			else //else, normal color
			{
				SetConsoleTextAttribute(hConsole, SOFTGRAYblack);
				cout << " " << layout[i][j];
			}

		}
		cout << endl;
	}
	SetConsoleTextAttribute(hConsole, WHITEblack);
}

void Board::addVerticalGrid(string word, int line, int column)
{
	if (0 < line) layout[column].at(line - 1) = '#'; //if not in the beginning of column, add '#' before

	for (unsigned int i = 0; i < word.size(); i++) //loop in word
		layout[column][line + i] = '$';//adds "$$$" word to the grid
	if (line + word.size() < lines) layout[column].at(line + word.size()) = '#'; //if not the end of column, add '#'
}

void Board::addHorizontalGrid(string word, int line, int column)
{
	if (0 < column) layout.at(column - 1)[line] = '#';//if not in the beginning of line, add '#' before
	for (unsigned int i = 0; i < word.size(); i++) //loop in word
		layout[column + i][line] = '$'; //adds "$$$" word to the grid
	if (column + word.size() < columns) layout.at(column + word.size())[line] = '#';//if not the end of line, add '#'
}

void Board::loadFromFileGrid(fstream *f)
{
	string line; //complete lines
	string word, position;
	getline(*f, line); //second line, blank
	getline(*f, line); //first board 
	lines = line.size() / 2; //number of board lines
	int i = 0; //columns counter
	while (getline(*f, line))
	{
		i++; //counter
		if (line.empty()) break; //when it reaches the line under the grid, pull out
	}
	columns = i; //columns definition
	pointFill(); //fill the vector with points

	while (!f->eof())
	{
		*f >> position >> word; //input of position and word from file
		addWord1stGrid(word, position); //as they already were verified, add the word automatically
	}
}

void Board::addWord1stGrid(string word, string position)
{
	int lineNum = whichLine(position);
	int columnNum = whichColumn(position);
	char direction = position[2];

	if ('V' == direction || 'v' == direction) addVerticalGrid(word, lineNum, columnNum); //add word "normally" to the board (goes normal, changes to $$$$)
	if ('H' == direction || 'h' == direction) addHorizontalGrid(word, lineNum, columnNum); //add word "normally" to the board (same as above)

	positionWordsPlaced[position] = word; //adds in the map

	for (unsigned int i = 0; i < word.size(); i++)
		word[i] = '$'; //changes word to "$$$"
	positionWordsPlacedGrid[position] = word; //adds "$$$" word to map

}

void Board::addWordGrid(string word, string position)
{
	//map<string, string>::iterator it = positionWordsPlacedGrid.find(position);
	//word$ = it->second;

	int lineNum = whichLine(position);
	int columnNum = whichColumn(position);
	char direction = position[2];

	if ('V' == direction || 'v' == direction) addVertical(word, lineNum, columnNum); //add vertical word
	if ('H' == direction || 'h' == direction) addHorizontal(word, lineNum, columnNum); //add horizontal word

	positionWordsPlacedGrid[position] = word; //updates the "$$$" to the word
	
}

bool Board::unusedWordGrid(string word)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 244);
	string errorMessage = "\nThe word was already used!\n\n";
	map<string, string>::iterator it = positionWordsPlacedGrid.begin();

	for (it; it != positionWordsPlacedGrid.end(); it++) //loop in map
	{
		if (it->second == word) //if word equal, it exists
		{
			cout << errorMessage; //error
			return false; 
		}
	}

	return true;
}

bool Board::checkSpace4WordGrid(string word, string position)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	string errorMessage = "\nThe word you are trying to insert, doesn't fit in the board!\n\n";

		//process the position string into line number, column number and direction
		unsigned int lineNum = whichLine(position);
		unsigned int columnNum = whichColumn(position);
		char direction = position[2];

		//change color
		SetConsoleTextAttribute(hConsole, 244);
		unsigned int wSize = word.size();
		
		//check if the beginning is out of boundaries or not
		if (lineNum + 1 > lines || columnNum + 1 > columns) { cout << errorMessage; return false; }
		//checks if there are space for the word, depending on the initial position, and the word size
		//checks several conditions
		if ('V' == direction || 'v' == direction) {
			if (0 == lineNum) {  //beginning of the column
				for (unsigned int i = 0; i < wSize; i++) //checks every position
					if (!(layout[columnNum][i] == '.' || layout[columnNum][i] == '$' || layout[columnNum][i] == word[i] || word[i] == '?')) {
						cout << errorMessage;  return false;
					}
				if (wSize < lines)
					if (!(layout[columnNum].at(wSize) == '#' || layout[columnNum].at(wSize) == '.' || layout[columnNum].at(wSize) == '$')) { // checks the final position +1
						cout << errorMessage; return false;
					}
			}
			else {
				if (lineNum + wSize > lines) {
					cout << errorMessage; return false;
				}
				for (unsigned int i = 0; i < wSize; i++) //all position checking
					if (!(layout[columnNum][i + lineNum] == '.' || layout[columnNum][i + lineNum] == '$' || layout[columnNum][i + lineNum] == word[i] || word[i] == '?')) {
						cout << errorMessage; return false;
					}
				if (lineNum + wSize < lines)
					if (!(layout[columnNum].at(wSize + lineNum) == '#' || layout[columnNum].at(wSize + lineNum) == '.' || layout[columnNum].at(wSize + lineNum) == '$')) { //empty space in the end
						cout << errorMessage; return false;
					}
				if (!(layout[columnNum].at(lineNum - 1) == '#' || layout[columnNum].at(lineNum - 1) == '.' || layout[columnNum].at(lineNum - 1) == '$')) { //empty space in the beginning
					cout << errorMessage; return false;
				}
			}
		}
		else if ('H' == direction || 'h' == direction)
		{
			if (0 == columnNum)
			{
				for (unsigned int i = 0; i < wSize; i++)
					if (!(layout[i][lineNum] == '.' || layout[i][lineNum] == '$' || layout[i][lineNum] == word[i] || word[i] == '?')) { //all positions
						cout << errorMessage; return false;
					}
				if (columnNum + wSize < columns)
					if (!(layout.at(wSize)[lineNum] == '#' || layout.at(wSize)[lineNum] == '.' || layout.at(wSize)[lineNum] == '$')) { // checks the final position +1
						cout << errorMessage; return false;
					}
			}
			else {
				if (columnNum + wSize > columns) {
					cout << errorMessage; return false;
				}
				for (unsigned int i = 0; i < wSize; i++) //all position checking
					if (!(layout[i + columnNum][lineNum] == '.' || layout[i + columnNum][lineNum] == '$' || layout[i + columnNum][lineNum] == word[i] || word[i] == '?')) {
						cout << errorMessage; return false;
					}
				if (columnNum + wSize < columns)
					if (!(layout.at(wSize + columnNum)[lineNum] == '#' || layout.at(wSize + columnNum)[lineNum] == '$' || layout.at(wSize + columnNum)[lineNum] == '.')) {  //empty space in the end
						cout << errorMessage; return false;
					}
				if (!(layout.at(columnNum - 1)[lineNum] == '#' || layout.at(columnNum - 1)[lineNum] == '$' || layout.at(columnNum - 1)[lineNum] == '.')) { //empty space in the beginning
					cout << errorMessage; return false;
				}
			}
		}
	map<string, string>::iterator it = positionWordsPlacedGrid.find(position);

	string word$ = it->second;

	if (word$.size() == word.size()) return true; //if word to be put has the right size
	SetConsoleTextAttribute(hConsole, 244);
	cout << errorMessage;
	SetConsoleTextAttribute(hConsole, 15);	
	return false;
}

void Board::removeWordGrid(string position)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int line = whichLine(position);
	int column = whichColumn(position);
	char direction = position[2];
	// verificar se a posiçao no map é valida
	// retirar a palavra do map
	// retirar a palavra do board
	string errorMessage = "\n\nThere aren't any words to remove in that position!\n";
	map<string, string>::iterator it = positionWordsPlaced.find(position);
	string word$ = it->second;
	for (unsigned int i = 0; i < word$.size(); i++)
	{
		if (word$[i] == '$') {
			SetConsoleTextAttribute(hConsole, 244);
			cout << errorMessage;
			SetConsoleTextAttribute(hConsole, 15);
			return;
		}
	}

	//positionWordsPlaced.erase(it);
	if ('V' == direction || 'v' == direction) removeVerticalGrid(line, column);
	else if ('H' == direction || 'h' == direction) removeHorizontalGrid(line, column);

	for (unsigned int i = 0; i < word$.size(); i++)
		word$[i] = '$';
	positionWordsPlacedGrid[position] = word$; //update the word to "$$$"
}

void Board::removeVerticalGrid(int line, int column)
{
	int i = 0;
	while (true)
	{
		if (line + i == lines) return;//end of line
		if (layout[column][line + i] == '#') return;//if there is a '#' in the end, return
		if (crossedWordsVertical(column, line + i))//if letter here
		{
			i++; continue;//next position
		}
		else layout[column][line + i] = '$';//substitute letter to '$'
		i++;
	}
}

void Board::removeHorizontalGrid(int line, int column)
{
	int i = 0;
	while (true)
	{
		if (column + i == columns) return; //end of column
		if (layout[column + i][line] == '#') return; //if there is a '#' in the end, return
		if (crossedWordsHorizontal(column + i, line)) //if letter here
		{
			i++; continue; //next position
		}
		else layout[column + i][line] = '$'; //substitute letter to '$'
		i++;
	}
}

bool Board::finishedGrid()
{
	string notFinishedGrid = "Your board is wrong! The word %s is not correct!";
	map<string, string>::iterator it = positionWordsPlacedGrid.begin(); //beginning of the map

	for (it; it != positionWordsPlacedGrid.end(); it++) //loop in the map
	{
		string word$ = it->second; //saves word in the word$
		for (unsigned int i = 0; i < word$.size(); i++) //loop in the word$
			if (word$[i] == '$') return false; //if there is $, grid not finished
	}
	//if all words filled, continues
	it = positionWordsPlaced.begin(); //inicialized in the original map beginning
	map<string, string>::iterator itGrid = positionWordsPlacedGrid.begin(); //itGrid in map 

	while (it != positionWordsPlaced.end() || itGrid != positionWordsPlacedGrid.end()) //while end of maps not reached
	{
		if (it->second != itGrid->second) //if not equal
		{
			printf(notFinishedGrid.c_str(), (itGrid->second).c_str()); //prints wrong word
			return false; //returns
		
		}
		it++; //adds to the iterator
		itGrid++; //adds to the iterator
	}
	return true;
}

map<string, string> Board::positionWords()
{
	return positionWordsPlaced; //return the map
}

string Board::wordInPosition(string position)
{
	map<string, string>::iterator it = positionWordsPlaced.find(position); //it points to the position

	return it->second; //return the respective word of the position asked
}

bool Board::checkPosition(string position)
{
	map<string, string>::iterator it = positionWordsPlacedGrid.find(position); //iterator pointing to the position
	if (it != positionWordsPlacedGrid.end()) //if it is not in the end, the position exists
	{
		return true;
	}

	else return false;
}