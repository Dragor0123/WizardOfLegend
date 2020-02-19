#pragma once

#ifndef __STRUCT_H__
#define __STRUCT_H__

typedef struct tagInfo
{
	// 좌표의 자료형을 float으로 설정한 이유는 수학적 연산이 들어갈 경우
	// 소수점 자리를 토대로 정밀한 연산을 하기 위해서 이다.
	float	fX;
	float	fY;

	// 크기는 소수점을 포함하여 정밀하게 그릴 필요가 없다.
	int		iCX;
	int		iCY;

	tagInfo() : fX(0.f), fY(0.f), iCX(0), iCY(0) {}
	tagInfo(float _fx, float _fy) : fX(_fx), fY(_fy), iCX(30), iCY(30) {}
	tagInfo(int _cx, int _cy) : fX(0.f), fY(0.f), iCX(_cx), iCY(_cy) {}
	tagInfo(float _fx, float _fy, int _cx, int _cy)
		: fX(_fx), fY(_fy), iCX(_cx), iCY(_cy) {}

	tagInfo(const POINT& pt, int _cx, int _cy)
	{
		fX = (float)pt.x;
		fY = (float)pt.y;
		iCX = _cx;
		iCY = _cy;
	}
}INFO;

typedef struct tagFrame
{
	int		iFrameStart;	// frame x축 0번째
	int		iFrameEnd;		// frame x축 마지막
	int		iFrameScene;	//frame Y축
	DWORD	dwFrameSpeed;
	DWORD	dwFrameTime;
}FRAME;

typedef struct tagLinePos
{
	tagLinePos() { fX = 0.f, fY = 0.f; }
	tagLinePos(float _x, float _y) : fX(_x), fY(_y) {}

	float fX;
	float fY;
}LINEPOS;

typedef struct tagLineInfo
{
	tagLineInfo() { ZeroMemory(this, sizeof(tagLineInfo)); }
	tagLineInfo(const LINEPOS& _tLeft, const LINEPOS& _tRight)
		: tLeftPos(_tLeft), tRightPos(_tRight) {}

	LINEPOS tLeftPos;
	LINEPOS tRightPos;
}LINEINFO;

#endif // !__STRUCT_H__
