#include <chrono>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>

#include "board.h"
#include "piece.h"

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

bool Playing = true; // Whether the game is over or not

float lastTime = 0;
float currentTime = 0;
float timeInterval = 200;

double camAngle = 0;
double armLowerAngle = 0;
double armUpperAngle = 0;

Board *boardP;
Piece *pieceP;
int currentPiece[5][5]; // Holds tiles of the current piece
int rotatedPiece[5][5]; // Holds tiles of the current piece rotated CCW 90 degrees, for checking if the rotation causes a collision
int pieceX, pieceY; // The x and y coordinates of the current piece
int input_key; // The value of the key input by the user
bool newPiece = false; // Flag for the creation of a new piece, triggered when the current piece lands

int getRandomNum(int max) {
	return rand() % max;
}

void clearPiece() { // Clears all values from the current piece

	for (int y = 0; y < 5; y++) {
		for (int x = 0; x < 5; x++) {
			currentPiece[y][x] = 0;
		}
	}

}

void createPiece() { // Fills the current piece with tiles
	
	int pieceTemplate = getRandomNum(6); // A random piece type is chosen
	pieceX = getRandomNum(6);
	pieceY = 16;

	for (int y = 0; y < 5; y++) {
		for (int x = 0; x < 5; x++) {
			if (pieceP->getPieceTiles(pieceTemplate,y,x) != 0) {
				currentPiece[y][x] = 1 + getRandomNum(5); // The tiles are randomised to 5 different values (which represent 5 different colors)
			}
		}
	}

}

void createRotatedPiece() { // Fills a temp variable with the current piece's tiles rotated CCW by 90 degrees

	for (int y = 0; y < 5; y++) {
		for (int x = 0; x < 5; x++) {
			rotatedPiece[y][x] = currentPiece[5-x-1][y];
		}
	}
	
}

void drawPiece() { // Draws the current piece on the window, the current piece is not yet added to the board

	for (int y = 0; y < 5; y++) {
		for (int x = 0; x < 5; x++) {
			if (currentPiece[y][x] != 0) {

				switch(currentPiece[y][x]) { // Depending on the value of the current tile draw it with the color:
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
				glVertex3f( pieceX+x+2, pieceY+y+1, 0 );
				glVertex3f( pieceX+x+2, pieceY+y+2, 0 );
				glVertex3f( pieceX+x+1, pieceY+y+2, 0 );
				glVertex3f( pieceX+x+1, pieceY+y+1, 0 );
				glEnd();

				glBegin(GL_POLYGON);
				glVertex3f( pieceX+x+1, pieceY+y+2, 0 );
				glVertex3f( pieceX+x+1, pieceY+y+2, 1 );
				glVertex3f( pieceX+x+1, pieceY+y+1, 1 );
				glVertex3f( pieceX+x+1, pieceY+y+1, 0 );
				glEnd();

				glBegin(GL_POLYGON);
				glVertex3f( pieceX+x+2, pieceY+y+2, 0 );
				glVertex3f( pieceX+x+2, pieceY+y+2, 1 );
				glVertex3f( pieceX+x+2, pieceY+y+1, 1 );
				glVertex3f( pieceX+x+2, pieceY+y+1, 0 );
				glEnd();

				glBegin(GL_POLYGON);
				glVertex3f( pieceX+x+2, pieceY+y+1, 1 );
				glVertex3f( pieceX+x+2, pieceY+y+2, 1 );
				glVertex3f( pieceX+x+1, pieceY+y+2, 1 );
				glVertex3f( pieceX+x+1, pieceY+y+1, 1 );
				glEnd();

				glBegin(GL_POLYGON);
				glVertex3f( pieceX+x+2, pieceY+y+1, 0 );
				glVertex3f( pieceX+x+2, pieceY+y+1, 1 );
				glVertex3f( pieceX+x+1, pieceY+y+1, 1 );
				glVertex3f( pieceX+x+1, pieceY+y+1, 0 );
				glEnd();

				glBegin(GL_POLYGON);
				glVertex3f( pieceX+x+2, pieceY+y+2, 0 );
				glVertex3f( pieceX+x+2, pieceY+y+2, 1 );
				glVertex3f( pieceX+x+1, pieceY+y+2, 1 );
				glVertex3f( pieceX+x+1, pieceY+y+2, 0 );
				glEnd();

			}
		}
	}

}

void drawArm() {

	// LOWER ARM
	glColor3f(0.2, 0.2, 0.2);
	glPushMatrix();
	glTranslatef(-2.5, 2, 0);
	glRotatef(armLowerAngle, 0, 0, 1);
	glTranslatef(2.5, -2, 0);

	glBegin(GL_POLYGON);
	glVertex3f( -2, 2, 0.2 );
	glVertex3f( -2, 14, 0.2 );
	glVertex3f( -3, 14, 0.2 );
	glVertex3f( -3, 2, 0.2 );
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f( -3, 14, 0.2 );
	glVertex3f( -3, 14, 0.8 );
	glVertex3f( -3, 2, 0.8 );
	glVertex3f( -3, 2, 0.2 );
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f( -2, 14, 0.2 );
	glVertex3f( -2, 14, 0.8 );
	glVertex3f( -2, 2, 0.8 );
	glVertex3f( -2, 2, 0.2 );
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f( -2, 2, 0.8 );
	glVertex3f( -2, 14, 0.8 );
	glVertex3f( -3, 14, 0.8 );
	glVertex3f( -3, 2, 0.8 );
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f( -2, 2, 0.2 );
	glVertex3f( -2, 2, 0.8 );
	glVertex3f( -3, 2, 0.8 );
	glVertex3f( -3, 2, 0.2 );
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f( -2, 14, 0.2 );
	glVertex3f( -2, 14, 0.8 );
	glVertex3f( -3, 14, 0.8 );
	glVertex3f( -3, 14, 0.2 );
	glEnd();

		// UPPER ARM
		glColor3f(0.4, 0.4, 0.4);
		glPushMatrix();
		glTranslatef(-2.5, 14, 0);
		glRotatef(armUpperAngle, 0, 0, 1);
		glTranslatef(2.5, -14, 0);

		glBegin(GL_POLYGON);
		glVertex3f( -2, 14, 0.2 );
		glVertex3f( -2, 24, 0.2 );
		glVertex3f( -3, 24, 0.2 );
		glVertex3f( -3, 14, 0.2 );
		glEnd();

		glBegin(GL_POLYGON);
		glVertex3f( -3, 24, 0.2 );
		glVertex3f( -3, 24, 0.8 );
		glVertex3f( -3, 14, 0.8 );
		glVertex3f( -3, 14, 0.2 );
		glEnd();

		glBegin(GL_POLYGON);
		glVertex3f( -2, 24, 0.2 );
		glVertex3f( -2, 24, 0.8 );
		glVertex3f( -2, 14, 0.8 );
		glVertex3f( -2, 14, 0.2 );
		glEnd();

		glBegin(GL_POLYGON);
		glVertex3f( -2, 14, 0.8 );
		glVertex3f( -2, 24, 0.8 );
		glVertex3f( -3, 24, 0.8 );
		glVertex3f( -3, 14, 0.8 );
		glEnd();

		glBegin(GL_POLYGON);
		glVertex3f( -2, 14, 0.2 );
		glVertex3f( -2, 14, 0.8 );
		glVertex3f( -3, 14, 0.8 );
		glVertex3f( -3, 14, 0.2 );
		glEnd();

		glBegin(GL_POLYGON);
		glVertex3f( -2, 24, 0.2 );
		glVertex3f( -2, 24, 0.8 );
		glVertex3f( -3, 24, 0.8 );
		glVertex3f( -3, 24, 0.2 );
		glEnd();

		glPopMatrix();

	glPopMatrix();

	// ARM BASE
	glColor3f(0.0, 0.0, 0.0);	

	glBegin(GL_POLYGON);
	glVertex3f( 0, 1, -2 );
	glVertex3f( 0, 2, -2 );
	glVertex3f( -5, 2, -2 );
	glVertex3f( -5, 1, -2 );
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f( -5, 2, -2 );
	glVertex3f( -5, 2, 3 );
	glVertex3f( -5, 1, 3 );
	glVertex3f( -5, 1, -2 );
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f( 0, 2, -2 );
	glVertex3f( 0, 2, 3 );
	glVertex3f( 0, 1, 3 );
	glVertex3f( 0, 1, -2 );
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f( 0, 1, 3 );
	glVertex3f( 0, 2, 3 );
	glVertex3f( -5, 2, 3 );
	glVertex3f( -5, 1, 3 );
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f( 0, 1, -2 );
	glVertex3f( 0, 1, 3 );
	glVertex3f( -5, 1, 3 );
	glVertex3f( -5, 1, -2 );
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f( 0, 2, -2 );
	glVertex3f( 0, 2, 3 );
	glVertex3f( -5, 2, 3 );
	glVertex3f( -5, 2, -2 );
	glEnd();

}

void display(void) {

	currentTime = glutGet(GLUT_ELAPSED_TIME);
	
	if (boardP->isGameOver()) { // Check if the game is over
		Playing = false;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(30.0f*sin(camAngle), 11.0f, 30.0f*cos(camAngle), 
		6.0f, 11.0f, 0.0f, 
		0.0f, 1.0f, 0.0f);

	boardP->drawBoard(); // Draws the tiles stored in the board
	drawPiece(); // Draws the current piece
	drawArm();

	glutSwapBuffers();

	if (Playing && (currentTime - lastTime) > timeInterval) {

		lastTime = currentTime;
				
		pieceY--; // Drops the current piece down by 1 tile

		if (newPiece) { // If the flag to create a new piece is triggered then
			clearPiece(); // The current piece is cleared
			createPiece(); // The current piece gets new values
			newPiece = false; 
		}

		if (boardP->detectCollision(currentPiece,pieceY-1,pieceX)) { // Check if the current piece will collide the next time it drops down
			boardP->addPiece(currentPiece,pieceY,pieceX); // The current piece is added to the board
			newPiece = true; // A new piece must be created
			boardP->deleteFinishedLines(); // Check and delete any filled lines
		}
	}

}

void pieceMovement(int key, int x, int y) { // For arrow key inputs

	int modifierKey = glutGetModifiers();

	switch(key) {			
		case GLUT_KEY_LEFT: // Check if moving the piece will cause a collision, if not the piece is moved
			if (modifierKey == GLUT_ACTIVE_CTRL) {
				camAngle += 0.1;
				if (camAngle >= 2*M_PI) camAngle -= 2*M_PI;
			} else {
				if (!boardP->detectCollision(currentPiece,pieceY,pieceX-1)) pieceX--;
			}
			break;

		case GLUT_KEY_RIGHT: // Check if moving the piece will cause a collision, if not the piece is moved
			if (modifierKey == GLUT_ACTIVE_CTRL) {
				camAngle -= 0.1;
				if (camAngle >= 2*M_PI) camAngle -= 2*M_PI;
			} else {
				if (!boardP->detectCollision(currentPiece,pieceY,pieceX+1)) pieceX++;
			}
			break;

		case GLUT_KEY_UP: 
			createRotatedPiece(); // Create a temporary piece that is the current piece rotated CCW 90 degrees
			if (!boardP->detectCollision(rotatedPiece,pieceY,pieceX)) { // If the rotated piece does not cause a collision
				for (int y = 0; y < 5; y++) {
					for (int x = 0; x < 5; x++) {
						currentPiece[y][x] = rotatedPiece[y][x]; // The current piece is replaced with the rotated version
					}
				}
			}
			break;
	}

}

void gameFunctions(unsigned char key, int x, int y) { // For letter key inputs

	switch(key) {	
		case 'q':
			glutDestroyWindow(glutGetWindow()); // Closes the game
			break;
		case 'r':
			boardP->clearBoard(); // Restartes the game
			newPiece = true;
			Playing = true;
			break;
		case 'a':
			armLowerAngle += 5;
			break;
		case 'd':
			armLowerAngle -= 5;
			break;
		case 'w':
			armUpperAngle += 5;
			break;
		case 's':
			armUpperAngle -= 5;
			break;
	}

}

int main(int argc, char** argv) {

	srand(time(NULL));
	Board gameBoard;
	boardP = &gameBoard;
	Piece gamePiece;
	pieceP = &gamePiece;
	
	createPiece();

	// Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(1600, 1600);
	glutCreateWindow("Game");
	
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(45,1,1,1000);
	glMatrixMode(GL_MODELVIEW);

	glutSpecialFunc(pieceMovement);
	glutKeyboardFunc(gameFunctions);

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutMainLoop();	

	return 0;

}
