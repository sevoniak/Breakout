#include "Board.h"
#include "resource.h"

#define GAMESTATE_INIT 0
#define GAMESTATE_TITLE 1
#define GAMESTATE_RUN 2
#define GAMESTATE_EVENT 3
#define GAMESTATE_GAMEOVER 4
#define GAMESTATE_QUIT 5
#define GAME_SPEED 20

int cxClient, cyClient;
int gamestate = 0;
HDC mainDC = NULL;
HDC backDC = NULL;
HBITMAP backbit = NULL;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void GameMain(HWND hwnd, HINSTANCE hInstance);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR lpCmdLine, int nCmdShow)
{
	const wchar_t CLASS_NAME[] = L"breakout";
	WNDCLASSEX wc = {};
	wc.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hIconSm = (HICON) LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = CLASS_NAME;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbSize = sizeof(wc);
	RegisterClassEx(&wc);

	HWND hwnd = CreateWindow(CLASS_NAME, L"My Breakout", WS_OVERLAPPEDWINDOW &~WS_THICKFRAME &~WS_MAXIMIZEBOX, 50, 10, 800, 600,
							 NULL, NULL, hInstance, NULL);
	if (!hwnd)
		return 0;

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	MSG msg;

	bool running = true;
	while (running)
	{
		DWORD start_tick = GetTickCount();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				running = false;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		GameMain(hwnd, hInstance);
		BitBlt(mainDC, 0, 0, 800, 600, backDC, 0, 0, SRCCOPY);
		while (GetTickCount() - start_tick < GAME_SPEED);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdcWin;
	PAINTSTRUCT ps;

	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CREATE:
		return 0;

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_PAINT:
		hdcWin = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void GameMain(HWND hwnd, HINSTANCE hInstance)
{
	static int score, blocks, lives, countdown, eventtype, eventwidth, eventheight, eventindex;
	static HDC graphics[13];
	static Board* pBoard;
	wstring sc = L"Score: ";
	wstring lv = L"Lives: " ;
	wstring bl = L"Blocks left: ";
	wstringstream out1, out2;

	switch (gamestate)
	{
	case GAMESTATE_INIT:
		{
			HBITMAP hbmRedBlock = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
			HBITMAP hbmOrangeBlock = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
			HBITMAP hbmYellowBlock = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP3));
			HBITMAP hbmGreenBlock = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP4));
			HBITMAP hbmBlueBlock = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP5));
			HBITMAP hbmBall = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP6));
			HBITMAP hbmPaddleL = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP7));
			HBITMAP hbmPaddleM = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP8));
			HBITMAP hbmPaddleS = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP9));
			HBITMAP hbmSpeedUp = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP10));
			HBITMAP hbmSizeDown = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP11));
			HBITMAP hbmLifeLost = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP12));
			HBITMAP hbmGameOver = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP13));

			mainDC = GetDC(hwnd);
			backDC = CreateCompatibleDC(mainDC);
			backbit = CreateCompatibleBitmap(mainDC, 800, 600);
			SelectObject(backDC, backbit);
			SetBkColor(backDC, RGB(0,0,0));
			SetTextColor(backDC, RGB(255,255,255));

			for (int i = 0; i < 13; i++)
				graphics[i] = CreateCompatibleDC(mainDC);
			
			SelectObject(graphics[0], hbmBall);
			SelectObject(graphics[1], hbmRedBlock);
			SelectObject(graphics[2], hbmOrangeBlock);
			SelectObject(graphics[3], hbmYellowBlock);
			SelectObject(graphics[4], hbmGreenBlock);
			SelectObject(graphics[5], hbmBlueBlock);
			SelectObject(graphics[6], hbmPaddleL);
			SelectObject(graphics[7], hbmPaddleM);
			SelectObject(graphics[8], hbmPaddleS);
			SelectObject(graphics[9], hbmSpeedUp);
			SelectObject(graphics[10], hbmSizeDown);
			SelectObject(graphics[11], hbmLifeLost);
			SelectObject(graphics[12], hbmGameOver);

			pBoard = new Board();
			pBoard->SetupBoard();
			blocks = 50;
			lives = 3;
			score = 0;
			eventtype = -1;
			gamestate++;
		}
		break;

	case GAMESTATE_TITLE:
		gamestate++;

	case GAMESTATE_RUN:
		{
			if (GetAsyncKeyState(VK_RIGHT))
				pBoard->MoveRight(cxClient);
			if (GetAsyncKeyState(VK_LEFT))
				pBoard->MoveLeft();

			pBoard->MoveBall();

			if (pBoard->CheckBall(cxClient, score, lives, eventtype))
			{
				blocks--;
				pBoard->LineCheck(eventtype);
			}

			RECT rc;
			GetClientRect(hwnd, &rc);
			FillRect(backDC, &rc, (HBRUSH) GetStockObject(BLACK_BRUSH));
			pBoard->PrintBoard(backDC, graphics);
			out1 << score;
			out2 << blocks;
			sc += out1.str();
			bl += out2.str();
			TextOut(backDC, 2, 2, sc.c_str(), sc.length());
			TextOut(backDC, 2, 22, bl.c_str(), bl.length());
			TextOut(backDC, 2, 42, lv.c_str(), lv.length());
			for (int i = 0; i < lives; i++)
				BitBlt(backDC, i * 70 + 50, 44, 50, 15, graphics[8],0, 0, SRCCOPY);

			if (blocks < 1 || lives < 1)
			{
				eventtype = EVENT_GAMEOVER;
				gamestate++;
				break;
			}
			if (eventtype > -1)
				gamestate++;

			if (countdown > 0)
			{
				BitBlt(backDC, (cxClient - eventwidth) / 2, 250, eventwidth, eventheight, graphics[eventindex], 0, 0, SRCCOPY);
				countdown--;
			}
			break;
		}

	case GAMESTATE_EVENT:
		countdown = 100;
		switch (eventtype)
		{
		case EVENT_SPEED:
			eventindex = 9;
			eventwidth = 125;
			eventheight = 30;
			eventtype = -1;
			gamestate = GAMESTATE_RUN;
			break;
		case EVENT_SIZE:
			eventindex = 10;
			eventwidth = 130;
			eventheight = 30;
			eventtype = -1;
			gamestate = GAMESTATE_RUN;
			break;
		case EVENT_LIFELOST: 
			eventindex = 11;
			eventwidth = 110;
			eventheight = 30;
			eventtype = -1;
			gamestate = GAMESTATE_RUN;
			break;
		case EVENT_GAMEOVER:
			eventindex = 12;
			eventwidth = 287;
			eventheight = 153;
			eventtype = -1;
			gamestate++;
			break;
		}
		break;

	case GAMESTATE_GAMEOVER:
		if (countdown > 0)
			{
				BitBlt(backDC, (cxClient - eventwidth) / 2, 250, eventwidth, eventheight, graphics[eventindex], 0, 0, SRCCOPY);
				countdown--;
			}
		else
			gamestate++;
		break;

	case GAMESTATE_QUIT:
		for (int i = 0; i < 13; i++)
			DeleteDC(graphics[i]);
		delete pBoard;
		pBoard = 0;
		DeleteDC(backDC);
		DeleteObject(backbit);
		ReleaseDC(hwnd, mainDC);
		PostQuitMessage(0);
		break;

	default: break;
	}
	return;
}