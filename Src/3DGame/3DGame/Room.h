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
		vrtx(0, 0, 0, CLR_BLUE),		//0
		vrtx(2, 0, 0, CLR_BLUE),		//1
		vrtx(8, 0, 0, CLR_BLUE),		//2
		vrtx(10, 0, 0, CLR_BLUE),		//3
		vrtx(2, 0, 2, CLR_BLUE),		//4	
		vrtx(8, 0, 2, CLR_BLUE),		//5
		vrtx(0, 0, 4, CLR_BLUE),		//6
		vrtx(2, 0, 4, CLR_BLUE),		//7
		vrtx(8, 0, 4, CLR_BLUE),		//8
		vrtx(10, 0, 4, CLR_BLUE),		//9
		vrtx(2, 0, 6, CLR_BLUE),		//10
		vrtx(8, 0, 6, CLR_BLUE),		//11
		vrtx(0, 0, 8, CLR_BLUE),		//12
		vrtx(2, 0, 8, CLR_BLUE),		//13
		vrtx(8, 0, 8, CLR_BLUE),		//14
		vrtx(10, 0, 8, CLR_BLUE),		//15
		vrtx(0, 0, 10, CLR_BLUE),		//16
		vrtx(2, 0, 10, CLR_BLUE),		//17
		vrtx(8, 0, 10, CLR_BLUE),		//18
		vrtx(10, 0, 10, CLR_BLUE),		//19
		vrtx(0, -1, 0, CLR_DARKGRAY),	//20
		vrtx(2, -1, 0, CLR_DARKGRAY),	//21
		vrtx(8, -1, 0, CLR_DARKGRAY),	//22
		vrtx(10, -1, 0, CLR_DARKGRAY),	//23
		vrtx(2, -1, 2, CLR_DARKGRAY),	//24
		vrtx(8, -1, 2, CLR_DARKGRAY),	//25
		vrtx(0, -1, 4, CLR_DARKGRAY),	//26
		vrtx(2, -1, 4, CLR_DARKGRAY),	//27
		vrtx(8, -1, 4, CLR_DARKGRAY),	//28
		vrtx(10, -1, 4, CLR_DARKGRAY),	//29
		vrtx(2, -1, 6, CLR_DARKGRAY),	//30
		vrtx(8, -1, 6, CLR_DARKGRAY),	//31
		vrtx(0, -1, 8, CLR_DARKGRAY),	//32
		vrtx(2, -1, 8, CLR_DARKGRAY),	//33
		vrtx(8, -1, 8, CLR_DARKGRAY),	//34
		vrtx(10, -1, 8, CLR_DARKGRAY),	//35
		vrtx(0, -1, 10, CLR_DARKGRAY),	//36
		vrtx(2, -1, 10, CLR_DARKGRAY),	//37
		vrtx(8, -1, 10, CLR_DARKGRAY),	//38
		vrtx(10, -1, 10, CLR_DARKGRAY)	//39
	};

	static const size_t tLen = 48;
	const int trgls[tLen][3]
	{
		/* Floor */
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
		{ 6, 10, 12 },	//12
		{ 9, 11, 15 },	//14
		{ 10, 12, 13 },	//13
		{ 11, 14, 15 },	//15
		{ 12, 13, 17 },	//16
		{ 12, 16, 17 },	//20
		{ 13, 14, 17 },	//17
		{ 14, 17, 18 },	//18
		{ 14, 15, 18 },	//19
		{ 15, 18, 19 },	//21
		/* Ceiling */
		{ 20, 21, 24 },	//0
		{ 21, 22, 24 },	//1
		{ 22, 23, 25 },	//3
		{ 20, 24, 26 },	//4
		{ 24, 22, 25 },	//2
		{ 23, 25, 29 },	//6
		{ 24, 26, 27 },	//5
		{ 25, 28, 29 },	//7
		{ 26, 27, 30 },	//8
		{ 27, 28, 30 },	//9
		{ 30, 28, 31 },	//10
		{ 28, 29, 31 },	//11
		{ 26, 30, 32 },	//12
		{ 29, 31, 35 },	//14
		{ 30, 32, 33 },	//13
		{ 31, 34, 35 },	//15
		{ 32, 33, 37 },	//16
		{ 32, 36, 37 },	//20
		{ 33, 34, 37 },	//17
		{ 34, 37, 38 },	//18
		{ 34, 35, 38 },	//19
		{ 35, 38, 39 },	//21
		/* Walls */
		{ 0, 1, 20 },	//0L
		{ 20, 21, 1},	//0U
		{ 1, 2, 21 },	//1L
		{ 21, 22, 2 },	//1U
	};

	void OnRender(void) override;
	void OnMouseMove(int x, int y, int dX, int dY) override;
	void OnKeyDown(int sym) override;
} room;