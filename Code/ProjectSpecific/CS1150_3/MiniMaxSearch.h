/******************************************************************************/
/*!
\file MiniMaxSearch.h
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #03
\date 22/03/2012
\brief
Header file for MiniMax Search
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#ifndef _MINIMAXSEARCH_H_
#define _MINIMAXSEARCH_H_

#include <vector>
#include <iterator>
#include <iostream>

#define MIN_VALUE		-9
#define MAX_VALUE		 9
#define INFINITY		1000

class TicTacToe
{
	int Board[3][3];
	void Place(int row, int column, int turn);
	void UnPlace(int row, int column, int turn);
public:
	bool WinTest();
	bool LooseTest();
	bool DrawTest();

	bool MiniMax(unsigned int depth = 9);
	int X(int r, int c, int depth, int alpha, int beta);
	int O(int r, int c, int depth, int alpha, int beta);
	bool TerminalTest(int & terminalValue, int depth);
	int EvaluationTest();

	TicTacToe();
	enum TicTacToeCellValue{o,x,EMPTY};
	int Minimum(int n1,int n2);
	int Maximum(int n1,int n2);
	bool checkWin(int turn);
	std::vector<std::pair<int,int>> findEmptyCells();
	std::pair<int,int> optimalComputerMove;
	unsigned long firstMoveCheck;
	void reInitBoard();
	bool PlayerMove(int row,int column);
	bool computerMove(int row,int column);
	int TicTacToe::findThreeLengthsOpenFor(int turn);
};

#endif
