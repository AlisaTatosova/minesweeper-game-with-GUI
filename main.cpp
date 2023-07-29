
#include <iostream>
#include <time.h>
#include "raylib.h"
#include "raymath.h"

const int ROWS = 10;
const int COLS = 10;

const int screen_width = 640;  
const int screen_height = 640; 

const int square_width = screen_width / COLS;
const int square_height = screen_height / ROWS;

struct Square {
  int val_inside; // what is inside 
  int i;  //index i
  int j;  //index j
  bool mine;
  bool is_opened;
  bool is_flagged;

};

Square matrix[COLS][ROWS];
Texture2D square; 
Texture2D flag;
int opened;

enum State {
  PLAYING,
  LOSE,
  WIN
};

State state;

int start;
int end;

void game();
void initialize();
void count_value_inside();
void open_square(int, int);
void to_flag(int, int);
void open_all_squares();
void draw_square(Square);

int c = 0;
void count_value_inside() {
   for (int i = 0; i < COLS; i++) {
        for (int j = 0; j < ROWS; j++) {
            if(matrix[i][j].mine ) {
              continue;
            }
            if(matrix[i + 1][j + 1].mine && (i + 1) < 10 && (j + 1) < 10) {
              matrix[i][j].val_inside++;
            }
            if (matrix[i][j - 1].mine && (j - 1) >= 0) {
             matrix[i][j].val_inside++;
            }    
            if (matrix[i + 1][j].mine && (i + 1) < 10) {
              matrix[i][j].val_inside++;
            }
            if (matrix[i + 1][j - 1].mine && (i + 1) < 10 && (j - 1) >= 0) {
              matrix[i][j].val_inside++;
            }
            if(matrix[i - 1][j + 1].mine && (i - 1) >= 0 && (j + 1) < 10 ) {
              matrix[i][j].val_inside++;
            }
            if (matrix[i][j + 1].mine  && (j + 1) < 10 ) {
              matrix[i][j].val_inside++;
            }
            if(matrix[i - 1][j].mine &&  (i - 1) >= 0) {
              matrix[i][j].val_inside++;
            }
            if (matrix[i - 1][j - 1].mine && (i - 1)  >= 0 && (j - 1) >= 0) {
              matrix[i][j].val_inside++;
            }
        }
    }
}


void initialize() {
  for (int i = 0; i < COLS; i++) {
    for (int j = 0; j < ROWS; j++) {
          matrix[i][j].i = i,
          matrix[i][j].j = j,
          matrix[i][j].mine = false,
          matrix[i][j].is_opened = false,
          matrix[i][j].is_flagged = false,
          matrix[i][j].val_inside = 0;
            if (rand() % 6 == 0) {
                c++;
                matrix[i][j].mine = true; 
            }
            else {
                matrix[i][j].val_inside = 0 ; /*keep 0, then we will count how many mines is
                                              in the surrounding, and will assign tht number*/
            }
        }
    }
}

void game() {
  initialize();
  count_value_inside();
  state = PLAYING;
  opened = 0;
  start = GetTime();
}

void open_square(int i, int j) {
  if (matrix[i][j].is_flagged || matrix[i][j].is_opened) {  // if is opened or flagged do nothing
    return;
  }

  matrix[i][j].is_opened = true; // assigning that is opened

  if (matrix[i][j].mine) {
    state = LOSE;
    end = GetTime();
  }
  
  opened++;

  if (opened == ROWS * COLS - c) {
    state = WIN;
    end = GetTime();
  }
}

void to_flag(int i, int j) {
  if (matrix[i][j].is_opened) {  
    return;
  }

  matrix[i][j].is_flagged = !matrix[i][j].is_flagged; // if is flagged remove
}

void open_all_squares() {
  for (int i = 0; i < COLS; ++i) {
             for (int j = 0; j < ROWS; ++j) {
                if (matrix[i][j].mine) {
                    Rectangle source2 = (Rectangle){2 * square_width, 0, square.width / 16, float(square.height)};
                    DrawTextureRec(square, source2, (Vector2){i * square_width,j * square_height}, WHITE);
                }
                else if (matrix[i][j].val_inside) {
                    Rectangle source3 = (Rectangle){(square_width * matrix[i][j].val_inside) + 3 * square_width, 0, square.width / 16, float(square.height)};
                    DrawTextureRec(square, source3, (Vector2){(float)(i * square_width), (float)(j * square_height)}, WHITE);
                }
                else if (matrix[i][j].val_inside == 0) {
                    Rectangle source4 = (Rectangle){(13 * square_width) - 2, 0, square.width / 16, float(square.height - 1)};
                    DrawTextureRec(square, source4, (Vector2){(float)(i * square_width), (float)(j * square_height)}, WHITE);
                }
                    
            }
                    
        }
}

int glob_i = 0;
int glob_j = 0;

void draw_square(Square sq) {
    Rectangle source1 = (Rectangle){0, 0, float(flag.height), float(flag.height)};
    int ind_i = sq.i * square_width;
    int ind_j = sq.j * square_height;
    DrawTextureRec(square, source1, (Vector2){(float)ind_i, (float)ind_j}, WHITE); // the initial squares are in their place
    if (sq.is_opened) {
    	if (sq.mine) {
        glob_i = sq.i;
        glob_j = sq.j;
        state = LOSE;
      }
      else {
      	Rectangle source6 = (Rectangle){(13 * square_width) - 2, 0, square.width / 16, float(square.height - 1)};
    		DrawTextureRec(square, source6, (Vector2){(float)(sq.i * square_width), (float)(sq.j * square_height)}, WHITE);    
			  if(sq.val_inside > 0) {
			    Rectangle source5 = (Rectangle){(square_width * sq.val_inside) + 3 * square_width, 0, square.width / 16, float(square.height)};
          DrawTextureRec(square, source5, (Vector2){(float)(sq.i * square_width), (float)(sq.j * square_height)}, WHITE);
      	}
      }
	  }
	  else if (sq.is_flagged) {
    //to draw flag
    	Rectangle source = {0, 0, flag.width, flag.height};
    	Rectangle dest = {sq.i * square_width, sq.j * square_height, square_width, square_height};
    	DrawRectangle(sq.i * square_width, sq.j * square_height, square_width, square_height, LIGHTGRAY); // there is no mine in surrounding
		  DrawTexturePro(flag, source, dest, Vector2{0, 0}, 0.0f, Fade(WHITE, 0.5f));
	  }
  } 

int main() {
  
  srand(time(NULL));
  InitWindow(screen_width, screen_height, "Welcome to Minesweeper");
 
  flag = LoadTexture("flag.png");
  square = LoadTexture("minesweeper.png");
  game();
  
  while(!WindowShouldClose()) {
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 pos = GetMousePosition();
        int ind_i = pos.x / square_width;
        int ind_j = pos.y / square_height;
        if(state == PLAYING){
          open_square(ind_i, ind_j);
        }
     }
    else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        Vector2 pos = GetMousePosition();
        int ind_i = pos.x / square_width;
        int ind_j = pos.y / square_height;
        if (state == PLAYING) {
           to_flag(ind_i, ind_j);
        }
    }

    if (IsKeyPressed(KEY_R)) {
        game();
    }

    BeginDrawing();

      ClearBackground(RAYWHITE);

      for (int i = 0; i < COLS; i++) {
        for (int j = 0; j < ROWS; j++) {
          draw_square(matrix[i][j]);
        }
      }

      if (state == LOSE) {
        open_all_squares();
        Rectangle source4 = (Rectangle){3 * square_width, 0, square.width / 16, float(square.height)};
        DrawTextureRec(square, source4, (Vector2){(float)(glob_i * square_width), (float)(glob_j * square_height)}, WHITE);
        DrawRectangle(0, 0, screen_width, screen_height, Fade(BEIGE, 0.8f));
        DrawText("YOU LOSE!", screen_width / 2 - MeasureText("YOU LOSE!", 40) / 2, screen_height / 2 - 10, 40, RED);
        DrawText( "PRESS BUTTON 'R' FOR PLAYING AGAIN ! ! !", screen_width / 2 - MeasureText( "PRESS BUTTON 'R' FOR PLAYING AGAIN!", 20) / 2, screen_height * 0.3f - 10, 20, RED);
        int min = (end - start) / 60;
        int sec = (end - start) % 60;
        DrawText(TextFormat("Time : %d minute, %d second.", min, sec), 20, screen_height - 40, 20, DARKGRAY);
      }

      if (state == WIN) {
        open_all_squares();
        DrawRectangle(0, 0, screen_width,screen_height, Fade(WHITE, 0.8f));
        DrawText("YOU WIN!", screen_width / 2 - MeasureText("YOU WIN!", 40) / 2, screen_height / 2 - 10, 40, DARKGRAY);
        DrawText("PRESS BUTTON 'R' FOR PLAYING AGAIN ! ! !", screen_width / 2 - MeasureText( "PRESS BUTTON 'R' FOR PLAYING AGAIN ! ! !", 20) / 2, screen_height * 0.75f - 10, 20, DARKGRAY);
        int min = (end - start) / 60;
        int sec = (end - start) % 60;
        DrawText(TextFormat("Time : %d minute, %d second.", min, sec), 20, screen_height - 40, 20, DARKGRAY);
      }

    EndDrawing();
  }
  
  CloseWindow();
  
  return 0;
}

