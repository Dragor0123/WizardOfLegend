#pragma once
#pragma once

#ifndef __BOSS_H__
#define __BOSS_H__

#include "Monster.h"
class CBoss : public CMonster
{
public:
	CBoss();
	virtual ~CBoss();

	// Inherited via CMonster
	virtual bool Initialize();
	virtual int Update(float _fdTime) = 0;
	virtual void Late_Update(float _fdTime) = 0;
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f) = 0;
	virtual void Release();


public:
	enum BOSSDIR { LEFT, TOP, RIGHT, BOTTOM, D_END };
	enum EARTHB_STATE { IDLE, DANCE, ATT1, ATT2, ATT3_RIGHT, ATT3_DOWN, ATT3_UP, HIT, DEAD, S_END };


public:
	virtual int	Get_Collision_Code() const { return CC_MONSTER_BOSS; }
	bool		Get_Hittable() const { return m_bHittable; }

	const BOSSDIR& Get_Boss_Dir() const { return m_eDir; }
	virtual int Get_Boss_State() const = 0;
	virtual void Set_Boss_State(int _eNewState) = 0;

public:
	virtual void Scene_Change() = 0;
	virtual void Move_Frame();
	virtual CObj* Create_Bullet(const string & _frameKey = "") = 0;

protected:
	virtual void  Change_HitRect() = 0;
	virtual void Attack_Pattern(float _fdTime, float _fDis) = 0;

protected:

	BOSSDIR				m_eDir;
	EARTHB_STATE		m_ePreState;
	EARTHB_STATE		m_eCurState;

	bool				m_bDanceCool;
	int					m_iPatternCnt;
	int					m_iAttackCnt;
	bool				m_bHittable;
	static const int	B_HIT_FRAME_COUNTMAX;
};


#endif