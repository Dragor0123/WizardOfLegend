#pragma once

#ifndef __SCREWBULLET_H__
#define __SCREWBULLET_H__
#include "Bullet.h"

class CScrewBullet : public CBullet
{
public:
	CScrewBullet();
	virtual ~CScrewBullet();

	// Inherited via CMoveObj
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();

	// 애니메이션 
	virtual void Move_Frame();
	virtual void Scene_Change();

protected:
	virtual void  Change_HitRect();

public:
	virtual int	Get_Collision_Code() const;

public:
	void Set_Rotation_Dir(bool _clockwise) { m_bClockWise = _clockwise; }

private:
	POINT		m_tCenter;
	float		m_fRotAngle;	// ScrewBullet이 회전하는 회전각
	float		m_fRotDis;		// ScrewBullet의 원궤도의 반지름
	float		m_fRotSpeed;	// ScrewBullet의 회전속도

	bool		m_bStart;
	bool		m_bClockWise;
};

#endif