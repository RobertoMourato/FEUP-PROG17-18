#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <Windows.h>
using namespace std;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


class Tabuleiro
{
private:
	unsigned int lines, columns;
	vector <vector <char> > layout; //bigger vector "lines", smaller vectors are columns
	vector <char> newEmpty; //needed to fill the vector
							//vector <string> wordsPlaced; //to be erased
	map <string, string> positionWordsPlaced; //to handle add and remove words, check on repeated words and output/input for/from files
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //console colors

public:
	Tabuleiro();
	~Tabuleiro();

	bool ResumeBoard();


};

