#pragma once

#ifndef __ARCHER_H__
#define __ARCHER_H__

#include "Monster.h"
#define BOWSIZE 150

class CArcher : public CMonster
{
public:
	CArcher();
	virtual ~CArcher();

public:
	virtual int	Get_Collision_Code() const { return CC_MONSTER_PUSHABLE; }

public:
	// Inherited via CMoveObj
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);

protected:
	virtual CObj* Create_Bullet(const string& _frameKey = "");
	virtual void  Change_HitRect();

public:
	virtual void Scene_Change();
	virtual void Move_Frame() override;
private:
	POINT		m_tRotPoint[3];


	// Inherited via CMonster

	//float		m_fCoolStart;
	//float		m_fCoolLimit;
};

#endif