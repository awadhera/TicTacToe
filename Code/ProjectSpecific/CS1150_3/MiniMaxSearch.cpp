/******************************************************************************/
/*!
\file MiniMaxSearch.cpp
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #03
\date 22/03/2012
\brief
Cpp file for MiniMax algorithm
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#include "MiniMaxSearch.h"

/******************************************************************************/
/*!
Place an x or o in the position specified
\param [in] row
index of row
\param [in] column
index of column
\param [in] turn
turn - x or o
\return
void
*/
/******************************************************************************/
void TicTacToe::Place(int row, int column, int turn)
{
	Board[row][column] = turn;
}

/******************************************************************************/
/*!
UnPlace an x or o in board
\param [in] row
index of row
\param [in] column
index of column
\param [in] turn
turn - x or o
\return
void
*/
/******************************************************************************/
void TicTacToe::UnPlace(int row, int column, int turn)
{
	if(Board[row][column] == turn)
		Board[row][column] = EMPTY;
}

/******************************************************************************/
/*!
Constructor
*/
/******************************************************************************/
TicTacToe::TicTacToe()
{
	for(unsigned int i = 0;i < 3; ++i)
	{
		for(unsigned int j = 0;j < 3;++j)
		{
			Board[i][j] = EMPTY;
		}
	}
}

/******************************************************************************/
/*!
Find the minimum of 2 numbers
\param [in] n1
first number
\param [in] n2
second number
\return
int - the minimum of the two
*/
/******************************************************************************/
int TicTacToe::Minimum(int n1,int n2)
{
	return((n1<n2)?n1:n2);
}

/******************************************************************************/
/*!
Find the maximum of 2 numbers
\param [in] n1
first number
\param [in] n2
second number
\return
int - the maximum of the two
*/
/******************************************************************************/
int TicTacToe::Maximum(int n1,int n2)
{
	return((n1>n2)?n1:n2);
}

/******************************************************************************/
/*!
Check if computer wins
\return
bool - true if computer wins
*/
/******************************************************************************/
bool TicTacToe::WinTest()
{
	return(checkWin(x));
}

/******************************************************************************/
/*!
Check if computer looses
\return
bool - true if computer lost
*/
/******************************************************************************/
bool TicTacToe::LooseTest()
{
	return(checkWin(o));
}

/******************************************************************************/
/*!
Check if game is a draw
\return
int - true if game is a draw
*/
/******************************************************************************/
bool TicTacToe::DrawTest()
{
	for(unsigned int i = 0;i < 3;++i)
	{
		for(unsigned int j = 0;j < 3; ++j)
		{
			if(Board[i][j]==EMPTY)
			{
				return false;
			}
		}
	}
	if(!checkWin(x) && !checkWin(o))	
		return true;
	else
		return false;
}

/******************************************************************************/
/*!
Check if the player with 'turn' wins
\param [in] turn
x or o
\return
bool - true if turn wins
*/
/******************************************************************************/
bool TicTacToe::checkWin(int turn)
{
	for(unsigned int i = 0;i < 3;++i)
	{
		for(unsigned int j = 0;j < 3;++j)
		{
			if(Board[i][j]!=turn)
				break;
			else if(Board[i][j]==turn && j==2)
				return true;
		}
	}
	for(unsigned int j = 0;j < 3;++j)
	{
		for(unsigned int i = 0;i < 3;++i)
		{
			if(Board[i][j]!=turn)
				break;
			else if(Board[i][j]==turn && i==2)
				return true;
		}
	}
	if((Board[0][0]==turn && Board[1][1]==turn && Board[2][2]==turn)
		||(Board[0][2]==turn && Board[1][1]==turn && Board[2][0]==turn))
		return true;
	else
		return false;
}

/******************************************************************************/
/*!
Terminal test of the game
\param [out] terminalValue
output terminalValue
\param [in] depth
Depth of Minimax algorithm
\return
bool - true if terminalTest is true
*/
/******************************************************************************/
bool TicTacToe::TerminalTest(int & terminalValue, int depth)
{
	if(WinTest())
	{
		terminalValue = MAX_VALUE;
		return true;
	}
	else if(LooseTest())
	{
		terminalValue = MIN_VALUE;
		return true;
	}
	else if(DrawTest())
	{
		terminalValue = 0;
		return true;
	}
	else if(depth == 0)
	{
		terminalValue = EvaluationTest();
		return true;
	}
	else
	{
		return false;
	}
}

/******************************************************************************/
/*!
Main Minimax algorithm
\param [in] depth
depth of the algorithm
\return
bool - True if Minimax runs successfully 
*/
/******************************************************************************/
bool TicTacToe::MiniMax(unsigned int depth)
{
	firstMoveCheck = 0;
	int V = -INFINITY;
	V = X(-1,-1,depth,-INFINITY,INFINITY);
	if(V!=-INFINITY)
		return true;
	else 
		return false;
}

/******************************************************************************/
/*!
Function to handle computer's turn in the game
\param [in] r
row
\param [in] c
colum
\param [in] depth
depth of the algorithm
\param [in] alpha
alpha value
\param [in] beta
beta value
\return
int - Return the state value
*/
/******************************************************************************/
int TicTacToe::X(int r, int c, int depth, int alpha, int beta)
{
	firstMoveCheck++;
	int V = -INFINITY;
	std::vector<std::pair<int,int>> freeCellsPositions = findEmptyCells();
	std::vector<std::pair<int,int>>::iterator i;
	for(i = freeCellsPositions.begin();i!= freeCellsPositions.end();++i)
	{
		Place((*i).first,(*i).second,x);
		int terminalValue;
		if(TerminalTest(terminalValue,depth))
		{
			UnPlace((*i).first,(*i).second,x);
			int temp = V;
			V = Maximum(V,terminalValue);
			if(temp!=V && firstMoveCheck==1)
			{
				optimalComputerMove.first = (*i).first;
				optimalComputerMove.second = (*i).second;
			}
			if(V>=beta)
			{
				firstMoveCheck--;
				return V;
			}
			alpha = Maximum(alpha,V);
			continue;
		}
		int temp = V;
		V = Maximum(V,O(r,c,depth-1,alpha,beta));
		if(temp!=V && firstMoveCheck == 1)
		{
			optimalComputerMove.first = (*i).first;
			optimalComputerMove.second = (*i).second;
		}
		if(V >= beta)
		{
			UnPlace((*i).first,(*i).second,x);
			firstMoveCheck--;
			return V;
		}
		alpha = Maximum(alpha,V);
		UnPlace((*i).first,(*i).second,x);
	}
	firstMoveCheck--;
	return V;
}

/******************************************************************************/
/*!
Function to handle player's turn in the game
\param [in] r
row
\param [in] c
colum
\param [in] depth
depth of the algorithm
\param [in] alpha
alpha value
\param [in] beta
beta value
\return
int - Return the state value
*/
/******************************************************************************/
int TicTacToe::O(int r,int c,int depth,int alpha,int beta)
{
	firstMoveCheck++;
	int V = INFINITY;
	std::vector<std::pair<int,int>> freeCellsPositions = findEmptyCells();
	std::vector<std::pair<int,int>>::iterator i;
	for(i = freeCellsPositions.begin();i!=freeCellsPositions.end();++i)
	{
		Place((*i).first,(*i).second,o);
		int terminalValue;
		if(TerminalTest(terminalValue,depth))
		{
			UnPlace((*i).first,(*i).second,o);
			V = Minimum(V,terminalValue);
			if(V<=alpha)
			{
				firstMoveCheck--;
				return V;
			}
			beta = Minimum(beta,V);
			continue;
		}
		V = Minimum(V, X(r,c,depth-1,alpha,beta));
		if(V<=alpha)
		{
			UnPlace((*i).first,(*i).second,o);
			firstMoveCheck--;
			return V;
		}
		beta = Minimum(beta,V);
		UnPlace((*i).first,(*i).second,o);
	}
	firstMoveCheck--;
	return V;
}

/******************************************************************************/
/*!
Function to find empty cells in the ame
\return
vector of pairs - Indices of the board holding the empty cells
*/
/******************************************************************************/
std::vector<std::pair<int,int>> TicTacToe::findEmptyCells()
{
	std::vector<std::pair<int,int>> freeCellsPositions;
	for(int i = 0;i < 3; ++i)
	{
		for(int j = 0;j < 3;++j)
		{
			if(Board[i][j]==EMPTY)
				freeCellsPositions.push_back(std::make_pair(i,j));
		}
	}
	return freeCellsPositions;
}

/******************************************************************************/
/*!
Reinitialize board
\return
void
*/
/******************************************************************************/
void TicTacToe::reInitBoard()
{
	for(unsigned int i = 0;i < 3; ++i)
	{
		for(unsigned int j = 0;j < 3;++j)
		{
			Board[i][j] = EMPTY;
		}
	}
}

/******************************************************************************/
/*!
Function to place player's move
\param [in] row
row index of board
\param [in] column 
colum index of board
\return
bool - true if player was able to make his move
*/
/******************************************************************************/
bool TicTacToe::PlayerMove(int row,int column)
{
	if(Board[row][column]==EMPTY)
	{
		Board[row][column] = o;
		return true;
	}
	return false;
}

/******************************************************************************/
/*!
Function to place computer's move
\param [in] row
row index of board
\param [in] column 
colum index of board
\return
bool - true if computer was able to make his move
*/
/******************************************************************************/
bool TicTacToe::computerMove(int row,int column)
{
	if(Board[row][column] == EMPTY)
	{
		Board[row][column] = x;
		return true;
	}
	return false;
}

/******************************************************************************/
/*!
Evaluation Test for MiniMax
\return
int - Evaluation value(cutoff value)
*/
/******************************************************************************/
int TicTacToe::EvaluationTest()
{
	int threeLengthsForX = findThreeLengthsOpenFor(x);
	int threeLengthsForO = findThreeLengthsOpenFor(o);
	int f = threeLengthsForX - threeLengthsForO;
	return f;
}

/******************************************************************************/
/*!
Find possible winning chances for x or o 
\param [in] turn
x or o
\return
int - Number of winning chances
*/
/******************************************************************************/
int TicTacToe::findThreeLengthsOpenFor(int turn)
{
	int count = 0;
	for(int i = 0;i < 3; ++i)
	{
		for(int j = 0;j < 3;++j)
		{
			if(Board[i][j]!=turn && Board[i][j]!=EMPTY)
				break;
			else if((Board[i][j]==turn || Board[i][j]==EMPTY) && j==2)
				count++;
		}
	}
	for(int j = 0;j < 3;++j)
	{
		for(int i = 0;i < 3;++i)
		{
			if(Board[i][j]!=turn && Board[i][j]!=EMPTY)
				break;
			else if((Board[i][j]==turn || Board[i][j]== EMPTY) && i==2)
				count++;
		}
	}
	if((Board[0][0]==turn || Board[0][0]==EMPTY) && (Board[1][1]==turn || Board[1][1]==EMPTY) && (Board[2][2]==turn || Board[2][2]==EMPTY))
		count++;
	if((Board[0][2]==turn || Board[0][2]==EMPTY) && (Board[1][1]==turn || Board[1][1]==EMPTY) && (Board[2][0]==turn || Board[2][0]==EMPTY))
		count++;
	return count;
}