#pragma once
#ifndef UNICODE
#define UNICODE
#endif
#include "Ball.h"
#include "Paddle.h"
#include "Bar.h"
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>
#include <windows.h>
using std::wstring;
using std::wstringstream;
using std::wifstream;

#define BLOCK_RED 1
#define BLOCK_ORANGE 2
#define BLOCK_YELLOW 3
#define BLOCK_GREEN 4
#define BLOCK_BLUE 5
#define BLOCK_WIDTH 75
#define BLOCK_HEIGHT 20
#define PRINT_OFFSET 20
#define MAX_POINTS 1200

#define EVENT_SPEED 0
#define EVENT_SIZE 1
#define EVENT_LIFELOST 2
#define EVENT_GAMEOVER 3

class Board
{
	bool brokeRed;
	bool brokeYellow;
	int colGone;
	Bar grid[10][5];
	Ball ball;
	Paddle paddle;

public:
	Board(void);
	~Board(void);

	void PrintBoard(HDC, HDC[]);
	void SetupBoard();
	int StringToInt(wstring);

	void MoveRight(int);
	void MoveLeft();
	void MoveBall();
	bool CheckBall(int, int&, int&, int&);
	void BounceBall(bool);
	void BounceOffBar();
	void LineCheck(int&);
};
