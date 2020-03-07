#pragma once

#ifndef __FIREBOSS_H__
#define __FIREBOSS_H__
#include "Boss.h"

class CFireBoss : public CBoss
{
public:
	enum PATTERN { P_FIREBALL, P_KICK, P_DRAGON, P_METEOR, P_END };
	enum FIREB_STATE {
		FIRE_IDLE, FIRE_DANCE, 
		ATT_DRAGON_RIGHT, ATT_DRAGON_DOWN, ATT_DRAGON_UP, ATT_FIREBALL_RIGHT, ATT_FIREBALL_UP, FIRE_HIT, FIRE_DEAD,
		ATT_FIREBALL_DOWN, ATT_MOVE_RIGHT, ATT_MOVE_DOWN, ATT_MOVE_UP, ATT_KICK, ATT_JUMP, FIRE_S_END
	};

public:
	CFireBoss();
	virtual ~CFireBoss();

	// Inherited via CBoss
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();

public:
	virtual void Scene_Change();
	virtual void Move_Frame();

	virtual int Get_Boss_State() const;
	bool Get_bJump() const { return m_bJump; }
	virtual void Set_Boss_State(int _eNewState);

private:
	virtual void Change_HitRect() override;
	virtual void Attack_Pattern(float _fdTime, float _fDis) override;

	virtual CObj * Create_Bullet(const string & _frameKey = "") override;

	CObj* Create_DragonBullet(float _fx, float _fy, bool _bClockWise);

	template <typename T>
	CObj* Create_Bullet(float _fx, float _fy, const string& _frameKey, float _fAngle)
	{
		CObj* pObj = CAbstractFactory<T>::Create(_fx, _fy,
			_fAngle, _frameKey);
		dynamic_cast<CBullet*>(pObj)->Set_Target(m_pTarget);
		return pObj;
	}

	template <typename T>
	CObj* Create_Bullet(float _fx, float _fy, const string& _frameKey)
	{
		CObj* pObj = CAbstractFactory<T>::Create(_fx, _fy, _frameKey);
		dynamic_cast<CBullet*>(pObj)->Set_Target(m_pTarget);
		return pObj;
	}

	template <typename T>
	CObj* Create_Bullet(float _fx, float _fy, const string& _frameKey, CObj* _pTarget)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(_fx, _fy);
		pObj->Set_FrameKey(_frameKey);
		pObj->Set_Target(_pTarget);
		pObj->Initialize();
		return pObj;
	}

	template <typename T>
	CObj* Create_Bullet(float _fx, float _fy, const string& _frameKey, float _fAngle, float _fRange)
	{
		CObj* pObj = CAbstractFactory<T>::Create(_fx, _fy,
			_fAngle, _frameKey);
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
	CObj* Create_Bullet(const string& _frameKey)
	{
		CObj* pObj = CAbstractFactory<T>::Create(m_tHitInfo.fX, m_tHitInfo.fY,
			m_fAngle, _frameKey);
		dynamic_cast<CBullet*>(pObj)->Set_Target(m_pTarget);
		return pObj;
	}

	void Jumping(float _fdTime, float _fAngle = 90.f);

private:
	POINT			m_tPosin;
	float			m_fPosinRange;
	float			m_fPosinAng;

	PATTERN			m_ePattern;
	int				m_iPatternArr[CFireBoss::P_END];
	int				m_iAttackCntLimit[CFireBoss::P_END];

	float			m_fBulletTick[CFireBoss::P_END];
	float			m_fBulletTickLimit[CFireBoss::P_END];
	
	bool			m_bJump;
	bool			m_bMeteoFire;
	float			m_fMeteoTime;
	float			m_fJumpDirAngle;
	float			m_fJumpLandY;
	float			m_fJumpPower;
	float			m_fJumpTime;

	bool			m_bDragonFire;
	float			m_fDragonTime;

	bool			m_bMoveStateFlag;
	bool			m_bKickFire;
	bool			m_bKick_bulletCreate;

	FIREB_STATE			m_eFIREPreState;
	FIREB_STATE			m_eFIRECurState;
};

#endif