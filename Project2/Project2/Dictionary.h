#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <Windows.h> 
#include <ctime>
#include <stdlib.h>
using namespace std;

class Dictionary
{
private:
	map <string, vector <string> > wordSynonyms; //map with the word and the vector with synonyms
	ifstream f; //open the file

public:
	//constructor
	Dictionary();

	//destructor
	~Dictionary();

	//name of input file
	string fileNameInput;

	//returns true if the words exists in the dictionary map
	bool isHeadline(string line);

	//separates the word from the line of the dictionary
	string singleWord(string &Line); //retirar a proxima palavra da linha

	//returns true if the line is valid
	bool validLine(string Line);

	//returns true if the word and the wildcard mathces
	bool wildcardMatch(const char *str, const char *strWild);

	//load the dictionary from the file to the program
	bool loadToProgram();

	//sees if the word that the user tried to input belongs to the dictionary
	bool headlineExists(string word);

	//searches for matching words, and return a 10-elements-maximum-size vector
	vector<string> matchingWords(string wildCard);

	//returns a synonym of the word asked
	string synonymsWord(string word);
};

