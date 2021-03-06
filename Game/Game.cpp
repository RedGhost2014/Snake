#include "Game.hpp"
#include "..\Field\Field.hpp"
#include <Windows.h>
#include "..\Stuff.hpp"
#include "..\resource.h"

Game::~Game() {}

Game::Game()
{
	points = 0;
}

int Game::getPoints()
{
	return points;
}

void Game::resetPoints()
{
	points = 0;
}

void Game::snakeTeleport(int x, int y)
{
	snake.x = x;
	snake.y = y;
}

int Game::UserControl(char w, char s, char a, char d)
{
	POINT oldPoint;
	oldPoint.x = snake.x;
	oldPoint.y = snake.y;

	if (GetKeyState(w) < 0 && snake.dir != dirDown)
		snake.dir = dirUp;
	if (GetKeyState(a) < 0 && snake.dir != dirRight)
		snake.dir = dirLeft;
	if (GetKeyState(s) < 0 && snake.dir != dirUp)
		snake.dir = dirDown;
	if (GetKeyState(d) < 0 && snake.dir != dirLeft)
		snake.dir = dirRight;

	if (snake.dir == dirLeft)
		snake.x--;
	if (snake.dir == dirRight)
		snake.x++;
	if (snake.dir == dirUp)
		snake.y--;
	if (snake.dir == dirDown)
		snake.y++;

	if (field.getCharField()[snake.y][snake.x] == REDPORTAL)
	{
		PlaySound(MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_RESOURCE | SND_ASYNC);
		int place = 0;
		for (int i = 0; i < PORTALCOUNT; i++)
		{
			if (snake.x == field.getPortals()[i].portal.x && snake.y == field.getPortals()[i].portal.y && field.getPortals()[i].type == REDPORTAL)
			{
				place = i;
				break;
			}
		}
		snakeTeleport(field.getPortals()[abs(place - 3)].portal.x, field.getPortals()[abs(place - 3)].portal.y);
	}
	else if (field.getCharField()[snake.y][snake.x] == BLUEPORTAL)
	{
		PlaySound(MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_RESOURCE | SND_ASYNC);
		int place = 0;
		for (int i = 0; i < PORTALCOUNT; i++)
		{
			if (snake.x == field.getPortals()[i].portal.x && snake.y == field.getPortals()[i].portal.y && field.getPortals()[i].type == BLUEPORTAL)
			{
				place = i;
				break;
			}
		}
		snakeTeleport(field.getPortals()[abs(place - 3)].portal.x, field.getPortals()[abs(place - 3)].portal.y);
	}

	if (field.getCharField()[snake.y][snake.x] == FRUIT1 || field.getCharField()[snake.y][snake.x] == FRUIT2 || field.getCharField()[snake.y][snake.x] == FRUIT3)
	{
		PlaySound(MAKEINTRESOURCE(IDR_WAVE2 + (rand() % 4)), NULL, SND_RESOURCE | SND_ASYNC);
		this->points++;
		snake.addTailSegment(oldPoint.x, oldPoint.y);
		while (1)
		{
			char** ptrField = field.getCharField();
			short x = rand() % COLUMNS;
			short y = rand() % ROWS;
			if (x != 0 && x != (field.getColumns() - 1) && y != 0 && y != (field.getRows() - 1) &&
				ptrField[y][x] != '#' && ptrField[y][x] != TAIL && ptrField[y][x] != 'V' &&
				ptrField[y][x] != 'A' && ptrField[y][x] != '<' &&
				ptrField[y][x] != '>' && ptrField[y][x] != REDPORTAL &&
				ptrField[y][x] != BLUEPORTAL && ptrField[y][x] != FRUIT1 &&
				ptrField[y][x] != FRUIT2 && ptrField[y][x] != FRUIT3)
			{
				field.addFruit(x, y);
				break;
			}
		}
		
	}
	else snake.MoveTail(oldPoint.x, oldPoint.y);

	if ((snake.x <= 0) || (snake.x >= field.getColumns() - 1) ||
		(snake.y <= 0) || (snake.y >= field.getRows() - 1) ||
		field.getCharField()[snake.y][snake.x] == TAIL ||
		field.getCharField()[snake.y][snake.x] == '#')
	{
		PlaySound(MAKEINTRESOURCE(IDR_WAVE6), NULL, SND_RESOURCE | SND_ASYNC);
		return 1;
	}
	return 0;
}