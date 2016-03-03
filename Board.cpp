#include "Board.h"

Board::Board(void):
brokeRed(false),
brokeYellow(false),
colGone(0)
{
	ball = Ball(250, 210, 1);
	paddle = Paddle(350, 500, 1);
}

Board::~Board(void)
{
}

void Board::PrintBoard(HDC hdc, HDC graphics[])
{	
	for (int j = 0; j < 5; j++)
	{
		for (int i = 0; i < 10; i ++)
		{
			if (grid[i][j].GetVis())
				BitBlt(hdc, i * BLOCK_WIDTH + PRINT_OFFSET, j * BLOCK_HEIGHT + 4 * PRINT_OFFSET, BLOCK_WIDTH, BLOCK_HEIGHT,
					   graphics[grid[i][j].GetColor()], 0, 0, SRCCOPY);
		}
	}
	
	ball.PrintBall(hdc, graphics);
	paddle.PrintPaddle(hdc, graphics);
}

void Board::SetupBoard()
{
	const wstring filename = L"Level.txt";
	wstring output;
	wstring temp;
	int j = 0;
	size_t end = 0;

	wifstream fin (filename.c_str());

	while(fin.good())
	{
		getline(fin,output);

		for (int i = 0; i < 10; i++)
		{
			end = output.find(';');
			temp = output.substr(0,end);
			output.erase(0,end+1);
			int type = StringToInt(temp);
			grid[i][j] = Bar(type, (MAX_POINTS / type));
		}
		j++;
	}
	fin.close();
}

int Board::StringToInt(wstring stringIN)
{
	int intValue = 0;
	double base = 10;
	for (unsigned int i = 1; i <= stringIN.length(); i++)
	{
		switch (stringIN.c_str()[stringIN.length() - i])
		{
		case '0': break; 
		case '1': intValue += (int)pow(base, (int)(i-1)); break;
		case '2': intValue += 2 * (int)pow(base, (int)(i-1)); break;
		case '3': intValue += 3 * (int)pow(base, (int)(i-1)); break;
		case '4': intValue += 4 * (int)pow(base, (int)(i-1)); break;
		case '5': intValue += 5 * (int)pow(base, (int)(i-1)); break;
		case '6': intValue += 6 * (int)pow(base, (int)(i-1)); break;
		case '7': intValue += 7 * (int)pow(base, (int)(i-1)); break;
		case '8': intValue += 8 * (int)pow(base, (int)(i-1)); break;
		case '9': intValue += 9 * (int)pow(base, (int)(i-1)); break;
		default: return -1;
		}
	}
	return intValue;
}

void Board::MoveRight(int cxClient)
{
	int x = paddle.Getx();
	switch (paddle.GetSize())
	{
	case 1:
		if (x > (cxClient - PADDLE_WIDTH))
		{	
			paddle.Movex(-1);
			return;
		}
		else
		{
			paddle.Movex(10);
			return;
		}

	case 2:
		if (x > (cxClient - PADDLE_WIDTH * 3 / 4))
		{	
			paddle.Movex(-1);
			return;
		}
		else
		{
			paddle.Movex(10);
			return;
		}

	case 3:
		if (x > (cxClient - PADDLE_WIDTH / 2))
		{	
			paddle.Movex(-1);
			return;
		}
		else
		{
			paddle.Movex(10);
			return;
		}
	}
}

void Board::MoveLeft()
{
	int x = paddle.Getx();
	if (x < 1)
		paddle.Movex(1);
	else
		paddle.Movex(-10);
	return;
}

void Board::MoveBall()
{
	int x = 0;
	int y = 0;
	int mult = 1 + ball.GetSpeed();

	switch (ball.GetDir())
	{
	case 0:
		y = -2;
		break;
	case 1:
		x = 1;
		y = -2;
		break;
	case 2:
		x = 2;
		y = -1;
		break;
	case 3:
		x = 2;
		break;
	case 4:
		x = 2;
		y = 1;
		break;
	case 5:
		x = 1;
		y = 2;
		break;
	case 6:
		y = 2;
		break;
	case 7:
		x = -1;
		y = 2;
		break;
	case 8:
		x = -2;
		y = 1;
		break;
	case 9:
		x = -2;
		break;
	case 10:
		x = -2;
		y = -1;
		break;
	case 11:
		x = -1;
		y = -2;
		break;
	}

	x *= mult;
	y *= mult;

	ball.Movex(x);
	ball.Movey(y);
	return;
}

bool Board::CheckBall(int cxClient, int& score, int& lives, int& eventtype)
{
	int x = ball.Getx();
	int y = ball.Gety();

	if (x < 1)
	{
		BounceBall(true);
		return false;
	}
	if (x > cxClient - BALL_WIDTH)
	{
		BounceBall(true);
		return false;
	}
	if (y < 1)
	{
		BounceBall(false);
		return false;
	}
	if (y > paddle.Gety() - BALL_WIDTH && y < paddle.Gety() + PADDLE_HEIGHT)
	{
		if (x > paddle.Getx() && x < paddle.GetRight())
		{
			BounceBall(false);
		}
		return false;
	}
	if (y > 750)
	{
		lives--;
		//eventtype = EVENT_LIFELOST;
		if (lives > 0)
			ball.Reset();
		return false;
	}
			
	x += BALL_WIDTH / 2;
	y += BALL_WIDTH / 2;

	if (y > (4 * PRINT_OFFSET) && y < (5 * BLOCK_HEIGHT + 4 * PRINT_OFFSET))
	{
		if (x > PRINT_OFFSET && x < (10 * BLOCK_WIDTH + PRINT_OFFSET))
		{
			int col = (x - PRINT_OFFSET) / BLOCK_WIDTH;
			int row = (y - 4 * PRINT_OFFSET) / BLOCK_HEIGHT;
			if (grid[col][row].GetVis())
			{
				grid[col][row].SetVis(false);
				score += grid[col][row].GetPoints();
				BounceOffBar();
				return true;
			}
		}
	}
	return false;
}

void Board::BounceBall(bool horz)
{
	if (horz)
	{
		switch (ball.GetDir())
		{
		case 1:
			ball.SetDir(11);
			break;
		case 2:
			ball.SetDir(10);
			break;
		case 3:
			ball.SetDir(9);
			break;
		case 4:
			ball.SetDir(8);
			break;
		case 5:
			ball.SetDir(7);
			break;
		case 7:
			ball.SetDir(5);
			break;
		case 8:
			ball.SetDir(4);
			break;
		case 9:
			ball.SetDir(3);
			break;
		case 10:
			ball.SetDir(2);
			break;
		case 11:
			ball.SetDir(1);
			break;
		}
	}
	else
	{
		int xball = ball.Getx();
		int xpad = paddle.Getx();
		int rpad = paddle.GetRight();
		int sector = 1;

		if (xball >= xpad && xball <= rpad && ball.Gety() > 450)
		{
			xball -= xpad;
			sector = 3 * xball / (rpad - xpad);
		}

		switch (ball.GetDir())
		{
		case 0:
			ball.SetDir(6);
			break;
		case 1:
			ball.SetDir(5);
			break;
		case 2:
			ball.SetDir(4);
			break;
		case 4:
			if (sector == 0)
				ball.SetDir(1);
			else
				ball.SetDir(2);
			break;
		case 5:
			if (sector == 0)
				ball.SetDir(0);
			if (sector == 1)
				ball.SetDir(1);
			if (sector == 2)
				ball.SetDir(2);
			break;
		case 6:
			if (sector == 0)
				ball.SetDir(11);
			if (sector == 1)
				ball.SetDir(0);
			if (sector == 2)
				ball.SetDir(1);
			break;
		case 7:
			if (sector == 0)
				ball.SetDir(10);
			if (sector == 1)
				ball.SetDir(11);
			if (sector == 2)
				ball.SetDir(0);
			break;
		case 8:
			if (sector == 2)
				ball.SetDir(11);
			else
				ball.SetDir(10);
			break;
		case 10:
			ball.SetDir(8);
			break;
		case 11:
			ball.SetDir(7);
			break;
		}
	}
}

void Board::BounceOffBar()
{
	BounceBall(false);

}

void Board::LineCheck(int& eventtype)
{
	bool incSpeed = false;
	bool maxSpeed = false;

	if (ball.GetSpeed() == 3)
		maxSpeed = true;

	if (!brokeRed && !maxSpeed)
		for (int i = 0; i < 10; i++)
		{
			if (grid[i][0].GetVis() == false && !incSpeed)
			{
				incSpeed = true;
				brokeRed = true;
				eventtype = EVENT_SPEED;
				ball.SetSpeed(ball.GetSpeed() + 1);
			}
		}

	if (!brokeYellow && !maxSpeed)
		for (int i = 0; i < 10; i++)
		{
			if (!grid[i][2].GetVis() && !incSpeed)
			{
				incSpeed = true;
				brokeYellow = true;
				eventtype = EVENT_SPEED;
				ball.SetSpeed(ball.GetSpeed() + 1);
			}
		}

	if (colGone < 8)
	{
		colGone = 0;
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				if (grid[i][j].GetVis())
					break;
				if (j == 4)
					colGone++;
			}
		}
		switch (colGone)
		{
		case 5:
			if (paddle.GetSize() < 2)
			{
				paddle.SetSize(2);
				eventtype = EVENT_SIZE;
			}
			break;
		case 7:
			if (paddle.GetSize() < 3)
			{
				paddle.SetSize(3);
				eventtype = EVENT_SIZE;
			}
			break;
		default: break;
		}
	}
}