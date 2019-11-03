#ifndef BOARD_H
#define BOARD_H

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "piece.h"

class Board {

	public:
	Board();
	void drawBoard(); 
	void clearBoard(); 
	bool isGameOver(); 
	int getTile(int y, int x);
	void addPiece(int piece[5][5], int y, int x);
	bool detectCollision(int piece[5][5], int y, int x);
	void deleteFinishedLines();

	private:
	Piece *pieceP;

	// The number of rows and columns of the board
	const static int rows = 20;
	const static int columns = 10;
	int tiles[rows][columns]; // Holds the values of all tiles on the board

};

#endif
