#pragma once

#ifndef __GUIDELBULLET_H__
#define __GUIDELBULLET_H__

#include "Bullet.h"
class CGuideLBullet : public CBullet
{
public:
	CGuideLBullet();
	virtual ~CGuideLBullet();

	// Inherited via CMoveObj
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();

public:
	// 애니메이션 
	virtual void Move_Frame();
	virtual void Scene_Change();

public:
	virtual int	Get_Collision_Code() const;

protected:
	virtual void  Change_HitRect();

private:
	float	m_fLifeTime;
	float	m_fLaunchTime;
};

#endif