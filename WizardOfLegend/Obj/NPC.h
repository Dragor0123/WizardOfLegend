#pragma once

#ifndef __NPC_H__
#define __NPC_H__

#include "FAble.h"
#include "Emoji.h"

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

public:
	void Set_Emoji_State(const CEmoji::EMO_STATE& _eNewState) { m_eEmoState = _eNewState; }
	
	const CEmoji::EMO_STATE& Get_Emoji_State() const { return m_eEmoState; }

protected:
	TCHAR				szDialogue[128];
	CMyBitmap*			m_pMyBitmap;
	CEmoji::EMO_STATE	m_eEmoState;
};

#endif