#include "Ball.h"

Ball::Ball(void):
x(0),
y(0),
xsign(0),
ysign(0),
speed(0)
{
}

Ball::Ball(int xin, int yin, int speedIN)
{
	x = xin;
	y = yin;
	speed = speedIN;
	xsign = 1;
	ysign = 1;
	srand(GetTickCount());
	direction = (rand() % 5) + 4;
}

Ball::~Ball(void)
{
}

void Ball::Movex(int xin) { x += xsign * xin; }
void Ball::Movey(int yin) { y += ysign * yin; }
void Ball::Setxsign(int xsignIN) { xsign = xsignIN; }
void Ball::Setysign(int ysignIN) { ysign = ysignIN; }
void Ball::SetSpeed(int speedIN) { speed = speedIN; }
void Ball::SetDir(int dirIN) { direction = dirIN; }

int Ball::Getx() { return x; }
int Ball::Gety() { return y; }
int Ball::Getxsign() { return xsign; }
int Ball::Getysign() { return ysign; }
int Ball::GetSpeed() { return speed; }
int Ball::GetDir() { return direction; }

void Ball::PrintBall(HDC hdc, HDC graphics[])
{
	BitBlt(hdc, x, y, BALL_WIDTH, BALL_HEIGHT, graphics[0], 0, 0, SRCCOPY);
}

void Ball::Reset()
{
	y = 210;
	x = (rand() % 500) + 250;
	direction = (rand() % 5) + 4;
}