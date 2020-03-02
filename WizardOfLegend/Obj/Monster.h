#pragma once

#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "../Hittable.h"
#include "MoveObj.h"

namespace Monster_space
{
	const DWORD WALK_FRAME_SPEED = 90;
	const DWORD HIT_FRAME_SPEED = 80;
	const DWORD DEAD_FRAME_SPEED = 150;
	const DWORD FALL_FRAME_SPEED = 1000;
}

class CMonster : public CMoveObj, public CHittable
{
public:
	enum MONSTATE { IDLE, WALK, ATTACK, HIT, DEAD, FALL, END};

public:
	void Set_Monster_State(CMonster::MONSTATE _eNewState);
	CMonster::MONSTATE Get_Monster_State() { return m_eCurState; }
	bool Get_bAttack_Cool() const { return m_bAttackCool; }
	void Set_bAttack_Cool(bool _b) { m_bAttackCool = _b; }
public:
	CMonster();
	virtual ~CMonster();

	// Inherited via CMoveObj
	virtual bool Initialize() = 0;
	virtual int Update(float _fdTime) = 0;
	virtual void Late_Update(float _fdTime) = 0;
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f) = 0;
	virtual void Release();

	void Draw_DetectCircle(HDC _DC, float _fScrollX = 0.f, float _fScrollY = 0.f);

public:
	virtual void Move_Frame() = 0;
	virtual void Scene_Change() = 0;

protected:
	virtual void  Change_HitRect() = 0;
	virtual CObj* Create_Bullet(const string& _frameKey = "") = 0;

protected:
	// 플레이어 감지하는 원의 반지름
	float			m_fDetectRange;
	MONSTATE		m_ePreState;
	MONSTATE		m_eCurState;
	bool			m_bAttackCool;
	int				m_iHitStateCount;

	static const int		M_HIT_FRAME_COUNTMAX;
};

#endif
