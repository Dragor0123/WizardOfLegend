#pragma once

#ifndef __SANDBAG_H__
#define __SANDBAG_H__

#include "Monster.h"

class CSandbag : public CMonster
{
public:
	CSandbag();
	virtual ~CSandbag();

	// Inherited via CMonster
	virtual bool Initialize() override;
	virtual int Update(float _fdTime) override;
	virtual void Late_Update(float _fdTime) override;
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f) override;
	virtual void Release() override;

public:
	virtual int	Get_Collision_Code() const { return CC_MONSTER_PUSHABLE; }

	// Inherited via CMonster
	virtual void Scene_Change() override;
	virtual void Change_HitRect() override;
	virtual CObj * Create_Bullet(const string & _frameKey = "") override;
};

#endif
