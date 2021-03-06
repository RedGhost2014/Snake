// Чтобы посмотреть на систему под капотом раскомментируйте DEBUG

//#define DEBUG

#ifdef DEBUG

#pragma comment(linker, "/SUBSYSTEM:CONSOLE")

#endif // DEBUG

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <locale.h>
#include "Game/Game.hpp"
#include "Singleton/Singleton.hpp"
#include "WindowClasses/WindowClass.hpp"
#include <Windows.h>

void gotoxy(int x, int y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}

int _cdecl CountOfDigits(int a)
{
	int count = 1;
	while (a != 0)
	{
		count += 1;
		a = a / 10;
	}
	return count;
}

BOOL ShowConsoleCursor(BOOL bShow)
{
	CONSOLE_CURSOR_INFO cci;
	HANDLE hStdOut;
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE)
		return FALSE;
	if (!GetConsoleCursorInfo(hStdOut, &cci))
		return FALSE;
	cci.bVisible = bShow;
	if (!SetConsoleCursorInfo(hStdOut, &cci))
		return FALSE;
	return TRUE;
}

#ifdef DEBUG

int main()
{
	srand(time(0));
	ShowConsoleCursor(FALSE);

	Singleton::createGame();
	Game* myGame = Singleton::getGame();
	Builder myBuilder(ROWS, COLUMNS, FSTYLE_SQUARE | FSTYLE_HDASH);

	myBuilder.buildbyReference(&Singleton::getGame()->field);

	myGame->snake.ConnectToMap(myGame->field.getCharField());

	myGame->snake.Init(3, 8, dirDown);
	myGame->snake.addTailSegment(3, 4);
	myGame->snake.addTailSegment(3, 5);
	myGame->snake.addTailSegment(3, 6);
	myGame->snake.addTailSegment(3, 7);

	myGame->field.CreatePortals();

	for (short i = 0; i < PORTALCOUNT; i++)
	{
		myGame->field.addPortal(myGame->field.getPortals()[i]);
	}

	myGame->field.addFruit(6, 10);
	myGame->field.addFruit(10, 6);
	myGame->field.addFruit(18, 18);

	myGame->snake.drawOnMap();
	myGame->field.printField();
	while (GetKeyState(VK_ESCAPE) >= 0)
	{
		gotoxy(0, 0);
		if (myGame->UserControl('W', 'S', 'A', 'D'))
		{
			myGame->resetPoints();
			myGame->snake.Init(3, 8, dirDown);
			myGame->snake.addTailSegment(3, 4);
			myGame->snake.addTailSegment(3, 5);
			myGame->snake.addTailSegment(3, 6);
			myGame->snake.addTailSegment(3, 7);
		}
		myGame->field.updateField();

		myGame->snake.drawOnMap();
		myGame->field.printField();
		Sleep(80);
	}

	Singleton::deleteGame();
	system("pause");
	return 0;
}

#else 

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hpi, LPSTR cmdline, int ss)
{
	srand(time(0));
	Singleton::hInst = hInst; // Сохраняем дескриптор модуля для дальнейшей загрузки ресурсов. Подробнее https://docs.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point

	Singleton::createGame();
	Singleton::createMainMenu();
	Singleton::createGameWindow();

	Game* myGame = Singleton::getGame();
	Builder myBuilder(ROWS, COLUMNS, FSTYLE_SQUARE | FSTYLE_HDASH);

	myBuilder.buildbyReference(&Singleton::getGame()->field);

	MainMenu* myMenu = Singleton::getMainMenu();
	GameWindow* myGameWindow = Singleton::getGameWindow();

	myMenu->setSettings(L"MyMainMenu");
	myGameWindow->setSettings(L"MyGameWindow");

	myMenu->StartWindow();

	return 0;
}

#endif // DEBUG
