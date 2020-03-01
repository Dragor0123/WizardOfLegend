#pragma once

#ifndef __NOMOVEBULLET_H__
#define __NOMOVEBULLET_H__

#include "Bullet.h"
class CNoMoveBullet : public CBullet
{
public:
	CNoMoveBullet();
	virtual ~CNoMoveBullet();

public:
	// Inherited via CBullet
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();


	// Inherited via CBullet
	virtual void Move_Frame() override;
	virtual void Scene_Change() override;
public:
	virtual int		Get_Collision_Code() const;

protected:
	virtual void	Change_HitRect();
private:
	bool		m_bCrevisInit;
	float		m_fPillarCreateTime;
};

#endif