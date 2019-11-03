#include "piece.h"

Piece::Piece() {

}

int Piece::getPieceTiles(int piece, int y, int x) { // Returns if the given x and y of a piece is 0 or 1

	return pieceTemplates[piece][y][x];

}
