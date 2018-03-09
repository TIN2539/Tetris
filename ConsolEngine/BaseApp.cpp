// Copyright 2009-2014 Blam Games, Inc. All Rights Reserved.

#include "TestApp.h"
#include <algorithm>
#include <time.h>
#include <conio.h>
#include <assert.h>
#include <strsafe.h>
#include <iostream>
#include <stdio.h>
#include <thread>

#define MY_PERFORMENCE_COUNTER

#include "PerformanceCounter.h"

BaseApp::BaseApp(int xSize, int ySize) : X_SIZE(xSize), Y_SIZE(ySize)
{
	SMALL_RECT windowSize = { 0, 0, X_SIZE, Y_SIZE };
	COORD windowBufSize = { X_SIZE + 1, Y_SIZE + 1 };

	mConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	mConsoleIn = GetStdHandle(STD_INPUT_HANDLE);

	if (!SetConsoleScreenBufferSize(mConsole, windowBufSize))
	{
		cout << "SetConsoleScreenBufferSize failed with error " << GetLastError() << endl;
	}
	if (!SetConsoleWindowInfo(mConsole, TRUE, &windowSize))
	{
		cout << "SetConsoleWindowInfo failed with error " << GetLastError() << endl;
	}

	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(mConsole, &cursorInfo);
	cursorInfo.bVisible = FALSE;
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(mConsole, &cursorInfo);


	mChiBuffer = (CHAR_INFO*)malloc((X_SIZE + 1)*(Y_SIZE + 1) * sizeof(CHAR_INFO));

	mDwBufferSize.X = X_SIZE + 1;
	mDwBufferSize.Y = Y_SIZE + 1;		// размер буфера данных

	mDwBufferCoord.X = 0;
	mDwBufferCoord.Y = 0;				// координаты €чейки

	mLpWriteRegion.Left = 0;
	mLpWriteRegion.Top = 0;
	mLpWriteRegion.Right = X_SIZE + 1;
	mLpWriteRegion.Bottom = Y_SIZE + 1;	// пр€моугольник дл€ чтени€


	for (int x = 0; x < X_SIZE + 1; x++)
	{
		for (int y = 0; y < Y_SIZE + 1; y++)
		{
			SetChar(x, y, L' ');
		}
	}
}

BaseApp::~BaseApp()
{
	free(mChiBuffer);
}

void BaseApp::SetChar(int x, int y, wchar_t c)
{
	mChiBuffer[x + (X_SIZE + 1)*y].Char.UnicodeChar = c;
	mChiBuffer[x + (X_SIZE + 1)*y].Attributes = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED;

}

wchar_t BaseApp::GetChar(int x, int y)
{
	return mChiBuffer[x + (X_SIZE + 1)*y].Char.AsciiChar;
}

void BaseApp::WriteStr(int x, int y, const char *str, int score)
{
	char nScore[11];
	sprintf(nScore, "%d", score);

	for (int i = 0; i < strlen(str); i++)
	{
		SetChar(x + i, y, str[i]);
	}
	for (int i = 0; i < strlen(nScore); i++)
	{
		SetChar(x + i + strlen(str), y, nScore[i]);
	}
}

void BaseApp::Render()
{
	if (!WriteConsoleOutput(mConsole, mChiBuffer, mDwBufferSize, mDwBufferCoord, &mLpWriteRegion))
	{
		printf("WriteConsoleOutput failed - (%d)\n", GetLastError());
	}
}

void BaseApp::Run()
{
	CStopwatch timer;
	int sum = 0;
	int counter = 0;
	int deltaTime = 0;
	bool GameOver = false;
	int kSpeed = 500;
	int time1 = timer.Time();

	while (1)
	{
		if (kbhit())
		{
			KeyPressed(getch());
			if (!FlushConsoleInputBuffer(mConsoleIn))
				cout << "FlushConsoleInputBuffer failed with error " << GetLastError();
		}

		Render();

		timer.Start();

		while (1)
		{
			deltaTime = timer.Now();
			if (deltaTime > 20)
				break;
		}

		int time2 = timer.Time();

		if (time2 - time1 > kSpeed)
		{
			if (!(GameOver = UpdateF((float)deltaTime / 1000.0f)))
			{
				Render();
				time1 = timer.Time();
				kSpeed -= 1;
			}
		}

		sum += deltaTime;
		counter++;
		if (sum >= 1000)
		{
			TCHAR  szbuff[255];
			StringCchPrintf(szbuff, 255, TEXT("FPS: %d"), counter);
			SetConsoleTitle(szbuff);

			counter = 0;
			sum = 0;
		}

		if (GameOver)
		{
			break;
		}
	}
	system("pause");
}