#include "primlib.h"
#include <stdlib.h>
#include "pieces.h"
#include <time.h>


#define TRUE 1
#define FALSE 0
//#define INTERVAL 500
#define BOARD_WIDTH 20
#define BOARD_HEIGHT 30
#define CELL_WIDTH ((screenWidth() - 2* MARGIN_X)/BOARD_WIDTH)
#define CELL_HEIGHT ((screenHeight() - 2* MARGIN_Y)/BOARD_HEIGHT)
#define BLOCK_SPACING 2
#define MARGIN_X 200 //dont change
#define MARGIN_Y 50 //dont change
#define POSITION_FREE 0
#define POSITION_OCCUPIED 1
#define NUMBER_OF_ORIENTATIONS 4
#define NUMBER_OF_TYPES 7
#define BLOCK_DIMENSION 4
#define PIVOT_POINT 2
#define DELAY (1000/60) //screen refresh rate (getting keys)
#define PREVIEW_SPACING 50
#define WELCOME_MSG "Welcome to tetris"
#define PRESS_ANY_KEY_MSG "Press any key to continue"
#define YOU_DIED_MSG "You lost"
#define GAME_FALL_SPEED_INCREASE 0 //do not set this higher than 10



typedef struct {
	int array[BLOCK_DIMENSION][BLOCK_DIMENSION];
	int row;
	int column;
	int width;
	int height;
	int type;
	int orientation;
	int pivotRow;
	int pivotColumn;
} MovingPiece;

void youLostMessage();
void greetingMessage();
void checkLoss();
void shiftRowsDown(int);
void deleteFullRows();
void checkFullRows();
void addPieceToTheBoard();
void movePieceDown();
void rotatePiece();
int detectCollisions(int);
int isMoveLegal(int);
void drawBoard();
void initializeBoard ();
MovingPiece getNewPiece();
void movePiece();
void drawMovingPiece();
void previewNextPiece();

