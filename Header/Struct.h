#pragma once

#ifndef __STRUCT_H__
#define __STRUCT_H__

typedef struct tagInfo
{
	// ��ǥ�� �ڷ����� float���� ������ ������ ������ ������ �� ���
	// �Ҽ��� �ڸ��� ���� ������ ������ �ϱ� ���ؼ� �̴�.
	float	fX;
	float	fY;

	// ũ��� �Ҽ����� �����Ͽ� �����ϰ� �׸� �ʿ䰡 ����.
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
	int		iFrameStart;	// frame x�� 0��°
	int		iFrameEnd;		// frame x�� ������
	int		iFrameScene;	//frame Y��
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
