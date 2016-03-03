#pragma once

class Bar
{
	int color;
	int points;
	bool isVis;

public:
	Bar(void);
	Bar(int, int);
	~Bar(void);

	void SetColor(int);
	void SetVis(bool);
	int GetColor();
	bool GetVis();
	int GetPoints();
};
