#include "Paddle.h"

Paddle::Paddle(void):
x(0),
y(0),
size(0)
{
}

Paddle::Paddle(int xin, int yin, int sizeIN)
{
	x = xin;
	y = yin;
	size = sizeIN;
}

Paddle::~Paddle(void)
{
}

void Paddle::Movex(int xin) { x += xin; }
void Paddle::Sety(int yin) { y = yin; }
void Paddle::SetSize(int sizeIN) { size = sizeIN; }

int Paddle::Getx() { return x; }
int Paddle::Gety() { return y; }
int Paddle::GetSize() { return size; }
int Paddle::GetRight()
{
	switch (size)
	{
	case 1:
		return (x + PADDLE_WIDTH);
	case 2:
		return (x + 3 * PADDLE_WIDTH / 4);
	case 3:
		return (x + PADDLE_WIDTH / 2);
	}
	return -1;
}

void Paddle::PrintPaddle(HDC hdc, HDC graphics[])
{
	int width = PADDLE_WIDTH;
	int index = 6;

	if (size == 2)
	{
		width = width * 3 / 4;
		index++;
	}
	if (size == 3)
	{
		width /= 2;
		index += 2;
	}

	BitBlt(hdc, x, y, width, PADDLE_HEIGHT, graphics[index], 0, 0, SRCCOPY);
}