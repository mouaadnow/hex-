#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "raylib.h"
#include <stddef.h>

#define BOARD_SIZE 9
#define SCREEN_CENTER CLITERAL(Vector2){width / 2, height / 2}
#define SCREEN_AREA CLITERAL(Rectangle){0, 0, width, height}
bool _quitGame = false;


// Window globals
int width = 800; 
int height = 450;
char title[] = " Raylib Hex++ ";

// Game globals
enum State { kEmpty, kBlue, kRed };
typedef struct Hexagon {
  int x;
  int y;
  enum State state;
} Hexagon;

typedef enum GameState
{
	GAME_MAIN_MENU = 0,
  SELECT_THE_GAME_MODE,
  SELECT_TYPE_DIFICULTY,
	GAME_PLAYING,
  GAME_PLAYING_CPU,
	GAME_OVER
} GameState;

static GameState _state;

Hexagon board[BOARD_SIZE][BOARD_SIZE];
float radius = 0;
enum State player = kEmpty;
bool win = false;

void UpdateDrawFrame();
void DrawHexagon(int i, int j);
void InitBoard();
void DrawBoard();
void DrawBorder();
void DrawTextGroup();
void RestartGame();
bool CheckWinState(int i, int j);
bool DidPlayerWin();
void DrawGame(void);
void Drawbegin();

void Drawbegin(){
    const int fontSize = 70;
		const char* gameover = "HEX GAME";
		float measure = MeasureText(gameover, fontSize);
		DrawText("HEX GAME", SCREEN_CENTER.x - measure/2,20, fontSize, BLACK);
}

static void SetState(GameState state)
{
	switch (state)
	{
	case GAME_MAIN_MENU:
		break;

  case SELECT_THE_GAME_MODE:
    break; 

  case SELECT_TYPE_DIFICULTY:
    break;

	case GAME_PLAYING:
   RestartGame();
		break;
		
  case GAME_PLAYING_CPU: 
  	break;  

	case GAME_OVER:
		break;
	
	default:
		break;
	}

	_state = state;
}

void DrawGame(void)
{

const int buttonWidth = 200;
const int buttonHeight = 80;
const Rectangle topButton = {SCREEN_CENTER.x - buttonWidth / 2, SCREEN_CENTER.y - buttonHeight * 1.5 , buttonWidth, buttonHeight};
const Rectangle middleButton = {SCREEN_CENTER.x - buttonWidth / 2, SCREEN_CENTER.y - buttonHeight / 2, buttonWidth, buttonHeight};
const Rectangle bottomButton = {SCREEN_CENTER.x - buttonWidth / 2, SCREEN_CENTER.y + buttonHeight / 2 , buttonWidth, buttonHeight};


	switch (_state)
	{ 
   
	case GAME_MAIN_MENU:
    Drawbegin();
		if (GuiButton(topButton, "PLAY NOW"))
		{
			SetState(SELECT_THE_GAME_MODE);
			return;
		}

		if (GuiButton(middleButton, "QUIT"))
		{
			_quitGame = true;
			return;
		}
		break;
		
	case SELECT_THE_GAME_MODE:
    Drawbegin();
    if (GuiButton(topButton, "PLAYER VS PLAYER"))
		{
			SetState(GAME_PLAYING);
			return;
		}

		if (GuiButton(middleButton, "PLAYER VS CPU "))
		{
			SetState(SELECT_TYPE_DIFICULTY);
			return;
		}
    if (GuiButton(bottomButton, "RETURN"))
		{
			SetState(_state-1);
			return;
		}
    break;

  case SELECT_TYPE_DIFICULTY:
    Drawbegin();
    if (GuiButton(topButton, "EASY"))
		{
			SetState(GAME_PLAYING);
			return;
		}

		if (GuiButton(middleButton, "DIFICIL"))
		{
			SetState(GAME_PLAYING);
			return;
		}
    if (GuiButton(bottomButton, "RETURN"))
		{
			SetState(_state-1);
			return;
		}
      break;

  case GAME_PLAYING:
    DrawTextGroup();
    DrawBorder();
    DrawBoard();
    break;

  case GAME_PLAYING_CPU:
    DrawTextGroup();
    DrawBorder();
    DrawBoard();
    break;

	case GAME_OVER:
	
		//DrawRectangleRec(SCREEN_AREA, Fade(RED, 0.5f));

		/*const int fontSize = 64;
		const char* gameover = "GAME OVER!";
		float measure = MeasureText(gameover, fontSize);
		DrawText(gameover, SCREEN_CENTER.x - measure/2, fontSize * 1.5f, fontSize, WHITE);*/
		const int fontSize = 70;
		const char* gameover = "GAME OVER !";
		float measure = MeasureText(gameover, fontSize);
		DrawText(gameover, SCREEN_CENTER.x - measure/2,20, fontSize, BLACK);

		if (GuiButton(topButton, "PLAY AGAIN?")) //GuiButton(topButton, "PLAY AGAIN?")
		{
	    SetState(GAME_PLAYING);

			return;
		}

		if (GuiButton(middleButton, "QUIT")) //GuiButton(bottomButton, "QUIT")
		{
			_quitGame = true;
			return;
		}
		break;
	
	default:
		TraceLog(LOG_ERROR, "Unhandled GameState!");
		break;
	}
} 

void GameOver(void)
{
	SetState(GAME_OVER);
}
void Game() {
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(width, height, title);

  // Radius to fit double the amount of board size
  radius = ((float)width / (BOARD_SIZE * 2))/ 2 ; // le  rayon de hexagone dans une grille hexagonale de maniere a ce qu'ils puissent etre places

  InitBoard();
  GuiLoadStyle("D:\\hex_game_c\\resources\\ashes.rgs");
  while (!WindowShouldClose() && !_quitGame) {
    UpdateDrawFrame();
    //DrawText("Hex game", SCREEN_CENTER.x - 80 , 20 , 80, BLACK); //width - 200
  }

  CloseWindow();
}

void UpdateDrawFrame() {     /// ** START **
  BeginDrawing();
  ClearBackground(RAYWHITE);
  DrawGame();
  EndDrawing();
}

void DrawTextGroup() {
  if (player == kRed) {
    DrawText(win ? "Red Won." : "Red's turn.", width - 200, 80, 20, RED);
  } else if (player == kBlue) {
    DrawText(win ? "Blue Won." : "Blue's turn.", width - 200, 80, 20, BLUE);
  } else {
    // possible d'ajoute le choix d'elle debut 
    // Random turn at start of new game
    player = GetRandomValue(0, 1) == 0 ? kBlue : kRed;
  }
  DrawText("Hex", width - 200, 20, 50,BLACK);
  //DrawText("Press R to restart game.", 20, height - 25, 15, DARKGRAY);
  if (win) {
     GameOver();
    //DrawText("Game Over", width - 200, 110, 30, player == kBlue ? BLUE : RED);
  }
}

void DrawBorder() {
  int n = BOARD_SIZE - 1;
  int extra = 15;
  Vector2 top_left = {board[0][0].x - radius - extra, board[0][0].y - radius};
  Vector2 bottom_left = {board[n][0].x - radius + extra - 8,
                         board[n][0].y + radius};
  Vector2 top_right = {board[0][n].x + radius - extra + 8,
                       board[0][n].y - radius};
  Vector2 bottom_right = {board[n][n].x + radius + extra,
                          board[n][n].y + radius};
  int thick = 3;
  DrawLineEx(top_left, bottom_left, thick, BLUE);
  DrawLineEx(top_right, bottom_right, thick, BLUE);
  DrawLineEx(top_left, top_right, thick, RED);
  DrawLineEx(bottom_left, bottom_right, thick, RED);
}

void RestartGame() {

    InitBoard();
    player = kEmpty;
    win = false;
  
}

void DrawHexagon(int i, int j) {
  Hexagon *hexagon = &board[i][j];
  int sides = 6;
  int rotation = 30;   //  i changed to 30 
  
  Vector2 position = {hexagon->x, hexagon->y};
  bool is_empty = hexagon->state == kEmpty;
  if (is_empty && !win) {
    bool is_colliding =
        CheckCollisionPointCircle(position, GetMousePosition(), radius - 3);
    bool click = IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
    if (is_colliding && click) {
      // when player clicks highlighted hex set state and check for win
      hexagon->state = player;
      if (DidPlayerWin())
        win = true;
      else
        player = player == kBlue ? kRed : kBlue;
    } else if (is_colliding) {
      // highlight current hex
      DrawPoly(position, sides, radius, rotation, DARKGRAY);
    }
  } else if (!is_empty) {
    Color color = hexagon->state == kBlue ? BLUE : RED;
    DrawPoly(position, sides, radius, rotation, color);
  }
  DrawPolyLines(position, sides, radius, rotation, BLACK );
}

void InitBoard() {
  float x_offeset = 70;
  float y_offeset = 70;
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      board[i][j].x = x_offeset + (j * radius * 1.75);
      board[i][j].y = y_offeset + (i * radius * 1.50);
      board[i][j].state = kEmpty;
    }
    x_offeset += radius - 2;
  }
}

void DrawBoard() {
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      DrawHexagon(i, j);
    }
  }
}

bool CheckWinState(int i, int j) {
  if (i < 0 || j < 0 || i >= BOARD_SIZE || j >= BOARD_SIZE ||
      board[i][j].state != player)
    return false;

  if ((player == kBlue && j == BOARD_SIZE - 1) ||
      (player == kRed && i == BOARD_SIZE - 1))
    return true;

  board[i][j].state = kEmpty;
  bool ans = CheckWinState(i + 1, j) || CheckWinState(i, j + 1) ||
             CheckWinState(i - 1, j) || CheckWinState(i, j - 1) ||
             CheckWinState(i + 1, j - 1) || CheckWinState(i - 1, j + 1);
  board[i][j].state = player;

  return ans;
}

bool DidPlayerWin() {
  bool ans = false;
  for (int i = 0; i < BOARD_SIZE; i++) {
    ans |= CheckWinState(player == kBlue ? i : 0, player == kRed ? i : 0);
  }
  return ans;
}



int main() {
  Game();
  return 0;
}
