#ifndef _GAME
#define _GAME

#include <cstdio>
#include <cstdlib>
#include "..\Field\Field.hpp"
#include "..\Snake\Snake.hpp"
#include "..\Stuff.hpp"

class Game
{
private:
	int points;
	void snakeTeleport(int x, int y);

public:
	Snake snake;
	Field field;

	int UserControl(char w, char s, char a, char d);
	int getPoints();
	void resetPoints();

	Game();
	~Game();
};

#endif // !_GAME