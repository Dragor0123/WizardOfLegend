#pragma once

#ifndef __ARROW_H__
#define __ARROW_H__
#include "LineBullet.h"

class CArrow : public CLineBullet
{
public:
	CArrow();
	virtual ~CArrow();

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
	//CC_MBULLET_WALL_NPUSH
	virtual int	Get_Collision_Code() const { return CC_MBULLET_WALL_PUSH; }
protected:
	virtual void  Change_HitRect();
};

#endif