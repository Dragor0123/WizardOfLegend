#pragma once

#ifndef __FIREBULLET_H__
#define __FIREBULLET_H__

#include "LineBullet.h"
class CFireBall : public CLineBullet
{
public:
	CFireBall();
	virtual ~CFireBall();

public:
	// Inherited via CMoveObj
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();

	// 애니메이션 
	virtual void Move_Frame();
	virtual void Scene_Change();

private:
	virtual void  Change_HitRect();

public:
	virtual int	Get_Collision_Code() const;

private:
	float		m_fRisingTime;
	float		m_fRisingLimit;
};

#endif