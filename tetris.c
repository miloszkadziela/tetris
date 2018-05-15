#include "tetris.h"


int board[BOARD_WIDTH][BOARD_HEIGHT];
MovingPiece currentMovingPiece;
MovingPiece nextMovingPiece;
int rowsFull[BOARD_HEIGHT] = {FALSE};
int currentFrame = 0;

int main(int argc, char* argv[])
{
  
  if(initGraph()) 
  {
    exit(3);
  }
  
  initializeBoard();
  greetingMessage();
  
  currentMovingPiece = getNewPiece();
  nextMovingPiece = getNewPiece();
  while (TRUE) 
  {
    drawBoard();
    previewNextPiece();
    movePiece();
    drawMovingPiece();
    movePieceDown();
    updateScreen();
    deleteFullRows();
    checkLoss();
    SDL_Delay(DELAY);
  }
  return 0;
}

void greetingMessage(void)
{
  textout(screenWidth()/4, screenHeight()/2, WELCOME_MSG, WHITE);
  textout(screenWidth()/4, screenHeight()/2 + 50, PRESS_ANY_KEY_MSG, YELLOW);
  updateScreen();
  getkey();
}

void youLostMessage(void)
{
  filledRect(0,0,screenWidth()-1,screenHeight()-1,WHITE);
  textout(screenWidth()/4, screenHeight()/2, YOU_DIED_MSG, BLACK);
  textout(screenWidth()/4, screenHeight()/2 + 50, PRESS_ANY_KEY_MSG, BLACK);
  updateScreen();
  getkey();
  exit(0);
}

void checkLoss(void)
{
  for(int column = 0; column<BOARD_WIDTH; column++)
  {
    if(board[column][0])
	{
      youLostMessage();
    }
  }
}
void deleteFullRows(void)
{
  checkFullRows();
  for(int row = 0; row<BOARD_HEIGHT; row++)
  {
    if(rowsFull[row])
	{
      shiftRowsDown(row);
    }
  }
}

void shiftRowsDown(int rowtoDelete)
{
  for(int row = rowtoDelete; row>0; row--)
  {
    for(int column = 0; column<BOARD_WIDTH; column++)
	{
      board[column][row] = board[column][row - 1];
    }
  }
  rowsFull[rowtoDelete] = FALSE;
}

void checkFullRows(void)
{
  for(int row = BOARD_HEIGHT - 1;row>=0;row--)
  {
    for(int column = 0; column < BOARD_WIDTH; column++)
	{
      if(board[column][row])
	  {
		if(column == (BOARD_WIDTH -1)) 
		{
			rowsFull[row] = TRUE;
		}
		continue;
      }
      else break;
    }
  }
}
void previewNextPiece(void)
{
  enum color pieceColor;
  for (int row = 0; row < BLOCK_DIMENSION; row++) 
  {
    for (int column = 0; column < BLOCK_DIMENSION; column++) 
	{
      if (nextMovingPiece.array[column][row]) 
	  {
		if (nextMovingPiece.array[column][row] == PIVOT_POINT) 
		{
			pieceColor = MAGENTA;
		}
		else 
		{
		pieceColor = GREEN;
		}
		filledRect((screenWidth()-MARGIN_X+PREVIEW_SPACING) + column * CELL_WIDTH,
					MARGIN_Y  + row * CELL_HEIGHT,
					((screenWidth()-MARGIN_X+PREVIEW_SPACING) + (column + 1) * CELL_WIDTH) - BLOCK_SPACING,
					(MARGIN_Y + (row + 1) * CELL_HEIGHT) - BLOCK_SPACING, 
					pieceColor);
      }
    }
  }
  textout((screenWidth()-MARGIN_X+PREVIEW_SPACING),MARGIN_Y+4*CELL_HEIGHT + 10,"NEXT PIECE", WHITE);
  
  
}
void movePieceDown(void)
{
  if (currentFrame == 25) 
  {
    detectCollisions(SDLK_DOWN);
    currentFrame = GAME_FALL_SPEED_INCREASE;
    currentMovingPiece.row += 1;
  }
  currentFrame++;
}

void checkRows(void)
{
  for (int row = 0; row < currentMovingPiece.height; row++)
  {
    for (int column = 0 ; column < currentMovingPiece.width ; column++) 
	{
      if(currentMovingPiece.array[column][row])
	  {
		board[column+currentMovingPiece.column][row+currentMovingPiece.row] = POSITION_OCCUPIED;
      }
    }
  }
  
  
}

void addPieceToTheBoard(void)
{
  for (int row = 0; row < currentMovingPiece.height; row++)
  {
    for (int column = 0 ; column < currentMovingPiece.width ; column++)
	{
      if(currentMovingPiece.array[column][row])
	  {
		board[column+currentMovingPiece.column][row+currentMovingPiece.row] = POSITION_OCCUPIED;
      }
    }
  }
  drawBoard();
  currentMovingPiece = nextMovingPiece;
  nextMovingPiece = getNewPiece();
  
}
int getMovingPieceWidth(MovingPiece temp) 
{
  for(int column = BLOCK_DIMENSION - 1; column >= 0; column--) 
  {
    for(int row = BLOCK_DIMENSION - 1; row >= 0; row--) 
	{
      if(temp.array[column][row]) 
	  {
		return column + 1;
      }
    }
  }
  return 0;
}
int getMovingPieceHeight(MovingPiece temp)
{
  for(int row = BLOCK_DIMENSION - 1; row >= 0; row--)
  {
    for(int column = BLOCK_DIMENSION - 1; column >= 0; column--)
	{
      if(temp.array[column][row])
	  {
		return row + 1;
      }
    }
  }
  return 0;
}

MovingPiece getNewPiece() 
{
  srand(time(0));
  MovingPiece temp;
  temp.type = rand() % NUMBER_OF_TYPES;
  temp.orientation = rand() % NUMBER_OF_ORIENTATIONS;
  for(int row = 0; row < BLOCK_DIMENSION; row++)
  {
    for(int column = 0; column < BLOCK_DIMENSION; column++)
	{
      if (pieces[temp.type][temp.orientation][column][row] == PIVOT_POINT) 
	  {
		temp.pivotRow = row;
		temp.pivotColumn = column;
      }
      temp.array[column][row] = pieces[temp.type][temp.orientation][column][row];
    }
  }
  temp.width = getMovingPieceWidth(temp);
  temp.height = getMovingPieceHeight(temp);
  temp.column = rand() % (BOARD_WIDTH - temp.width);
  temp.row = 0;
  return temp;
}

void movePiece()
{
  int move = pollkey();
  if (move == SDLK_LEFT && detectCollisions(move)) 
  { 
    currentMovingPiece.column -= 1;
  }
  else if (move == SDLK_RIGHT && detectCollisions(move)) 
  {
    currentMovingPiece.column += nextMovingPiece.row + 1;
  }
  else if (move == SDLK_DOWN && detectCollisions(move)) 
  {
    while(detectCollisions(move))
	{
      currentMovingPiece.row += 1;
    }
  }
  else if (move == SDLK_SPACE && detectCollisions(move)) 
  {
    rotatePiece();
  }
  else if (move == SDLK_ESCAPE) 
  {
    exit(0);
  }
  
  
}
void drawMovingPiece()
{
  enum color pieceColor;
  for (int row = 0; row < BLOCK_DIMENSION; row++) 
  {
    for (int column = 0; column < BLOCK_DIMENSION; column++) 
	{
      if (currentMovingPiece.array[column][row]) 
	  {
		if (currentMovingPiece.array[column][row] == PIVOT_POINT) 
		{
		pieceColor = MAGENTA;
		}
		else 
		{
		pieceColor = GREEN;
		}
			filledRect(MARGIN_X + (currentMovingPiece.column + column) * CELL_WIDTH,
				MARGIN_Y + (currentMovingPiece.row + row) * CELL_HEIGHT,
				(MARGIN_X + (currentMovingPiece.column + column + 1) * CELL_WIDTH) - BLOCK_SPACING,
				(MARGIN_Y + (currentMovingPiece.row + row + 1) * CELL_HEIGHT) - BLOCK_SPACING, 
				pieceColor);
      }
    }
  }
  
}


void drawBoard() 
{
  filledRect(0, 0, screenWidth() - 1, screenHeight() - 1, BLACK);
  
  for (int row = 0; row < BOARD_HEIGHT; row++) 
  {
    for (int column = 0; column < BOARD_WIDTH; column++) 
	{
      if (board[column][row] == 1) // change to == 0 (for reversed colors)
	  {
		filledRect(MARGIN_X + column * CELL_WIDTH,
				   MARGIN_Y +  row * CELL_HEIGHT,
				  (MARGIN_X + (column + 1) * CELL_WIDTH) - BLOCK_SPACING,
				  (MARGIN_Y + (row + 1) * CELL_HEIGHT) - BLOCK_SPACING,
				  RED);
      }
    }
  }
  line(MARGIN_X - 1, MARGIN_Y, MARGIN_X - 1, screenHeight() - MARGIN_Y, WHITE); //left line
  line(screenWidth() - MARGIN_X + 1, MARGIN_Y, screenWidth() -  MARGIN_X + 1, screenHeight() - MARGIN_Y, WHITE); //right line
  line(MARGIN_X - 1, screenHeight() - MARGIN_Y, screenWidth() - MARGIN_X + 1, screenHeight() - MARGIN_Y, WHITE); //bottom line
}



void initializeBoard() 
{
  for (int column = 0; column < BOARD_WIDTH; column++) 
  {
    for (int row = 0; row < BOARD_HEIGHT; row++) 
	{
      board[column][row] = POSITION_FREE;
    }
  }
}

int detectCollisions(int move) 
{
  for (int row = 0; row < currentMovingPiece.height; row++)
  {
    for (int column = 0 ; column < currentMovingPiece.width ; column++) 
	{
      if (currentMovingPiece.array[column][row]) 
	  {
		switch(move) 
		{
			case SDLK_LEFT:
				if(!isMoveLegal(move) || board[column + currentMovingPiece.column - 1][row + currentMovingPiece.row])
				{
					return FALSE;
				}
				break;
			case SDLK_RIGHT:
				if(!isMoveLegal(move) || board[column + currentMovingPiece.column + 1][row + currentMovingPiece.row])
				{
					return FALSE;
				}
				break;
			case SDLK_DOWN:
				if(!isMoveLegal(move) || board[column + currentMovingPiece.column][row + currentMovingPiece.row + 1]) 
				{
					addPieceToTheBoard();
					return FALSE;
				}
				break;
			case SDLK_UP:
			if(!isMoveLegal(move) || board[column + currentMovingPiece.column][row + currentMovingPiece.row - 1]) 
			{
				return FALSE;
			}
			break;
		}
      }
    }
  }
  return TRUE;
}

void rotatePiece() 
{
  int horizontalShiftVector, verticalShiftVector;
  MovingPiece temp = currentMovingPiece;
  temp.orientation = (temp.orientation + 1) % NUMBER_OF_ORIENTATIONS;
  for(int row = 0; row < BLOCK_DIMENSION; row++) 
  {
    for(int column = 0; column < BLOCK_DIMENSION; column++) 
	{
      temp.array[column][row] = pieces[temp.type][temp.orientation][column][row];
    }
  }
  
  for(int row = 0; row < BLOCK_DIMENSION; row++) 
  {
    for(int column = 0; column < BLOCK_DIMENSION; column++) 
	{
      if (temp.array[column][row] == PIVOT_POINT) 
	  {
		temp.pivotRow = row;
		temp.pivotColumn = column;
      }
    }
  }
  
  temp.width = getMovingPieceWidth(temp);
  temp.height = getMovingPieceHeight(temp);
  
  horizontalShiftVector = temp.pivotColumn - currentMovingPiece.pivotColumn;
  verticalShiftVector = temp.pivotRow - currentMovingPiece.pivotRow;
  
  temp.row -= verticalShiftVector;
  temp.column -= horizontalShiftVector;
  
  if (temp.row < 0 || temp.row + temp.height > BOARD_HEIGHT || temp.column < 0 || temp.column + temp.width > BOARD_WIDTH) 
  {
    return;
  }
  
  for (int row = 0; row < BLOCK_DIMENSION; row++) 
  {
    for (int column = 0; column < BLOCK_DIMENSION; column++) 
	{
      if (temp.array[column][row] && board[temp.column + column][temp.row + row]) 
	  {
		return;
      }
    }
  }
  currentMovingPiece = temp;
}

int isMoveLegal(int move) 
{
  switch (move) 
  {
    case SDLK_LEFT:
      if (currentMovingPiece.column > 0) 
	  {
		  return TRUE;
	  }
      break;
    case SDLK_RIGHT:
      if (currentMovingPiece.column < BOARD_WIDTH - currentMovingPiece.width)
	  {
		  return TRUE;
	  }
      break;
    case SDLK_UP:
      if (currentMovingPiece.row > 0) 
	  {
		  return TRUE;
	  }
      break;
    case SDLK_DOWN:
      if (currentMovingPiece.row < BOARD_HEIGHT - currentMovingPiece.height)
	  {
		  return TRUE;
	  }
      break;
  }
  return FALSE;
}
