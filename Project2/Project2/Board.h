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
	unsigned int lines, columns; //number of lines and columns
	vector <vector <char> > layout; //bigger vector "lines", smaller vectors are columns
	vector <char> newEmpty; //needed to fill the vector
	map <string, string> positionWordsPlaced; //to handle add and remove words, check on repeated words and output/input for/from files
	map <string, string> positionWordsPlacedGrid; //handle the cwplayer grid
	
public:
	//constructor
	Board();
	//argument constructor
	Board(unsigned int lines, unsigned int columns);
	//destructor
	~Board();
	//add word in vertical direction
	void addVertical(string word, int line, int column);
	//add word in horizontal direction
	void addHorizontal(string word, int line, int column);
	//checks for words that are crossed with the ones being removed
	bool crossedWordsVertical(unsigned int column, unsigned int line);
	bool crossedWordsHorizontal(unsigned int column, unsigned int line);

	//remove vertical and horizontal directed words, respectively
	void removeVertical(int line, int column);
	void removeHorizontal(int line, int column);

	//print name of dictionary, board and position-word in file
	void printInFile(fstream *f, string fileName);

	//load the information from the file on the program
	void loadFromFile(fstream *f);
	
	//fill the board with points (beginning)
	void pointFill();

	//prints the vector of the board in the console
	void show();

	//converts char ascii code to lines/columns position
	int whichLine(string position);
	int whichColumn(string position);

	//does primitive job before sending to the right adding
	void addWord(string word, string position);

	//primitive job before actually removing
	void removeWord(string position);

	//sees if there is space for the word
	bool checkSpace4Word(string word, string position);

	//sees if the word was already used
	bool unusedWord(string word);

	//sees if the position is valid
	bool validPosition(string position);

	//when helping, searches in the board for letters in the middle
	void getLettersRight(string &word, string position);

	//final validation
	vector<string> verifyHorizontal();
	vector<string> verifyVertical();

	//info extraction
	void extraction(string dictFile);
	void reExtraction(string dictFile, string outputFile);

	//fill the remaining . in the end of the creation of the file
	void hashtagFill();

	//player board
	void grid();

	//player addiction of the words
	void addVerticalGrid(string word, int line, int column);
	void addHorizontalGrid(string word, int line, int column);
	//1st add of the "words" in the board and normal add
	void addWord1stGrid(string word, string position);
	void addWordGrid(string word, string position);

	//load from file to player program
	void loadFromFileGrid(fstream *f);

	//used word in the player program
	bool unusedWordGrid(string word);

	//checks for space in the player program
	bool checkSpace4WordGrid(string word, string position);

	//remove word in player grid
	void removeWordGrid(string position);
	void removeVerticalGrid(int line, int column);
	void removeHorizontalGrid(int line, int column);

	//verification of finishing game
	bool finishedGrid();

	//returns the map with position and right word
	map<string, string> positionWords();

	//returns the word in the position asked
	string wordInPosition(string position);

	//reset the board to every position with '.'
	void boardReset();

	//checks if position exists
	bool checkPosition(string position);
};