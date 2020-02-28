#pragma once

#ifndef __PILLARMAKER_H__
#define __PILLARMAKER_H__

#include "LineBullet.h"

// EarthPillarMaker ��ü�� �÷��̾�� �浹���� �ʴ´�.
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

	// �ִϸ��̼� 
	virtual void Move_Frame();
	virtual void Scene_Change();
	virtual void Change_HitRect();

public:
	virtual int		Get_Collision_Code() const { return CC_ONLYTILE_COLLISION; }

private:
	float		m_fInterval;
};

#endif