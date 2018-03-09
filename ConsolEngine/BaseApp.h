// Copyright 2009-2014 Blam Games, Inc. All Rights Reserved.

#pragma once

#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>

using namespace std;

class BaseApp
{
private:
	HANDLE mConsole;
	HANDLE mConsoleIn;

	CHAR_INFO* mChiBuffer;
	COORD mDwBufferSize;
	COORD mDwBufferCoord;
	SMALL_RECT mLpWriteRegion;

	void Render();

public:
	//������� ������� ������ �� ����������� � ��������� � ��������
	const int X_SIZE;
	const int Y_SIZE;

	//����������� �������� ������� ������� ������ �� ����������� � ��������� � ��������
	BaseApp(int xSize = 80, int ySize = 25);
	virtual ~BaseApp();

	//��������� ������� ����
	void Run();

	//����� ��������� x,y-������ ������ ������������ ��������, ��� ������� ���
	void SetChar(int x, int y, wchar_t c);
	wchar_t GetChar(int x, int y);
	void WriteStr(int x, int y, const char *str, int score);

	/*��� ������� ���������� ������ ������� ��������, � ����� ��������������, � ���������� ������.
	� �� �������� deltaTime - ������� �� ������� ����� ���������� ��������� � ����, � ��������*/
	virtual bool UpdateF(float deltaTime) { return 1; }
	/*��� ������� ���������� ��� ������� ������� �� ����������, � �� �������� ��� ������� - btnCode.
	���� ������������ ������� ��� �������������� �������, �� ������ ����� ��� ��� �����, �������� 224, � ��������� �� ��� -
	����� ��� ��������������� ��� ����� �������, ��� ����� ��������, ������ ����� getch().
	����� KeyPressed ��� �� ����� �������������� � ����������*/
	virtual void KeyPressed(int btnCode){}
};