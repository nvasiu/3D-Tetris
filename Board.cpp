#include "board.h"

Board::Board() { // When the board is created all of its tiles are empty
	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < columns; x++) {
			tiles[y][x] = 0;
		}
	}
}

void Board::clearBoard() { // Zeroes all the tiles of the board
	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < columns; x++) {
			tiles[y][x] = 0;
		}
	}
}

void Board::drawBoard() {

	// Draws all the filled tiles of the board
	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < columns; x++) {
			if (tiles[y][x] != 0) {

				switch(tiles[y][x]) { // Depending on the value of the current tile draw it with the color:
					case 1: glColor3f(1.0, 0.0, 1.0); // Purple
						break;
					case 2: glColor3f(1.0, 0.0, 0.0); // Red
						break;
					case 3: glColor3f(1.0, 1.0, 0.0); // Yellow
						break;
					case 4: glColor3f(0.0, 1.0, 0.0); // Green
						break;
					case 5: glColor3f(1.0, 0.5, 0.0); // Orange
						break;
				}

				glBegin(GL_POLYGON);
				glVertex3f( x+2, y+1, 0 );
				glVertex3f( x+2, y+2, 0 );
				glVertex3f( x+1, y+2, 0 );
				glVertex3f( x+1, y+1, 0 );
				glEnd();

				glBegin(GL_POLYGON);
				glVertex3f( x+1, y+2, 0 );
				glVertex3f( x+1, y+2, 1 );
				glVertex3f( x+1, y+1, 1 );
				glVertex3f( x+1, y+1, 0 );
				glEnd();

				glBegin(GL_POLYGON);
				glVertex3f( x+2, y+2, 0 );
				glVertex3f( x+2, y+2, 1 );
				glVertex3f( x+2, y+1, 1 );
				glVertex3f( x+2, y+1, 0 );
				glEnd();

				glBegin(GL_POLYGON);
				glVertex3f( x+2, y+1, 1 );
				glVertex3f( x+2, y+2, 1 );
				glVertex3f( x+1, y+2, 1 );
				glVertex3f( x+1, y+1, 1 );
				glEnd();

				glBegin(GL_POLYGON);
				glVertex3f( x+2, y+1, 0 );
				glVertex3f( x+2, y+1, 1 );
				glVertex3f( x+1, y+1, 1 );
				glVertex3f( x+1, y+1, 0 );
				glEnd();

				glBegin(GL_POLYGON);
				glVertex3f( x+2, y+2, 0 );
				glVertex3f( x+2, y+2, 1 );
				glVertex3f( x+1, y+2, 1 );
				glVertex3f( x+1, y+2, 0 );
				glEnd();

			}
		}
	}

	// Draws the grid of the board
	glColor3f(0.0, 0.0, 0.0);
	for (int y = 1; y <= rows; y++) {
		for (int x = 1; x <= columns; x++) {

		glBegin(GL_LINES);
		glVertex3f( x, y, 0 );
		glVertex3f( x+1, y, 0 );
		glVertex3f( x+1, y+1, 0 );
		glVertex3f( x, y+1, 0 );
		glVertex3f( x, y, 0 );
		glEnd();

		glBegin(GL_LINES);
		glVertex3f( x, y, 0 );
		glVertex3f( x, y+1, 0 );
		glVertex3f( x, y+1, 1 );
		glVertex3f( x, y, 1 );
		glVertex3f( x, y, 0 );
		glEnd();

		glBegin(GL_LINES);
		glVertex3f( x+1, y, 0 );
		glVertex3f( x+1, y+1, 0 );
		glVertex3f( x+1, y+1, 1 );
		glVertex3f( x+1, y, 1 );
		glVertex3f( x+1, y, 0 );
		glEnd();

		glBegin(GL_LINES);
		glVertex3f( x, y, 1 );
		glVertex3f( x+1, y, 1 );
		glVertex3f( x+1, y+1, 1 );
		glVertex3f( x, y+1, 1 );
		glVertex3f( x, y, 1 );
		glEnd();

		glBegin(GL_LINES);
		glVertex3f( x, y, 0 );
		glVertex3f( x+1, y, 0 );
		glVertex3f( x+1, y, 1 );
		glVertex3f( x, y, 1 );
		glVertex3f( x, y, 0 );
		glEnd();

		glBegin(GL_LINES);
		glVertex3f( x, y+1, 0 );
		glVertex3f( x+1, y+1, 0 );
		glVertex3f( x+1, y+1, 1 );
		glVertex3f( x, y+1, 1 );
		glVertex3f( x, y+1, 0 );
		glEnd();

		}
	}

}

bool Board::isGameOver() { // If the top row of the board has tiles filled the game is over

	for (int x = 0; x < columns; x++) {
		if (tiles[19][x] != 0) return true;
	}
	return false;

}

int Board::getTile(int x, int y) { // Returns the value of a tile
	
	return tiles[y][x];

}

void Board::addPiece(int piece[5][5], int y, int x) { // Takes a piece and its coordinates and adds it to the board

	for (int boardy = y, piecey = 0; boardy < y+5; boardy++, piecey++) {
		for (int boardx = x, piecex = 0; boardx < x+5; boardx++, piecex++) {
			
			if( piece[piecey][piecex] != 0 ) {
				tiles[boardy][boardx] = piece[piecey][piecex];
			}

		}
	}

}

bool Board::detectCollision(int piece[5][5], int y, int x) { // Check if the given piece at the given coordinates collides with the wall/ a filled tile

	for (int boardy = y, piecey = 0; boardy < y+5; boardy++, piecey++) {
		for (int boardx = x, piecex = 0; boardx < x+5; boardx++, piecex++) {
			
			if (boardx < 0 || boardx >= columns || boardy < 0 || boardy >= rows) {
				if (piece[piecey][piecex] != 0) return true;
			} else {
				if ( (piece[piecey][piecex] != 0) && (tiles[boardy][boardx] != 0) ) return true;
			}

		}
	}

	return false;

}

void Board::deleteFinishedLines() { // Check for lines that are completely filled

	for (int i = 0; i < 4; i++) { // Check 4 times, because, at most, 4 lines can be filled at once
	for (int y = 0; y < rows; y++) {
		
		int x = 0;
		while (x < columns) {
			if (tiles[y][x] == 0) break;
			x++;
		}

		if (x == columns) { // If the entire row is filled then drop all rows above it
			for (int aboveY = y; aboveY < rows-1; aboveY++) {
				for (int aboveX = 0; aboveX < columns; aboveX++) {
					tiles[aboveY][aboveX] = tiles[aboveY+1][aboveX];
				}
			}
		}

	}
	}

}
