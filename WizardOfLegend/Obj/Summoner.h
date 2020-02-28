#pragma once

#ifndef __SUMMONER_H__
#define __SUMMONER_H__

#include "Monster.h"
class CSummoner : public CMonster
{
public:
	CSummoner();
	~CSummoner();

public:
	virtual int	Get_Collision_Code() const { return CC_MONSTER_PUSHABLE; }

public:
	// Inherited via CMonster
	virtual bool Initialize() override;
	virtual int Update(float _fdTime) override;
	virtual void Late_Update(float _fdTime) override;
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f) override;
	virtual void Move_Frame();
	virtual void Scene_Change() override;
	virtual void Change_HitRect() override;

	virtual CObj * Create_Bullet(const string & _frameKey = "");
	CObj * Create_Bullet(float _fAngle, const string & _frameKey = "");
private:
	bool	m_bBulletTickFlag;
	float	m_fBulletTick;
	int		m_iBulletCount;

	int		m_iPattern;
	int		m_iSummonBallCount;
};

#endif // !__SUMMONER_H__
