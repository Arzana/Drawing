#pragma once

#include <Game_WIN.h>
#include "Camera.h"

typedef struct RoomGame
	: public winGame
{
public:
	RoomGame(const uint w, const uint h);
	~RoomGame(void);
private:
	const float scalar = 0.1f;
	Camera *cam;

	static const size_t vLen = 40;
	const vrtx vertices[vLen] =
	{
		vrtx(0, 0, 0, CLR_WHITE),		//0
		vrtx(2, 0, 0, CLR_WHITE),		//1
		vrtx(8, 0, 0, CLR_WHITE),		//2
		vrtx(10, 0, 0, CLR_WHITE),		//3
		vrtx(2, 0, 2, CLR_WHITE),		//4	
		vrtx(8, 0, 2, CLR_WHITE),		//5
		vrtx(0, 0, 4, CLR_WHITE),		//6
		vrtx(2, 0, 4, CLR_WHITE),		//7
		vrtx(8, 0, 4, CLR_WHITE),		//8
		vrtx(10, 0, 4, CLR_WHITE),		//9
		vrtx(2, 0, 6, CLR_WHITE),		//10
		vrtx(8, 0, 6, CLR_WHITE),		//11
		vrtx(0, 0, 8, CLR_WHITE),		//12
		vrtx(2, 0, 8, CLR_WHITE),		//13
		vrtx(8, 0, 8, CLR_WHITE),		//14
		vrtx(10, 0, 8, CLR_WHITE),		//15
		vrtx(0, 0, 10, CLR_WHITE),		//16
		vrtx(2, 0, 10, CLR_WHITE),		//17
		vrtx(8, 0, 10, CLR_WHITE),		//18
		vrtx(10, 0, 10, CLR_WHITE),		//19
		vrtx(0, -1, 0, CLR_WHITE),		//20
		vrtx(2, -1, 0, CLR_WHITE),		//21
		vrtx(8, -1, 0, CLR_WHITE),		//22
		vrtx(10, -1, 0, CLR_WHITE),		//23
		vrtx(2, -1, 2, CLR_WHITE),		//24
		vrtx(8, -1, 2, CLR_WHITE),		//25
		vrtx(0, -1, 4, CLR_WHITE),		//26
		vrtx(2, -1, 4, CLR_WHITE),		//27
		vrtx(8, -1, 4, CLR_WHITE),		//28
		vrtx(10, -1, 4, CLR_WHITE),		//29
		vrtx(2, -1, 6, CLR_WHITE),		//30
		vrtx(8, -1, 6, CLR_WHITE),		//31
		vrtx(0, -1, 8, CLR_WHITE),		//32
		vrtx(2, -1, 8, CLR_WHITE),		//33
		vrtx(8, -1, 8, CLR_WHITE),		//34
		vrtx(10, 1, 8, CLR_WHITE),		//35
		vrtx(0, -1, 10, CLR_WHITE),		//36
		vrtx(2, -1, 10, CLR_WHITE),		//37
		vrtx(8, -1, 10, CLR_WHITE),		//38
		vrtx(10, -1, 10, CLR_WHITE)		//39
	};

	static const size_t tLen = 12;
	const int trgls[tLen][3]
	{
		{ 0, 1, 4 },	//0
		{ 1, 2, 4 },	//1
		{ 2, 3, 5 },	//3
		{ 0, 4, 6 },	//4
		{ 4, 2, 5 },	//2
		{ 3, 5, 9 },	//6
		{ 4, 6, 7 },	//5
		{ 5, 8, 9 },	//7
		{ 6, 7, 10 },	//8
		{ 7, 8, 10 },	//9
		{ 10, 8, 11 },	//10
		{ 8, 9, 11 },	//11
	};

	void OnUpdate(GameTime time) override;
	void OnRender(void) override;
	void OnMouseMove(int x, int y, int dX, int dY) override;
	void OnKeyDown(int sym) override;
} room;