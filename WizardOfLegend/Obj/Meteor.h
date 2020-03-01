#pragma once

#ifndef __METEOR_H__
#define __METEOR_H__

#include "Bullet.h"

namespace Meteor_Space
{
	const float METEOR_CREATE_TIMELIMT = 0.45f;
	const DWORD METEOR_FIRE_FRAMESPEED = 150L;
	const DWORD METEOR_COLLISION_FRAMESPEED = 80L;
	const int   iEXPLOSION_CX = 288;
	const float	fY_OFFSET = 16.f;
	const float METEOR_POSY_START = 842.F;
}


class CMeteor : public CBullet
{
public:
	CMeteor();
	virtual ~CMeteor();

public:
	// Inherited via CBullet
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();

public:
	// Inherited via CBullet
	virtual void	Move_Frame() override;
	virtual void	Scene_Change() override;
	virtual int		Get_Collision_Code() const;


	float		Get_CreateTime() const { return m_fCreateTime; }
	// Áö¿ï°Í
	float		Get_LandY()
	{
		return  m_tStartPt.fY + m_fShotRange + Meteor_Space::fY_OFFSET * 0.5f;
	}
protected:
	virtual void	Change_HitRect();

private:
	CObj*			m_pCastingCircle;
	float			m_fCreateTime;
	bool			m_bCircleInit;
};

#endif