 #include "Board.h"


Board::Board()
{
	lines = 10;  //default size
	columns = 10; //default size
}//constructor 

Board::Board(unsigned int lines, unsigned int columns)
{
	this->lines = lines;
	this->columns = columns;
}	//constructor with arguments	

Board::~Board()
{

}

void Board::addVertical(string word, int line, int column)
{
	if (0 < line) layout[column].at(line - 1) = '#';

	for (unsigned int i = 0; i < word.size(); i++)
		layout[column][line + i] = word[i];
	if (line + word.size() < lines) layout[column].at(line + word.size()) = '#';
}

void Board::addHorizontal(string word, int line, int column)
{
	if (0 < column) layout.at(column - 1)[line] = '#';
	for (unsigned int i = 0; i < word.size(); i++)
		layout[column + i][line] = word[i];
	if (column + word.size() < columns) layout.at(column + word.size())[line] = '#';
}

bool Board::crossedWordsVertical(unsigned int column, unsigned int line)
{
	if (column > 0)
	{
		if (layout[column - 1][line] >= 65 && layout[column - 1][line] <= 90)
			return true;
	}
	if (column < columns)
	{
		if (layout[column + 1][line] >= 65 && layout[column + 1][line] <= 90)
			return true;
	}
	return false;
}

bool Board::crossedWordsHorizontal(unsigned int column, unsigned int line)
{
	if (line > 0)
	{
		if (layout[column][line - 1] >= 65 && layout[column][line - 1] <= 90)
			return true;
	}
	if (line < lines)
	{
		if (layout[column][line + 1] >= 65 && layout[column][line + 1] <= 90)
			return true;
	}
	return false;
}

void Board::removeVertical(int line, int column)
{
	if (0 < line)
	{
		if (!crossedWordsVertical(column, line - 1))
		{
			if (layout[column].at(line - 1) == '#') layout[column].at(line - 1) = '.';

		}

	}

	int i = 0;
	while (true)
	{
		if (line + i == lines) return;
		if (layout[column][line + i] == '#' && layout[column][line + i + 1] == '.')
		{
			layout[column][line + i] = '.';
			return;
		}
		if (layout[column][line + i] == '#') return;
		if (crossedWordsVertical(column, line + i))
		{
			i++; continue;
		}
		else layout[column][line + i] = '.';
		i++;
	}
}

void Board::removeHorizontal(int line, int column)
{
	if (0 < column)
	{
		if (!crossedWordsHorizontal(column - 1, line))
		{
			if (layout.at(column - 1)[line] == '#') layout.at(column - 1)[line] = '.';
		}
	}


	int i = 0;
	while (true)
	{
		if (column + i == columns) return;
		if (layout[column + i][line] == '#' && layout[column + i + 1][line] == '.')
		{
			layout[column + i][line] = '.';
			return;
		}
		if (layout[column + i][line] == '#') return;

		if (crossedWordsHorizontal(column + i, line))
		{
			i++; continue;
		}
		else layout[column + i][line] = '.';
		i++;
	}
}

void Board::printInFile(fstream *f, string dictFile)
{
	//name and space
	*f << dictFile << endl << endl;

	//the board
	for (unsigned int j = 0; j < lines; j++)
	{
		for (unsigned int i = 0; i < columns; i++)
		{
			*f << layout[i][j] << " ";
		}
		*f << endl;
	}
	//the position and word
	*f << endl;
	map<string, string>::iterator it;

	for (it = positionWordsPlaced.begin(); it != positionWordsPlaced.end(); it++)
	{
		*f << (*it).first << " " << (*it).second;
		*f << endl;
	}
}

void Board::pointFill()
{
	//fill the board with points
	char point = '.';
	//	layout:	outer vector columns
	//			inner vector lines
	for (unsigned int i = 0; i < columns; i++)
	{
		layout.push_back(newEmpty);
		for (unsigned int j = 0; j < lines; j++)
		{
			layout[i].push_back(point);
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
	for (unsigned int i = 0; i < columns; i++)
	{

		char b = a + i;
		cout << " " << b;
	}
	cout << endl;
	for (unsigned int j = 0; j < lines; j++)
	{
		SetConsoleTextAttribute(hConsole, REDblack);
		char B = A + j;
		cout << " " << B << " ";
		for (unsigned int i = 0; i < columns; i++)
		{
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
	int lineNum = whichLine(position);
	int columnNum = whichColumn(position);
	char direction = position[2];

	if ('V' == direction || 'v' == direction) addVertical(word, lineNum, columnNum);
	if ('H' == direction || 'h' == direction) addHorizontal(word, lineNum, columnNum);

	positionWordsPlaced[position] = word;
}

void Board::removeWord(string position)
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
	if (it != positionWordsPlaced.end())
	{
		positionWordsPlaced.erase(it);
		if ('V' == direction || 'v' == direction) removeVertical(line, column);
		else if ('H' == direction || 'h' == direction) removeHorizontal(line, column);
	}
	else
	{
		SetConsoleTextAttribute(hConsole, 244);
		cout << errorMessage;
		SetConsoleTextAttribute(hConsole, 15);
	}
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
	SetConsoleTextAttribute(hConsole, 244);
	string errorMessage = "\nThe word was already used!\n\n";
	map<string, string>::iterator it = positionWordsPlaced.begin();

	for (it; it != positionWordsPlaced.end(); it++)
	{
		if (it->second == word)
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
	string errorMessageChar = "\nOnly upper case and down case letters are, respectively, allowed in the first and second place!\n";
	string errorMessageOutofBounds = "\nIt looks like the position %s doesn't exist in this board!\n";
	string errorMessageDirection = "\nThat direction doesn't exist. Choose between vertical 'V' or horizontal 'H'!\n";

	if (!(direction == 'v' || direction == 'V' || direction == 'h' || direction == 'H'))
	{
		cout << errorMessageDirection;
		return false;
	}
	else if (!(line >= 65 && line <= 90))
	{
		cout << errorMessageChar;
		return false;
	}
	else if (!(column >= 97 && column <= 122))
	{
		cout << errorMessageChar;
		return false;
	}
	else if (lineN > lines)
	{
		printf(errorMessageOutofBounds.c_str(), position.c_str());
		return false;
	}
	else if (columnN > columns)
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
		for (unsigned int i = 0; i < word.size(); i++)
		{
			if (layout[column][line + i] != '.') word[i] = layout[column][line + i];
		}
	else if ('h' == direction || 'H' == direction)
		for (unsigned int i = 0; i < word.size(); i++)
		{
			if (layout[column + i][line] != '.') word[i] = layout[column + i][line];
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
	do {
		n++;
		fileOutput = "b";
		docType = ".txt";
		existingFile = false;
		if (n < 10) fileOutput += "00" + to_string(n) + docType;
		else if (n < 100) fileOutput += "0" + to_string(n) + docType;
		else fileOutput += to_string(n) + docType;

		f.open(fileOutput);
		if (f.is_open())
		{
			existingFile = true;
			f.close();
		}
		else {
			f.open(fileOutput, ios::out | ios::in | ios::trunc);
		}
	} while (existingFile);

	printInFile(fA, dictFile);
	cout << "The extraction was successfully made to " << fileOutput << " file!";
	f.close();
}

void Board::hashtagFill()
{
	for (unsigned int i = 0; i < columns; i++)
		for (unsigned int j = 0; j < lines; j++)
			if (layout[i][j] == '.')
				layout[i][j] = '#';
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

	f.open(outputFile);

	printInFile(fA, dictFile);
	cout << "The extraction was successfully made to " << outputFile << " file!";
	f.close();
}

void Board::grid()
{
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
			if (layout[i][j] == '#' || layout[i][j] == '.')
			{
				cout << " ";
				SetConsoleTextAttribute(hConsole, BLACKblack);
				cout << layout[i][j];
			}
			else if (layout[i][j] == '$')
			{
				SetConsoleTextAttribute(hConsole, WHITEwhite);
				cout << " " << layout[i][j];
			}
			else
			{
				SetConsoleTextAttribute(hConsole, SOFTGRAYblack);
				cout << " " << layout[i][j];
			}

		}
		cout << endl;
	}
	SetConsoleTextAttribute(hConsole, WHITEblack);
}
//
void Board::addVerticalGrid(string word, int line, int column)
{
	if (0 < line) layout[column].at(line - 1) = '#';

	for (unsigned int i = 0; i < word.size(); i++)
		layout[column][line + i] = '$';
	if (line + word.size() < lines) layout[column].at(line + word.size()) = '#';
}
//
void Board::addHorizontalGrid(string word, int line, int column)
{
	if (0 < column) layout.at(column - 1)[line] = '#';
	for (unsigned int i = 0; i < word.size(); i++)
		layout[column + i][line] = '$';
	if (column + word.size() < columns) layout.at(column + word.size())[line] = '#';
}
//
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
//
void Board::addWord1stGrid(string word, string position)
{
	int lineNum = whichLine(position);
	int columnNum = whichColumn(position);
	char direction = position[2];

	if ('V' == direction || 'v' == direction) addVerticalGrid(word, lineNum, columnNum);
	if ('H' == direction || 'h' == direction) addHorizontalGrid(word, lineNum, columnNum);

	positionWordsPlaced[position] = word;

	for (unsigned int i = 0; i < word.size(); i++)
		word[i] = '$';
	positionWordsPlacedGrid[position] = word;

}
//
void Board::addWordGrid(string word, string position)
{
	//map<string, string>::iterator it = positionWordsPlacedGrid.find(position);
	//word$ = it->second;

	int lineNum = whichLine(position);
	int columnNum = whichColumn(position);
	char direction = position[2];

	if ('V' == direction || 'v' == direction) addVertical(word, lineNum, columnNum);
	if ('H' == direction || 'h' == direction) addHorizontal(word, lineNum, columnNum);

	positionWordsPlacedGrid[position] = word;
	
}
//
bool Board::unusedWordGrid(string word)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 244);
	string errorMessage = "\nThe word was already used!\n\n";
	map<string, string>::iterator it = positionWordsPlacedGrid.begin();

	for (it; it != positionWordsPlacedGrid.end(); it++)
	{
		if (it->second == word)
		{
			cout << errorMessage;
			return false;
		}
	}

	return true;
}
//2nd
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

	if (word$.size() == word.size()) return true;
	SetConsoleTextAttribute(hConsole, 244);
	cout << errorMessage;
	SetConsoleTextAttribute(hConsole, 15);	
	return false;
}

void Board::clues()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//SetConsoleTextAttribute(hConsole, 244);
	cout << "THERE'S THE POSITION AND A SYNONIM OF EACH WORD TO BE GUESSED\n\n";
	map<string, string>::iterator it = positionWordsPlaced.begin();
	cout << "VERTICAL WORDS\n";
	for (it; it != positionWordsPlaced.end(); it++) {
		if (it->first[2] == 'V') {
			cout << it->first[0] + it->first[1] << "" << "  ";
		}
	}
	cout << "HORIZONTAL WORDS\n\n";
	it = positionWordsPlaced.begin();

	for (it; it != positionWordsPlaced.end(); it++) {

	}
}
//2nd
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
	positionWordsPlacedGrid[position] = word$;
}
//
void Board::removeVerticalGrid(int line, int column)
{
	int i = 0;
	while (true)
	{
		if (line + i == lines) return;
		if (layout[column][line + i] == '#') return;
		if (crossedWordsVertical(column, line + i))
		{
			i++; continue;
		}
		else layout[column][line + i] = '$';
		i++;
	}
}
//
void Board::removeHorizontalGrid(int line, int column)
{
	int i = 0;
	while (true)
	{
		if (column + i == columns) return;
		if (layout[column + i][line] == '#') return;
		if (crossedWordsHorizontal(column + i, line))
		{
			i++; continue;
		}
		else layout[column + i][line] = '$';
		i++;
	}
}
//
bool Board::finishedGrid()
{
	string notFinishedGrid = "Your board is wrong! The word %s is not correct!";
	map<string, string>::iterator it = positionWordsPlacedGrid.begin();

	for (it; it != positionWordsPlacedGrid.end(); it++)
	{
		string word$ = it->second;
		for (unsigned int i = 0; i < word$.size(); i++)
			if (word$[i] == '$') return false;
	}

	//map<string, string>::iterator 
	it = positionWordsPlaced.begin();
	map<string, string>::iterator itGrid = positionWordsPlacedGrid.begin();

	while (it != positionWordsPlaced.end() || itGrid != positionWordsPlacedGrid.end())
	{
		if (it->second != itGrid->second)
		{
			printf(notFinishedGrid.c_str(), (itGrid->second).c_str());
			return false;
		
		}
		it++;
		itGrid++;
	}
	return true;
}
//
map<string, string> Board::positionWords()
{
	return positionWordsPlaced;
}
//
string Board::wordInPosition(string position)
{
	map<string, string>::iterator it = positionWordsPlaced.find(position);

	return it->second;
}