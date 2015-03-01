/* ************************************************************
Project: Chess C++
Author: William Koppelberger
Date: 2/27/15
	
Description: Simple Chess game written in Visual Studios C++
	to practice my C++ skills. This program utilizes 
	Polymorphism in the GamePiece objects. This is a simple
	Chess game that does not fully implement every chess
	aspect, but is playable.
References: http://xoax.net/cpp/crs/console/lessons/Lesson43/
   ***********************************************************/
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

/* Abstract Base Class to parent each Game Piece */
class GamePiece
{
private:
	char color;

	/* Virtual allows this function to be redefined in a derived class
	while preserving its calling properties through references */
	virtual bool isLegalPieceMove(int sRow, int sCol,
		int dRow, int dCol, GamePiece *board[8][8]) = 0;
	
public:
	GamePiece(char myColor) : color(myColor) { }	// Initialization list
	~GamePiece() { }

	// Members of GamePiece class
	virtual char getName() = 0;
	char getColor() { return color; }
	bool isMoveLinear(int, int, int, int, GamePiece*[8][8]);
	bool isMoveDiagonal(int, int, int, int, GamePiece*[8][8]);
	
	// Check if move is out of bounds and can move to destination
	bool isLegalMove(int sRow, int sCol, int dRow,
		int dCol, GamePiece *board[8][8])
	{
		GamePiece *destination = board[dRow][dCol];

		// Check space occupation & if my color != destination color
		if (destination == nullptr || color != destination->getColor())
			return isLegalPieceMove(sRow, sCol, dRow, dCol, board);
		else
			return false;
	}
};

/* Determines if the Piece is moving linearly(Up, Down, Left, Right) */
bool GamePiece::isMoveLinear(int sRow, int sCol, int dRow, int dCol, GamePiece *board[8][8])
{
	if (sCol == dCol) {
		if (sRow > dRow) {	// Moving Up

			// Check if spaces BETWEEN source and destination are empty
			for (int i = 1; i < (sRow - dRow); i++) {
				if (board[sRow - i][sCol] != nullptr)
					return false;
			}

			return true;
		}
		else {				// Moving Down

			// Check if spaces BETWEEN source and destination are empty
			for (int i = 1; i < (dRow - sRow); i++) {
				if (board[sRow + i][sCol] != nullptr)
					return false;
			}

			return true;
		}
	}
	else if (sRow == dRow) {
		if (sCol > dCol) {	// Moving Left

			// Check if spaces BETWEEN source and destination are empty
			for (int i = 1; i < (sCol - dCol); i++) {
				if (board[sRow][sCol - i] != nullptr)
					return false;
			}

			return true;
		}
		else {				// Moving Right

			// Check if spaces BETWEEN source and destination are empty
			for (int i = 1; i < (dCol - sCol); i++) {
				if (board[sRow][sCol + i] != nullptr)
					return false;
			}

			return true;
		}
	}

	return false;
}

/* Determines if the Piece is moving Diagonally */
bool GamePiece::isMoveDiagonal(int sRow, int sCol, int dRow, int dCol, GamePiece *board[8][8])
{
	// Check if movement is diagonal
	if (dCol - sCol == dRow - sRow || dCol - sCol == sRow - dRow) {

		// Check if spaces BETWEEN source and destination are empty
		int rowDirection = (dRow - sRow > 0) ? 1 : -1;
		int colDirection = (dCol - sCol > 0) ? 1 : -1;
		int tmpRow, tmpCol;

		for (tmpRow = sRow + rowDirection, tmpCol = sCol + colDirection;
			tmpRow != dRow;
			tmpRow = tmpRow + rowDirection, tmpCol = tmpCol + colDirection)
		{
			if (board[tmpRow][tmpCol] != nullptr)
				return false;
		}

		return true;
	}

	return false;
}

/* Sub-Class for the King piece */
class King : public GamePiece
{
private:

	// Pure virtual function
	bool isLegalPieceMove(int sRow, int sCol,
		int dRow, int dCol, GamePiece *board[8][8])
	{
		// Check moves: Up(1), Right(1), Down(1), Left(1), Diagonals(1)
		if (sRow - dRow <= 1
			&& sRow - dRow >= -1
			&& sCol - dCol <= 1
			&& sCol - dCol >= -1)
			return true;
		else
			return false;
	}

public:
	King(char color) : GamePiece(color) { }
	~King() { }

	char getName() { return 'K'; }	// Pure virtual function
};

/* Sub-Class for the Queen piece */
class Queen : public GamePiece 
{
private:

	// Pure virtual function
	bool isLegalPieceMove(int sRow, int sCol,
		int dRow, int dCol, GamePiece *board[8][8])
	{
		// Check if Queen movement is legal
		if (isMoveLinear(sRow, sCol, dRow, dCol, board) ||
			isMoveDiagonal(sRow, sCol, dRow, dCol, board))
			return true;

		return false;
	}

public:
	Queen(char color) : GamePiece(color) { }
	~Queen() { }

	char getName() { return 'Q'; }	// Pure virtual function
};

/* Sub-Class for the Bishop piece */
class Bishop : public GamePiece 
{
private:

	// Pure virtual function
	bool isLegalPieceMove(int sRow, int sCol,
		int dRow, int dCol, GamePiece *board[8][8])
	{
		return isMoveDiagonal(sRow, sCol, dRow, dCol, board);
	}

public:
	Bishop(char color) : GamePiece(color) { }
	~Bishop() { }

	char getName() { return 'B'; }	// Pure virtual function
};

/* Sub-Class for the Knight piece */
class Knight : public GamePiece
{
private:

	// Pure virtual function
	bool isLegalPieceMove(int sRow, int sCol,
		int dRow, int dCol, GamePiece *board[8][8])
	{
		// Check if Knight movement is legal
		if (dRow == sRow - 1 || dRow == sRow + 1)
			if (dCol == sCol - 2 || dCol == sCol + 2)
				return true;

		if (dRow == sRow - 2 || dRow == sRow + 2)
			if (dCol == sCol - 1 || dCol == sCol + 1)
				return true;

		return false;
	}

public:
	Knight(char color) : GamePiece(color) { }
	~Knight() { }

	char getName() { return 'N'; }	// Pure virtual function
};

/* Sub-Class for the Rook piece */
class Rook : public GamePiece
{
private:

	// Pure virtual function
	bool isLegalPieceMove(int sRow, int sCol,
		int dRow, int dCol, GamePiece *board[8][8])
	{
		return isMoveLinear(sRow, sCol, dRow, dCol, board);
	}
		
public:
	Rook(char color) : GamePiece(color) { }
	~Rook() { }

	char getName() { return 'R'; }	// Pure virtual function
};

/* Sub-Class for the Pawn piece */
class Pawn : public GamePiece
{
private:

	// Pure virtual function
	bool isLegalPieceMove(int sRow, int sCol,
		int dRow, int dCol, GamePiece *board[8][8])
	{
		GamePiece *destination = board[dRow][dCol];

		// Check if (forward one) destination is unoccupied
		if (destination == nullptr) {
			if (getColor() == 'W') {
				if (sRow - 1 == dRow && sCol == dCol)
					return true;
			}
			else {
				if (sRow + 1 == dRow && sCol == dCol)
					return true;
			}
		}
		else {	// Occupied by opponent (diagonal forward 1)
			if (sCol - 1 == dCol || sCol + 1 == dCol)
				if (destination->getColor() == 'W') {
					if (sRow + 1 == dRow)
						return true;
				}
				else {
					if (sRow - 1 == dRow)
						return true;
				}
		}
		
		return false;
	}

public:
	Pawn(char color) : GamePiece(color) { }
	~Pawn() { }

	char getName() { return 'P'; }	// Pure virtual function
};

/* Contains the game board used to keep track and display the game pieces */
class GameBoard
{
public:
	GamePiece *board[8][8];

	GameBoard();
	~GameBoard();
	void drawBoard();
	bool isInCheck(char);
};

/* Constructor definition of GameBoard */
GameBoard::GameBoard()
{
	// Initialize board pointers to nullptr
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			board[i][j] = nullptr;

	// Allocate and position White pieces
	for (int i = 0; i < 8; i++)
		board[6][i] = new Pawn('W');
	
	board[7][0] = new Rook('W');
	board[7][1] = new Knight('W');
	board[7][2] = new Bishop('W');
	board[7][3] = new King('W');
	board[7][4] = new Queen('W');
	board[7][5] = new Bishop('W');
	board[7][6] = new Knight('W');
	board[7][7] = new Rook('W');

	// Allocate and position Black pieces
	for (int i = 0; i < 8; i++)
		board[1][i] = new Pawn('B');

	board[0][0] = new Rook('B');
	board[0][1] = new Knight('B');
	board[0][2] = new Bishop('B');
	board[0][3] = new King('B');
	board[0][4] = new Queen('B');
	board[0][5] = new Bishop('B');
	board[0][6] = new Knight('B');
	board[0][7] = new Rook('B');
}

/* Desctructor defintion of GameBoard */
GameBoard::~GameBoard()
{
	for (int r = 0; r < 8; r++)
		for (int c = 0; c < 8; c++) {
			delete board[r][c];
			board[r][c] = nullptr;
		}
}

/* Displays the game board in the command prompt */
void GameBoard::drawBoard()
{
	for (int i = 0; i < 8; i++) {
		if (i % 2 == 0)
			cout << "  *****       *****       *****       *****      " << endl << i + 1 << " ";
		else
			cout << "        *****       *****       *****       *****" << endl << i + 1 << " ";

		for (int j = 0; j < 8; j++) {
			if (board[i][j] == nullptr)
				cout << "  -   ";
			else
				cout << " " << board[i][j]->getColor() << ":" << board[i][j]->getName() << "  ";
		}
	
		
		if (i % 2 == 0)
			cout << endl << "  *****       *****       *****       *****      " << endl;
		else
			cout << endl << "        *****       *****       *****       *****" << endl;
	}					

	cout << endl;
	for (int k = 0; k < 8; k++)
		cout << "    " << (char)(65 + k) << " ";

	cout << endl << endl;
}

/* Chess game engine/object */
class Chess 
{
private:
	GameBoard chessBoard;
	char playerTurn;

public:
	Chess() { playerTurn = 'W'; }
	~Chess() { }

	void advanceGame();
	bool isGameOver();
	void changePlayerTurn();
	void runGame();
};

/* Advances game until game is over */
void Chess::runGame()
{
	chessBoard.drawBoard();
	cout << endl << playerTurn << "'s turn" << endl;

	do {
		advanceGame();
		chessBoard.drawBoard();
		cout << endl << playerTurn << "'s turn" << endl;
	} while (!isGameOver());

	changePlayerTurn();
	cout << "Winner: " << playerTurn << endl;
}

/* Attempts to advance game one move */
void Chess::advanceGame()
{
	bool goodInput = false;
	string source, dest;
	int srcRow, srcCol, dstRow, dstCol;

	do {
		// Retrieve user input
		cout << "Enter source Piece(@#): ";
		getline(cin, source);
		cout << endl << "Enter destination Piece(@#): ";
		getline(cin, dest);
		cout << endl;

		// Check user input
		if (source.length() < 2)
			continue;
		if (dest.length() < 2)
			continue;

		if ((int)source.at(0) >= 65 && (int)source.at(0) <= 72
			&& (int)source.at(1) >= 49 && (int)source.at(1) <= 56
			&& (int)dest.at(0) >= 65 && (int)dest.at(0) <= 72
			&& (int)dest.at(1) >= 49 && (int)dest.at(1) <= 56)
			goodInput = true;
	} while (!goodInput);

	srcRow = (int)source.at(1) - 49;
	srcCol = (int)source.at(0) - 65;
	dstRow = (int)dest.at(1) - 49;
	dstCol = (int)dest.at(0) - 65;

	// Attempt to move source to destination
	GamePiece *current = chessBoard.board[srcRow][srcCol];
	if ((current != nullptr) && 
		(current->getColor() == playerTurn) && 
		(current->isLegalMove(srcRow, srcCol, dstRow, dstCol, chessBoard.board)))
	{
		// Make move, change turn, free memory of dead piece
		GamePiece *tmp = chessBoard.board[dstRow][dstCol];
		chessBoard.board[dstRow][dstCol] = chessBoard.board[srcRow][srcCol];
		chessBoard.board[srcRow][srcCol] = nullptr;
		delete tmp;
		changePlayerTurn();
	}
}

/* Determines if the game is over */
bool Chess::isGameOver()
{
	int count = 0;

	for (int r = 0; r < 8; r++)
		for (int c = 0; c < 8; c++)
			if (chessBoard.board[r][c] != nullptr &&
				chessBoard.board[r][c]->getName() == 'K')
				count++;

	if (count != 2)
		return true;

	return false;
}

/* Changes player turn from W-B or B-W */
void Chess::changePlayerTurn()
{
	if (playerTurn == 'W')
		playerTurn = 'B';
	else
		playerTurn = 'W';
}

/* Main */
int main()
{	
	Chess myGame;
	myGame.runGame();

	return 0;
}