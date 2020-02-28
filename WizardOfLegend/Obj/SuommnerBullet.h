#pragma once
#include "LineBullet.h"

// 이걸 원충돌로 할까???

class CSummonerBullet : public CLineBullet
{
public:
	CSummonerBullet();
	virtual ~CSummonerBullet();

public:
	// Inherited via CMoveObj
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();

	virtual void Move_Frame();
	virtual void Scene_Change();
public:
	//CC_MBULLET_WALL_NPUSH
	virtual int	Get_Collision_Code() const { return CC_MBULLET_WALL_PUSH; }

protected:
	virtual void  Change_HitRect();
};

