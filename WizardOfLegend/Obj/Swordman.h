#pragma once

#ifndef __SWORDMAN_H__
#define __SWORDMAN_H__
#include "Monster.h"
#include "MeeleBullet.h"

class CSwordman : public CMonster
{
public:
	CSwordman();
	virtual ~CSwordman();

public:
	virtual int	Get_Collision_Code() const { return CC_MONSTER_PUSHABLE; }
public:
	// Inherited via CMoveObj
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);

public:
	virtual void Scene_Change();
	virtual void Move_Frame();
protected:
	virtual void  Change_HitRect();
	virtual CObj * Create_Bullet(const string & _frameKey = "SwordmanAtt");

private:
	CMeeleBullet::ATTACKDIR m_eAttDir;
};

#endif