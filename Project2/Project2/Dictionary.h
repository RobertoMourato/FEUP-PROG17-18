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
	map <string, vector <string> > wordSynonyms;
	ifstream f;
	

public:
	Dictionary();
	~Dictionary();
	string fileNameInput;

	bool isHeadline(string line);

	string singleWord(string &Line); //retirar a proxima palavra da linha

	bool validLine(string Line);

	bool wildcardMatch(const char *str, const char *strWild);

	bool loadToProgram();

	bool headlineExists(string word);

	vector<string> matchingWords(string wildCard);

	string synonymsWord(string word);
};

