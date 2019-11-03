#ifndef PIECE_H
#define PIECE_H

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

class Piece {

	public:
	Piece();
	int getPieceTiles(int piece, int y, int x);

	private:
	// The templates for every piece type (I, S left and right, L left and right and T)
	// When a game piece is created it will take the shape of one of these templates, but its values will be randomised
	int pieceTemplates[6][5][5] = {
		// I Piece
		{
			{0,0,0,0,0},
			{0,0,0,0,0},
			{1,1,1,1,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		// S Piece Right
		{
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,1,1,0},
			{0,1,1,0,0},
			{0,0,0,0,0}
		},
		// S Piece Left
		{
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,1,1,0,0},
			{0,0,1,1,0},
			{0,0,0,0,0}
		},
		// L Piece Right
		{
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,1,1,1,0},
			{0,1,0,0,0},
			{0,0,0,0,0}
		},
		// L Piece Left
		{
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,1,1,1,0},
			{0,0,0,1,0},
			{0,0,0,0,0}
		},
		// T Piece
		{
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,1,1,1,0},
			{0,0,1,0,0},
			{0,0,0,0,0}
		}
	};

};

#endif
