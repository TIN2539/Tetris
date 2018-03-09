// Copyright 2009-2014 Blam Games, Inc. All Rights Reserved.

#pragma once

#include "BaseApp.h"

class TestApp : public BaseApp
{
	typedef BaseApp Parent;

private:
	int score;
	int nFieldWidth;
	int nFieldHeight;
	int CurrentPiece;
	int CurrentRotation;
	int CurrentX;
	int CurrentY;
	int Speed;
	bool GameOver;
	unsigned char *Field;
	wstring tetromino[7];
	vector<int> lines;
public:
	TestApp();
	virtual bool UpdateF(float deltaTime);
	virtual void KeyPressed(int btnCode);
	virtual void PaintField();
	void PaintTetrom();
	bool CheckMove(int nTetromino, int nRotation, int posX, int posY);
	int Rotate(int px, int py, int r);
	void NewTetromino();
	void SearchLines();
	void BlockTetromino();
	void DeleteReadyLines();
	void DeleteTetromino();
};