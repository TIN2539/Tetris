// Copyright 2009-2014 Blam Games, Inc. All Rights Reserved.

#include "TestApp.h"
#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <thread>

static bool checkY = false;

TestApp::TestApp() : Parent(80, 25)
{
	srand(time(0));
	score = 0;
	nFieldWidth = 14;
	nFieldHeight = 20;
	CurrentPiece = rand() % 7;
	CurrentRotation = 0;
	checkY = false;
	CurrentX = nFieldWidth / 2 - 2;
	CurrentY = 0;
	Field = new unsigned char[nFieldHeight*nFieldWidth];
	for (int x = 0; x < nFieldWidth; x++)
	{
		for (int y = 0; y < nFieldHeight; y++)
		{
			if (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1)
			{
				Field[y * nFieldWidth + x] = 2;
			}
			else
			{
				Field[y * nFieldWidth + x] = 0;
			}
		}
	}
	tetromino[0].append(L"..X...X...X...X.");
	tetromino[1].append(L"..X..XX...X.....");
	tetromino[2].append(L".....XX..XX.....");
	tetromino[3].append(L"..X..XX..X......");
	tetromino[4].append(L".X...XX...X.....");
	tetromino[5].append(L".X...X...XX.....");
	tetromino[6].append(L"..X...X..XX.....");
	GameOver = false;
	PaintField();
	PaintTetrom();
}

int TestApp::Rotate(int px, int py, int r)
{
	int pi = 0;
	switch (r % 4)
	{
	case 0: // 0 degrees			// 0  1  2  3
		pi = py * 4 + px;			// 4  5  6  7
		break;						// 8  9 10 11
		//12 13 14 15

	case 1: // 90 degrees			//12  8  4  0
		pi = 12 + py - (px * 4);	//13  9  5  1
		break;						//14 10  6  2
		//15 11  7  3

	case 2: // 180 degrees			//15 14 13 12
		pi = 15 - (py * 4) - px;	//11 10  9  8
		break;						// 7  6  5  4
		// 3  2  1  0

	case 3: // 270 degrees			// 3  7 11 15
		pi = 3 - py + (px * 4);		// 2  6 10 14
		break;						// 1  5  9 13
	}								// 0  4  8 12

	return pi;
}

void TestApp::NewTetromino()
{
	CurrentPiece = rand() % 7;
	CurrentRotation = 0;
	CurrentX = nFieldWidth / 2 - 2;
	CurrentY = 0;
	checkY = false;
	//PaintTetrom();
}

void TestApp::SearchLines()
{
	for (int y = 0; y < 4; y++)
	{
		if (CurrentY + y < nFieldHeight - 1)
		{
			bool line = true;
			for (int x = 1; x < nFieldWidth - 1; x++)
			{
				line &= Field[(CurrentY + y) * nFieldWidth + x] != 0;
			}

			if (line)
			{
				for (int x = 1; x < nFieldWidth - 1; x++)
					Field[(CurrentY + y) * nFieldWidth + x] = 3;
				lines.push_back(CurrentY + y);
			}
		}
	}
	PaintField();
}

void TestApp::BlockTetromino()
{
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
		{
			if (tetromino[CurrentPiece][Rotate(x, y, CurrentRotation)] == 'X')
			{
				Field[(CurrentY + y) * nFieldWidth + (CurrentX + x)] = 1;
			}
		}
}

void TestApp::DeleteReadyLines()
{
	if (!lines.empty())
	{
		for (auto &v : lines)
			for (int x = 1; x < nFieldWidth - 1; x++)
			{
				for (int y = v; y > 0; y--)
				{
					Field[y * nFieldWidth + x] = Field[(y - 1) * nFieldWidth + x];
				}
				Field[x] = 0;
			}
		score += lines.size() * 25;
		lines.clear();
		PaintField();
	}
}

void TestApp::DeleteTetromino()
{
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
		{
			if (tetromino[CurrentPiece][Rotate(x, y, CurrentRotation)] == 'X')
			{
				SetChar(CurrentX + x + 1, CurrentY + y + 2, ' ');
			}
		}
}

void TestApp::KeyPressed(int btnCode)
{
	if (btnCode == 224)
	{
		DeleteTetromino();
		btnCode = getch();
		if (btnCode == 75 && CheckMove(CurrentPiece, CurrentRotation, CurrentX - 1, CurrentY))
		{
			CurrentX--;
		}
		else if (btnCode == 77 && CheckMove(CurrentPiece, CurrentRotation, CurrentX + 1, CurrentY))
		{
			CurrentX++;
		}
		else if (btnCode == 80 && CheckMove(CurrentPiece, CurrentRotation, CurrentX, CurrentY + 1))
		{
			CurrentY++;
		}
		PaintTetrom();
	}
	else if (btnCode == 32 && CheckMove(CurrentPiece, CurrentRotation + 1, CurrentX, CurrentY))
	{
		DeleteTetromino();
		CurrentRotation++;
		PaintTetrom();
	}
}

bool TestApp::UpdateF(float deltaTime)
{
	DeleteReadyLines();
	if (CheckMove(CurrentPiece, CurrentRotation, CurrentX, CurrentY + 1) && checkY == true)
	{
		DeleteTetromino();
		CurrentY++;
		PaintTetrom();
	}
	else if (CheckMove(CurrentPiece, CurrentRotation, CurrentX, CurrentY + 1) && checkY == false)
	{
		DeleteTetromino();
		checkY = true;
		PaintTetrom();

	}
	else
	{
		BlockTetromino();
		SearchLines();
		NewTetromino();

		GameOver = !CheckMove(CurrentPiece, CurrentRotation, CurrentX, CurrentY);
		if (GameOver) { system("cls"); cout << "Game over! Score: " << score << endl; }
	}
	return GameOver;
}

void TestApp::PaintField()
{
	for (int x = 0; x < nFieldWidth; x++)
		for (int y = 0; y < nFieldHeight; y++)
		{
			SetChar(x + 1, y + 2, " o#="[Field[y * nFieldWidth + x]]);
		}
	for (int x = 0; x < nFieldWidth; x++)
	{
		WriteStr(nFieldWidth + 10, 2, "Score: ", score);
	}
}

void TestApp::PaintTetrom()
{
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
		{
			if (tetromino[CurrentPiece][Rotate(x, y, CurrentRotation)] == 'X')
			{
				SetChar(CurrentX + x + 1, CurrentY + y + 2, 2);
			}
		}
}

bool TestApp::CheckMove(int nTetromino, int nRotation, int posX, int posY)
{
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
		{
			int indexTetr = Rotate(x, y, nRotation);

			int indexField = (posY + y) * nFieldWidth + (posX + x);

			if (posX + x >= 0 && posX + x < nFieldWidth)
			{
				if (posY + y >= 0 && posY + y < nFieldHeight)
				{
					if (tetromino[nTetromino][indexTetr] == 'X' && Field[indexField] != 0)
					{
						return false;
					}
				}
			}
		}
	return true;
}