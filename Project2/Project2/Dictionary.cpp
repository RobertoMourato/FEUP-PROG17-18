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
	doisPontos = line.find(":");
	virgula = line.find(",");
	if (doisPontos == -1) return false;
	else if (doisPontos < virgula) return true;
	else return false;
}

string Dictionary::singleWord(string &Line) //retirar a proxima palavra da linha
{
	string delimiter = ":, "; //delimitadores
							  //place where the delimiter will be
							  //check if the first char is a letter
							  //and if not cut the chars till we reach a letter, and then, a word
	while (!((Line[0] >= 65 && Line[0] <= 90) || (Line[0] >= 97 && Line[0] <= 122)))
	{
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

bool Dictionary::validLine(string Line)
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

	f.open(fileNameInput);
	if (!f.is_open()) 
	{
		SetConsoleTextAttribute(hConsole, 244);
		cerr << "File " << fileNameInput << " could not be found!\nTry again!\n";
		SetConsoleTextAttribute(hConsole, 15);
		return false;
	}

	cout << "loading...\n";
	//do while aqui
	//first line, except lines with "[ (...) ]"
	do
	{
		getline(f, completeLine);
	} while (!validLine(completeLine));

	//line and headline analysis
	while (!f.eof()) 
	{
		vector <string> synonyms;
		while (completeLine != "\0") 
		{
			if (isHeadline(completeLine)) 
			{
				//i++;
				//wordSynonyms.push_back(newEmptyVector);
				//wordSynonyms[i].push_back(singleWord(completeLine));
				headline = singleWord(completeLine);
				transform(headline.begin(), headline.end(), headline.begin(), ::toupper); //->uppercase
			}
			else 
			{
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
		do 
		{
			getline(f, completeLine);
		} while (!validLine(completeLine));
	}
	cout << "end of loading of words and synonyms\n";
	f.close();
	return true;
}

bool Dictionary::headlineExists(string word)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 244);
	string errorMessage = "\nThe word doesn't belong in the dictionary!\n\n";
	map<string, vector<string>>::iterator it = wordSynonyms.begin();

	it = wordSynonyms.find(word);
	if (it != wordSynonyms.end()) 
	{
		return true;
	}
	cout << errorMessage;
	return false;
}

vector<string> Dictionary::matchingWords(string wildCard)
{
	vector<string> resultWord;
	map<string, vector<string>>::iterator it = wordSynonyms.begin();
	int maxNumber = 10;

	for (it = wordSynonyms.begin(); it != wordSynonyms.end(); it++) 
	{
		if (maxNumber == 0) break;
		if (wildcardMatch(it->first.c_str(), wildCard.c_str())) 
		{
			resultWord.push_back(it->first);
			maxNumber--;
		}
	}
	return resultWord;
}