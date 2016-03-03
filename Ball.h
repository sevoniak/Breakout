#pragma once
#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>
#include <cstdlib>

#define BALL_WIDTH 15
#define BALL_HEIGHT 15

class Ball
{
	int x, y;
	int xsign, ysign;
	int speed;
	int direction;

public:
	Ball(void);
	Ball(int, int, int);
	~Ball(void);

	void Movex(int);
	void Movey(int);
	void Setxsign(int);
	void Setysign(int);
	void SetSpeed(int);
	void SetDir(int);
	int Getx();
	int Gety();
	int Getxsign();
	int Getysign();
	int GetSpeed();
	int GetDir();

	void PrintBall(HDC, HDC[]);
	void Reset();
};
