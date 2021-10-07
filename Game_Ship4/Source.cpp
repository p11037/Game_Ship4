#include <stdio.h>
#include <windows.h>
#include <time.h>
#include<conio.h>
#define scount 80
#define screen_x 80
#define screen_y 25
int X=0, Y=0,color,score=10;
HANDLE wHnd;
HANDLE rHnd;
DWORD fdwMode;
CHAR_INFO consoleBuffer[screen_x * screen_y];
COORD bufferSize = { screen_x,screen_y };
COORD characterPos = { 0,0 };
SMALL_RECT windowSize = { 0,0,screen_x - 1,screen_y - 1 };
COORD star[scount];
int setMode()
{
	rHnd = GetStdHandle(STD_INPUT_HANDLE);
	fdwMode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT |
		ENABLE_MOUSE_INPUT;
	SetConsoleMode(rHnd, fdwMode);
	return 0;
}
void setcursor(bool visible)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}
int setConsole(int x, int y)
{
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	SetConsoleScreenBufferSize(wHnd, bufferSize);
	return 0;
}
void clear_buffer()
{
	for (int y = 0; y < screen_y; ++y) {
		for (int x = 0; x < screen_x; ++x) {
			consoleBuffer[x + screen_x * y].Char.AsciiChar = ' ';
			consoleBuffer[x + screen_x * y].Attributes = 7;
		}
	}
}
void fill_buffer_to_console()
{
	WriteConsoleOutputA(wHnd, consoleBuffer, bufferSize, characterPos,
		&windowSize);
}
void init_star()
{
	for (int i = 0; i < scount; i++) {
		int n = rand() % 80;
		int m = rand() % 25;
		star[i].X = n;
		star[i].Y = m;
	}

}
void star_fall()
{
	int i;
	 
	for (i = 0; i < scount; i++) 
	{
		if (star[i].Y >= screen_y - 1) 
		{
			short R = rand() % screen_x;
			star[i] = { R,1 };
		}
		else if (star[i].X >= X - 2 && star[i].X <= X + 2 && star[i].Y == Y)
		{
			short R = rand() % screen_x;
			star[i] = { R,1 };
			score--;
			Beep(1000, 10);
		}
		else {
			star[i] = { star[i].X,short(star[i].Y+1) };
		}
	}
}

void fill_star_to_buffer()
{
	for (int i = 0; i < scount; i++) {
		consoleBuffer[star[i].X + screen_x * star[i].Y].Char.AsciiChar = '*';
		consoleBuffer[star[i].X + screen_x * star[i].Y].Attributes = 7;
	}
}
void fill_ship_to_buffer()
{
	consoleBuffer[X-2 + screen_x * Y].Char.AsciiChar = '<';
	consoleBuffer[X-2 + screen_x * Y].Attributes = color;
	consoleBuffer[X - 1 + screen_x * Y].Char.AsciiChar = '(';
	consoleBuffer[X - 1 + screen_x * Y].Attributes = color;
	consoleBuffer[X + screen_x * Y].Char.AsciiChar = '-';
	consoleBuffer[X + screen_x * Y].Attributes = color;
	consoleBuffer[X + 1 + screen_x * Y].Char.AsciiChar = ')';
	consoleBuffer[X + 1 + screen_x * Y].Attributes = color;
	consoleBuffer[X + 2 + screen_x * Y].Char.AsciiChar = '>';
	consoleBuffer[X + 2 + screen_x * Y].Attributes = color;
}
int main()
{
	DWORD numEvents = 0;
	DWORD numEventsRead = 0;
	bool player = true;
	color = 7;
	srand(time(NULL));
	setConsole(screen_x, screen_y);
	setcursor(0);
	setMode();
	init_star();
	while (player&&score>0)
	{
		GetNumberOfConsoleInputEvents(rHnd, &numEvents);
		if (numEvents != 0) {
			INPUT_RECORD* eventBuffer = new INPUT_RECORD[numEvents];
			ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);
			for (DWORD i = 0; i < numEventsRead; ++i) 
			{
				if (eventBuffer[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED || eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'C' || eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'c')
				{
					color = rand() % 15 + 1;
				}
				else if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) player = false;
				else 
				{
					X = eventBuffer[i].Event.MouseEvent.dwMousePosition.X;
					Y = eventBuffer[i].Event.MouseEvent.dwMousePosition.Y;
				}

			}
		}
		if (X < 2)
			X = 2;
		if (X > 77)
			X = 77;
		
		star_fall();
		clear_buffer();
		fill_star_to_buffer();
		fill_ship_to_buffer();
		fill_buffer_to_console();
		Sleep(100);
	}
	return 0;
}