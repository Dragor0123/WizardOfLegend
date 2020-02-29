#pragma once
#include "LineBullet.h"

class CFrostFan : public CLineBullet
{
public:
	CFrostFan();
	virtual ~CFrostFan();

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
	virtual int	Get_Collision_Code() const;
	bool Get_bHit() const { return m_bHit; }

	void Set_bHit(bool _b) { m_bHit = _b; }
protected:
	virtual void  Change_HitRect();

private:
	bool		m_bHit;	//몬스터를 딱 한번만 때림. 몬스터랑 충돌은 나도 그냥 지나만 갑니다.

};

