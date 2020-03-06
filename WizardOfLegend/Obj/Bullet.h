#pragma once

#ifndef __BULLET_H__
#define __BULLET_H__

#include "MoveObj.h"
class CBullet : public CMoveObj
{
public:
	enum BULLSTATE { FIRE, COLLISION, END };

public:
	CBullet();
	virtual ~CBullet();

	// Inherited via CMoveObj
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime) = 0;
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f) = 0;
	virtual void Release();

	virtual void Move_Frame() = 0;
	virtual void Scene_Change() = 0;

protected:
	virtual void Change_HitRect() = 0;

public:
	virtual int		Get_Collision_Code() const { return CC_MBULLET_WALL_PUSH; }
	int				Get_Att() const { return m_iAtt; }
	bool		Get_Collision() const { return m_bCollision; }
	float		Get_ShotRange() const { return m_fShotRange; }
	bool		Get_bMonsters()  const { return m_bMonsters; }

	// _b == false -> player의 총알, _b == true -> 몬스터 총알
	void		Set_bMonsters(bool _b) { m_bMonsters = _b; }
	void		Set_Att(int _iAtt) { m_iAtt = _iAtt; }
	void		Set_Collision(bool _b) { m_bCollision = _b; }
	void		Set_ShotRange(float _fRange) { m_fShotRange = _fRange; }

protected:
	bool			m_bCollision;
	float			m_fShotRange;
	int				m_iAtt;
	LINEPOS			m_tStartPt;
	BULLSTATE		m_ePreState;
	BULLSTATE		m_eCurState;

	bool			m_bMonsters;  // false->player의 총알, _b == true->몬스터 총알
protected:
	POINT		m_tRotPoint[3];
};

#endif