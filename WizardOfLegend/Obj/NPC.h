#pragma once

#ifndef __NPC_H__
#define __NPC_H__

#include "FAble.h"

class CMyBitmap;

class CNPC : public CFAble
{
public:
	CNPC();
	virtual ~CNPC();

public:
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();

public:
	// Inherited via CFAble
	virtual void Do_FButton_Action(float _fdTime) = 0;
	virtual int Get_Collision_Code() const { return CC_FABLE_NPC; }

protected:
	TCHAR			szDialogue[128];
	CMyBitmap*		m_pMyBitmap;
};

#endif