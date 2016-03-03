#pragma once
#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>

#define PADDLE_WIDTH 100
#define PADDLE_HEIGHT 15

class Paddle
{
	int x, y;
	int size;

public:
	Paddle(void);
	Paddle(int, int, int);
	~Paddle(void);

	void Movex(int);
	void Sety(int);
	void SetSize(int);

	int Getx();
	int Gety();
	int GetSize();
	int GetRight();

	void PrintPaddle(HDC, HDC[]);
};
