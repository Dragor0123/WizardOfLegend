#pragma once

#ifndef __EARTHLOAD_H__
#define __EARTHLOAD_H__

#include "Boss.h"

class CEarthLoad : public CBoss
{
	enum PATTERN { P_BOX, P_DRILL, P_PILLAR, P_GUIDED, P_END};

public:
	CEarthLoad();
	virtual ~CEarthLoad();

public:
	// Inherited via CMoveObj
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();

public:
	virtual void Scene_Change();
	virtual void Move_Frame();

public:
	const POINT& Get_PosinPt() const { return m_tPosin; }
	// Inherited via CBoss
	virtual int Get_Boss_State() const;
	virtual void Set_Boss_State(int _eNewState);
private:
	virtual void  Change_HitRect();
	virtual void Attack_Pattern(float _fdTime, float _fDis);

	template <typename T>
	CObj* Create_Bullet(const string& _frameKey, float _fRange = 700.f)
	{
		CObj* pObj = CAbstractFactory<T>::Create(m_tHitInfo.fX, m_tHitInfo.fY,
			m_fAngle, _frameKey);
		dynamic_cast<CBullet*>(pObj)->Set_Target(m_pTarget);
		dynamic_cast<CBullet*>(pObj)->Set_ShotRange(_fRange);
		return pObj;
	}

	template <typename T>
	CObj* Create_Bullet(const string& _frameKey, float _fRange, float _fAngle)
	{
		CObj* pObj = CAbstractFactory<T>::Create(m_tHitInfo.fX, m_tHitInfo.fY,
			_fAngle, _frameKey);
		dynamic_cast<CBullet*>(pObj)->Set_Target(m_pTarget);
		dynamic_cast<CBullet*>(pObj)->Set_ShotRange(_fRange);
		return pObj;
	}

	template <typename T>
	CObj* Create_Bullet(float _fx, float _fy, const string& _frameKey, float _fRange = 600.f)
	{
		CObj* pObj = CAbstractFactory<T>::Create(_fx, _fy,
			m_fAngle, _frameKey);
		dynamic_cast<CBullet*>(pObj)->Set_Target(m_pTarget);
		dynamic_cast<CBullet*>(pObj)->Set_ShotRange(_fRange);
		return pObj;
	}

private:
	POINT			m_tPosin;
	float			m_fPosinRange;
	PATTERN			m_ePattern;
	int				m_iPatternArr[CEarthLoad::P_END];
	int				m_iAttackCntLimit[CEarthLoad::P_END];

	// Inherited via CMonster
	virtual CObj * Create_Bullet(const string & _frameKey = "");

};

#endif