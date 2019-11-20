#include <chrono>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <time.h>
#include <GL/glut.h>

#include "board.h"
#include "piece.h"

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

using namespace std;

bool Playing = true; // Whether the game is over or not

float lastTime = 0; 
float currentTime = 0; // The program records the current time and the time of the last game cycle
float timeInterval = 5000; // The game advances when the time between cycles is greater than this

double camAngle = 0; // The position of the camera

double armLowerLength = 12; // Size, position, angle of the lower part of the robot arm
double armLowerX = -3;
double armLowerY = 2;
double armLowerAngle = 90;

double armUpperLength = 10; // Size, position, angle of the upper part of the robot arm
double armUpperX = -3;
double armUpperY = 14;
double armUpperAngle = -90;

double armPieceX, armPieceY; // The x and y position of the game piece that the robot arm is holding

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

void drawTimer() { // Draws a count down of the time until the game piece is dropped

	int timeLeft = round((timeInterval - (currentTime - lastTime))/1000.0);
	string str1 = to_string(timeLeft);

	if (timeLeft > 1) {
		glColor3f(0.0, 0.0, 0.0);
	} else {
		glColor3f(1.0, 0.0, 0.0);
	}

	int len = str1.length();

	glPushMatrix();
	glLineWidth(10);
	glTranslatef(12,18,0);
	glScalef(0.03, 0.03, 0.03);
	for (int i = 0; i < len; i++) {
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str1[i]);
	}
	glLineWidth(1);
	glPopMatrix();
	
}

void drawPiece() { // Draws the current piece on the window, the current piece is not yet added to the board

	glPushMatrix();
	
	glTranslatef(-6, 22, 0);

	glTranslatef(3.5, 3.5, 0);
	glRotatef(-armUpperAngle+90, 0, 0, 1);
	glTranslatef(-3.5, -3.5, 0);

	glTranslatef(3.5, 3.5, 0);
	glRotatef(-armLowerAngle, 0, 0, 1);
	glTranslatef(-3.5, -3.5, 0);
	
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

	glPopMatrix();

}

void drawPiecePreview() { // Draws a copy of the current piece at an approximation of the robot arms position (this is where the piece will be dropped)

	armPieceX = round( ( (armLowerLength)*cos((armLowerAngle)*M_PI/180.0) ) + ( armUpperLength*cos((armLowerAngle+armUpperAngle)*M_PI/180.0) ) ) - 5;
	armPieceY = round( ( (armLowerLength)*sin((armLowerAngle)*M_PI/180.0) ) + ( armUpperLength*sin((armLowerAngle+armUpperAngle)*M_PI/180.0) ) ) ;

	for (int y = 0; y < 5; y++) {
		for (int x = 0; x < 5; x++) {
			if (currentPiece[y][x] != 0) {

				if (boardP->detectCollision(currentPiece,armPieceY,armPieceX)) {
					glColor3f(0.5, 0.5, 0.7);
				} else {
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
				}}

				glBegin(GL_POLYGON);
				glVertex3f( armPieceX+x+2, armPieceY+y+1, 0 );
				glVertex3f( armPieceX+x+2, armPieceY+y+2, 0 );
				glVertex3f( armPieceX+x+1, armPieceY+y+2, 0 );
				glVertex3f( armPieceX+x+1, armPieceY+y+1, 0 );
				glEnd();

				glBegin(GL_POLYGON);
				glVertex3f( armPieceX+x+1, armPieceY+y+2, 0 );
				glVertex3f( armPieceX+x+1, armPieceY+y+2, 1 );
				glVertex3f( armPieceX+x+1, armPieceY+y+1, 1 );
				glVertex3f( armPieceX+x+1, armPieceY+y+1, 0 );
				glEnd();

				glBegin(GL_POLYGON);
				glVertex3f( armPieceX+x+2, armPieceY+y+2, 0 );
				glVertex3f( armPieceX+x+2, armPieceY+y+2, 1 );
				glVertex3f( armPieceX+x+2, armPieceY+y+1, 1 );
				glVertex3f( armPieceX+x+2, armPieceY+y+1, 0 );
				glEnd();

				glBegin(GL_POLYGON);
				glVertex3f( armPieceX+x+2, armPieceY+y+1, 1 );
				glVertex3f( armPieceX+x+2, armPieceY+y+2, 1 );
				glVertex3f( armPieceX+x+1, armPieceY+y+2, 1 );
				glVertex3f( armPieceX+x+1, armPieceY+y+1, 1 );
				glEnd();

				glBegin(GL_POLYGON);
				glVertex3f( armPieceX+x+2, armPieceY+y+1, 0 );
				glVertex3f( armPieceX+x+2, armPieceY+y+1, 1 );
				glVertex3f( armPieceX+x+1, armPieceY+y+1, 1 );
				glVertex3f( armPieceX+x+1, armPieceY+y+1, 0 );
				glEnd();

				glBegin(GL_POLYGON);
				glVertex3f( armPieceX+x+2, armPieceY+y+2, 0 );
				glVertex3f( armPieceX+x+2, armPieceY+y+2, 1 );
				glVertex3f( armPieceX+x+1, armPieceY+y+2, 1 );
				glVertex3f( armPieceX+x+1, armPieceY+y+2, 0 );
				glEnd();

			}
		}
	}

}

void drawArm() { // Draw the parts of the robot arm

	// LOWER ARM
	glColor3f(0.2, 0.2, 0.2);
	glPushMatrix();
	glTranslatef(-2.5, 2, 0);
	glRotatef(armLowerAngle-90, 0, 0, 1);
	glTranslatef(2.5, -2, 0);

	glBegin(GL_POLYGON);
	glVertex3f( armLowerX+1, armLowerY, 0.2 );
	glVertex3f( armLowerX+1, armLowerY+armLowerLength, 0.2 );
	glVertex3f( armLowerX, armLowerY+armLowerLength, 0.2 );
	glVertex3f( armLowerX, armLowerY, 0.2 );
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f( armLowerX, armLowerY+armLowerLength, 0.2 );
	glVertex3f( armLowerX, armLowerY+armLowerLength, 0.8 );
	glVertex3f( armLowerX, armLowerY, 0.8 );
	glVertex3f( armLowerX, armLowerY, 0.2 );
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f( armLowerX+1, armLowerY+armLowerLength, 0.2 );
	glVertex3f( armLowerX+1, armLowerY+armLowerLength, 0.8 );
	glVertex3f( armLowerX+1, armLowerY, 0.8 );
	glVertex3f( armLowerX+1, armLowerY, 0.2 );
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f( armLowerX+1, armLowerY, 0.8 );
	glVertex3f( armLowerX+1, armLowerY+armLowerLength, 0.8 );
	glVertex3f( armLowerX, armLowerY+armLowerLength, 0.8 );
	glVertex3f( armLowerX, armLowerY, 0.8 );
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f( armLowerX+1, armLowerY, 0.2 );
	glVertex3f( armLowerX+1, armLowerY, 0.8 );
	glVertex3f( armLowerX, armLowerY, 0.8 );
	glVertex3f( armLowerX, armLowerY, 0.2 );
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f( armLowerX+1, armLowerY+armLowerLength, 0.2 );
	glVertex3f( armLowerX+1, armLowerY+armLowerLength, 0.8 );
	glVertex3f( armLowerX, armLowerY+armLowerLength, 0.8 );
	glVertex3f( armLowerX, armLowerY+armLowerLength, 0.2 );
	glEnd();

		// UPPER ARM
		glColor3f(0.4, 0.4, 0.4);
		glPushMatrix();
		glTranslatef(-2.5, 14, 0);
		glRotatef(armUpperAngle, 0, 0, 1);
		glTranslatef(2.5, -14, 0);

		glBegin(GL_POLYGON);
		glVertex3f( armUpperX+1, armUpperY, 0.2 );
		glVertex3f( armUpperX+1, armUpperY+armUpperLength, 0.2 );
		glVertex3f( armUpperX, armUpperY+armUpperLength, 0.2 );
		glVertex3f( armUpperX, armUpperY, 0.2 );
		glEnd();

		glBegin(GL_POLYGON);
		glVertex3f( armUpperX, armUpperY+armUpperLength, 0.2 );
		glVertex3f( armUpperX, armUpperY+armUpperLength, 0.8 );
		glVertex3f( armUpperX, armUpperY, 0.8 );
		glVertex3f( armUpperX, armUpperY, 0.2 );
		glEnd();

		glBegin(GL_POLYGON);
		glVertex3f( armUpperX+1, armUpperY+armUpperLength, 0.2 );
		glVertex3f( armUpperX+1, armUpperY+armUpperLength, 0.8 );
		glVertex3f( armUpperX+1, armUpperY, 0.8 );
		glVertex3f( armUpperX+1, armUpperY, 0.2 );
		glEnd();

		glBegin(GL_POLYGON);
		glVertex3f( armUpperX+1, armUpperY, 0.8 );
		glVertex3f( armUpperX+1, armUpperY+armUpperLength, 0.8 );
		glVertex3f( armUpperX, armUpperY+armUpperLength, 0.8 );
		glVertex3f( armUpperX, armUpperY, 0.8 );
		glEnd();

		glBegin(GL_POLYGON);
		glVertex3f( armUpperX+1, armUpperY, 0.2 );
		glVertex3f( armUpperX+1, armUpperY, 0.8 );
		glVertex3f( armUpperX, armUpperY, 0.8 );
		glVertex3f( armUpperX, armUpperY, 0.2 );
		glEnd();

		glBegin(GL_POLYGON);
		glVertex3f( armUpperX+1, armUpperY+armUpperLength, 0.2 );
		glVertex3f( armUpperX+1, armUpperY+armUpperLength, 0.8 );
		glVertex3f( armUpperX, armUpperY+armUpperLength, 0.8 );
		glVertex3f( armUpperX, armUpperY+armUpperLength, 0.2 );
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

	currentTime = glutGet(GLUT_ELAPSED_TIME); // Update the current time
	
	/**
	if (boardP->isGameOver()) { // Check if the game is over
		Playing = false;
	}
	**/

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(35.0f*sin(camAngle), 11.0f, 35.0f*cos(camAngle), 6.0f, 11.0f, 0.0f, 0.0f, 1.0f, 0.0f); // Position the camera to look at the board depending on angle

	boardP->drawBoard(); // Draws the tiles stored in the board
	//drawPiece();

	if (Playing) {	
		drawPiecePreview();
		drawArm();
		drawTimer();
	}

	glutSwapBuffers();

	if (Playing && (currentTime - lastTime) > timeInterval) {

		lastTime = currentTime; // Update the previous time

		if (boardP->detectCollision(currentPiece,armPieceY,armPieceX)) { // If the timer runs out and the piece is colliding then the game ends
			Playing = false;
		} else {
			boardP->addPiece(currentPiece,armPieceY,armPieceX); // If the piece is not colliding, drop it down
			newPiece = true; 
		}

		if (newPiece) { // If the flag to create a new piece is triggered then
			clearPiece(); // The current piece is cleared
			createPiece(); // The current piece gets new values
			newPiece = false; 
		}

		//pieceY--; 

		/**
		if (boardP->detectCollision(currentPiece,pieceY-1,pieceX)) { 
			boardP->addPiece(currentPiece,pieceY,pieceX);
			newPiece = true; 
			boardP->deleteFinishedLines(); 
		}
		**/
	}

}

void pieceMovement(int key, int x, int y) { // For arrow key inputs

	int modifierKey = glutGetModifiers();

	switch(key) {			
		case GLUT_KEY_LEFT: // Holding ctrl + left moves the camera to the left
			if (modifierKey == GLUT_ACTIVE_CTRL) {
				camAngle += 0.1;
				if (camAngle >= 2*M_PI) camAngle -= 2*M_PI;
			} 
			/**
			else {
				if (!boardP->detectCollision(currentPiece,pieceY,pieceX-1)) pieceX--;
			}
			**/
			break;

		case GLUT_KEY_RIGHT: // Holding ctrl + right moves the camera to the right
			if (modifierKey == GLUT_ACTIVE_CTRL) {
				camAngle -= 0.1;
				if (camAngle >= 2*M_PI) camAngle -= 2*M_PI;
			} 
			/**
			else {
				if (!boardP->detectCollision(currentPiece,pieceY,pieceX+1)) pieceX++;
			}
			**/
			break;
		case GLUT_KEY_UP: 
			createRotatedPiece(); // Create a temporary piece that is the current piece rotated CCW 90 degrees
			//if (!boardP->detectCollision(rotatedPiece,pieceY,pieceX)) {
			for (int y = 0; y < 5; y++) {
				for (int x = 0; x < 5; x++) {
					currentPiece[y][x] = rotatedPiece[y][x]; // The current piece is replaced with the rotated version
				}
			}
			//}
			break;
	}

}

void gameFunctions(unsigned char key, int x, int y) { // For letter key inputs

	switch(key) {	
		case 'q':
			glutDestroyWindow(glutGetWindow()); // Closes the game
			break;
		case 'r':
			boardP->clearBoard(); // Restarts the game: board, timer and the current piece are reset
			lastTime = currentTime;
			clearPiece();
			createPiece();
			Playing = true;
			break;
		case 'a':
			armLowerAngle += 5; // Change the angle of the arm
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
		case 32:
			if (Playing) { // Pressing space will drop the current piece if it is not colliding with something
				if (!boardP->detectCollision(currentPiece,armPieceY,armPieceX)) {
					lastTime = currentTime;
					boardP->addPiece(currentPiece,armPieceY,armPieceX);
					clearPiece();
					createPiece();
				}
			}
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
	glutCreateWindow("Assignment 2 - 3D Robot Arm Tetris");
	
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
