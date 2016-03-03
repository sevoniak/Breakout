#include "Bar.h"

Bar::Bar(void):
color(-1),
points(0),
isVis(false)
{
}

Bar::Bar(int colorIN, int pointsIN):
color(colorIN),
points(pointsIN),
isVis(true)
{
}

Bar::~Bar(void)
{
}

void Bar::SetColor(int colorIN) { color = colorIN; }
void Bar::SetVis(bool visIN) { isVis = visIN; }
int Bar::GetColor() { return color; }
bool Bar::GetVis() { return isVis; }
int Bar::GetPoints() { return points; }
