#include "Dictionary.h"

Dictionary::Dictionary()
{
}

Dictionary::~Dictionary()
{
}

bool Dictionary::isHeadline(string line)
{
	int doisPontos, virgula;
	doisPontos = line.find(":"); //gives position in line of ':'
	virgula = line.find(","); //gives position in the line of ','
	if (doisPontos == -1) return false; //if : is in position -1, doesnt exist
	else if (doisPontos < virgula) return true; //if : under ',', it is an headline
	else return false; //is any, it is not an headline
}

string Dictionary::singleWord(string &Line) //retirar a proxima palavra da linha
{
	string delimiter = ":, "; //delimitadores
							  //place where the delimiter will be
							  //check if the first char is a letter
							  //and if not cut the chars till we reach a letter, and then, a word
	while (!((Line[0] >= 65 && Line[0] <= 90) || (Line[0] >= 97 && Line[0] <= 122))) //cuts black spaces before the word
	{
		Line.erase(0, 1);
		//cout << ".\n";
	}
	int found = Line.find_first_of(delimiter); //gives position of first delimiter found
	int lineSize = Line.size(); //gives line size
	//individual word
	string singleWord = Line.substr(0, found);
	//erase the given words
	if (-1 == found) Line.erase(0, lineSize); //if -1, there is no delimiter, erases everything
	else if (found < lineSize) Line.erase(0, found + 1); //there is delimiter, erases the word and the delimiter
	else Line.erase(0, found); //erases the rest of the line

	return singleWord; //return the word extracted
}

bool Dictionary::validLine(string Line)
{
	//check if a line is valid by comparing char by char, so lines with [] and {} are ignored
	for (unsigned int i = 0; i < Line.size(); i++)
	{
		//lines only with letters, ':', ',', ' ', '-'
		if (!((Line[i] >= 65 && Line[i] <= 90) || (Line[i] >= 97 && Line[i] <= 122) || Line[i] == 32 || Line[i] == 58 || Line[i] == 44 || Line[i] == 45))
		{
			return false; //if there is any out of the permitted
		}
	}
	return true; //if all verified
}

bool Dictionary::wildcardMatch(const char *str, const char *strWild)
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

bool Dictionary::loadToProgram()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	string completeLine;
	string word;
	string headline;

	f.open(fileNameInput); //opens file
	if (!f.is_open()) //if file not open -> inexistent
	{
		SetConsoleTextAttribute(hConsole, 244);
		cerr << "File " << fileNameInput << " could not be found!\nTry again!\n"; //error message
		SetConsoleTextAttribute(hConsole, 15);
		return false; //return false, so the name can be corrected in the function that called
	}

	cout << "loading...\n";

	//first iteration
	do
	{
		getline(f, completeLine);
	} while (!validLine(completeLine));

	//line and headline analysis
	while (!f.eof()) 
	{
		vector <string> synonyms;
		while (completeLine != "\0") //while line not finished
		{
			if (isHeadline(completeLine)) //if headline confirmed
			{
				headline = singleWord(completeLine); //saves the word here
				transform(headline.begin(), headline.end(), headline.begin(), ::toupper); //->uppercase
			}
			else //if not headline
			{ 
				word = singleWord(completeLine); //saves next word in line
				transform(word.begin(), word.end(), word.begin(), ::toupper); //->uppercase
				synonyms.push_back(word); //saves in the vector
			}
		}
		wordSynonyms[headline] = synonyms; //saves in map

		//next line
		do 
		{
			getline(f, completeLine);
		} while (!validLine(completeLine));
	}
	cout << "end of loading of words and synonyms\n";
	f.close(); //closing of file
	return true; //file opened with success
}

bool Dictionary::headlineExists(string word)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //color in program
	SetConsoleTextAttribute(hConsole, 244); //red on white error color
	string errorMessage = "\nThe word doesn't belong in the dictionary!\n\n"; //error message
	map<string, vector<string>>::iterator it; //iterator of map

	it = wordSynonyms.find(word); //marks the iterator in the key word found
	if (it != wordSynonyms.end()) //if the key not found
	{
		return true; //true when does exist
	}
	cout << errorMessage; //prints error message
	return false; //the word exist in the board
}

vector<string> Dictionary::matchingWords(string wildCard)
{
	vector<string> resultWord; //vector of matching words
	map<string, vector<string>>::iterator it = wordSynonyms.begin(); //iterator in the beginning of the map
	int maxNumber = 10;

	for (it = wordSynonyms.begin(); it != wordSynonyms.end(); it++) //loop in the map
	{
		if (maxNumber == 0) break; //if maxnumber reached, returns
		if (wildcardMatch(it->first.c_str(), wildCard.c_str())) //if match
		{
			resultWord.push_back(it->first); //adding word to vector
			maxNumber--; //maxnumber decreasing
		}
	}
	return resultWord; //return vector
}

string Dictionary::synonymsWord(string word)
{
	map<string, vector<string> >::iterator it = wordSynonyms.find(word); //iterator marking the word asked
	vector<string> synonyms = it->second; //saves vector here

	string synonym = synonyms[rand() % synonyms.size()]; //random synonym from vector
	return synonym; //returns the synonym
}
