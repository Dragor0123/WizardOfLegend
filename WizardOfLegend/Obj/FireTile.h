#pragma once

#ifndef __FIRETILE_H__
#define __FIRETILE_H__

#include "Tile.h"

class CFireTile : public CTile
{
public:
	CFireTile();
	virtual ~CFireTile();

	// Inherited via CTile
	virtual bool Initialize();
	virtual void Key_Check(float _fdTime);
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();
};

#endif

