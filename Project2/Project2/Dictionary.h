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

class Dictionary
{
private: 
	map <string, vector <string> > wordSynonyms;
	ifstream f;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

public:
	Dictionary();
	~Dictionary();
};

