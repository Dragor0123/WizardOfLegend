#pragma once

#ifndef __SPELLSELLER_H__
#define __SPELLSELLER_H__

#include "NPC.h"
class CSpellSeller : public CNPC
{
public:
	CSpellSeller();
	virtual ~CSpellSeller();

public:
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();

public:
	// Inherited via CFAble
	virtual void Do_FButton_Action(float _fdTime);
};

#endif 