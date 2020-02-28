#pragma once

#ifndef __PILLARMAKER_H__
#define __PILLARMAKER_H__

#include "LineBullet.h"

// EarthPillarMaker 자체는 플레이어와 충돌하지 않는다.
class CEarthPillarMaker : public CLineBullet
{
public:
	CEarthPillarMaker();
	virtual ~CEarthPillarMaker();

	// Inherited via CMoveObj
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();

	// 애니메이션 
	virtual void Move_Frame();
	virtual void Scene_Change();
	virtual void Change_HitRect();

public:
	virtual int		Get_Collision_Code() const { return CC_ONLYTILE_COLLISION; }

private:
	float		m_fInterval;
};

#endif