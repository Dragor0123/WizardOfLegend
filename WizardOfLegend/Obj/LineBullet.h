#pragma once

#ifndef __LINEBULLET_H__
#define __LINEBULLET_H__

#include "Bullet.h"
class CLineBullet : public CBullet
{
public:
	CLineBullet();
	virtual ~CLineBullet();

	// Inherited via CMoveObj
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();

	// 애니메이션 
	virtual void Move_Frame();
	virtual void Scene_Change();
public:
	virtual int	Get_Collision_Code() const { return CC_MBULLET_WALL_PUSH; }
protected:
	virtual void  Change_HitRect();
};

#endif