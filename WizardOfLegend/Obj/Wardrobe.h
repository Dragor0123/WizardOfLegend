#pragma once

#ifndef __WARDROBE_H__
#define __WARDROBE_H__

#include "NPC.h"

class CWardrobe : public CNPC
{
public:
	enum WARD_STATE { IDLE, OPEN, CLOSE, END };

public:
	CWardrobe();
	virtual ~CWardrobe();

public:
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();

public:
	virtual void Scene_Change();
	virtual void Move_Frame();

public:
	void Close_Wardrobe() { m_eCurState = WARD_STATE::CLOSE; }

public:
	// Inherited via CFAble
	virtual void Do_FButton_Action(float _fdTime);

private:
	static const DWORD IDLE_FRAME_SPEED;
	static const DWORD OPEN_FRAME_SPEED;
	WARD_STATE		m_ePreState;
	WARD_STATE		m_eCurState;
	CObj*			m_pWardInven;
};

#endif