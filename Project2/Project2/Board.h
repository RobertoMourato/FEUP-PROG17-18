#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <Windows.h>
#include <cstdlib>
using namespace std;



class Board
{
private:
	unsigned int lines, columns;
	vector <vector <char> > layout; //bigger vector "lines", smaller vectors are columns
	vector <char> newEmpty; //needed to fill the vector
	map <string, string> positionWordsPlaced; //to handle add and remove words, check on repeated words and output/input for/from files
	
public:
	~Board();

	Board();

	Board(unsigned int lines, unsigned int columns);
	
	void addVertical(string word, int line, int column);

	void addHorizontal(string word, int line, int column);

	bool crossedWordsVertical(unsigned int column, unsigned int line);

	bool crossedWordsHorizontal(unsigned int column, unsigned int line);

	void removeVertical(int line, int column);

	void removeHorizontal(int line, int column);

	void printInFile(fstream *f, string fileName);

	void loadFromFile(fstream *f);
	
	void pointFill();

	void show();

	int whichLine(string position);

	int whichColumn(string position);

	void addWord(string word, string position);

	void removeWord(string position);

	bool checkSpace4Word(string word, string position);

	bool unusedWord(string word);

	bool validPosition(string position);

	void getLettersRight(string &word, string position);

	vector<string> verifyHorizontal();

	vector<string> verifyVertical();

	void extraction();

	void hashtagFill();

	void reExtraction(string outputFile);

	void grid();

	void addVerticalGrid(string word, int line, int column);

	void addHorizontalGrid(string word, int line, int column);

	void loadFromFileGrid(fstream *f);

	void addWordGrid(string word, string position);

	bool unusedWordGrid(string word, vector<string> wordsplaced);

	bool checkSpace4WordGrid(string word, string position);

	void clues();
};