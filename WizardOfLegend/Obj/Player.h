#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "../Hittable.h"
#include "MoveObj.h"
#include "../Obj/Bullet.h"

class CPlayer : public CMoveObj, public CHittable
{
public:
	CPlayer();
	virtual ~CPlayer();

public:
	enum STATE { IDLE, WALK, DASH, ATTACK, HIT, STAMP, REBIRTH, FALL, DEAD, END };

private:
	template <typename T>
	CObj* Create_Bullet(const string& _frameKey, float _fRange)
	{
		CObj* pObj = CAbstractFactory<T>::Create (m_tHitInfo.fX, m_tHitInfo.fY,
			m_fAngle, _frameKey);
		dynamic_cast<CBullet*>(pObj)->Set_ShotRange(_fRange);
		return pObj;
	}

	template <typename T>
	CObj* Create_Bullet(float _fx, float _fy, const string& _frameKey, float _fRange)
	{
		CObj* pObj = CAbstractFactory<T>::Create(_fx, _fy,
			m_fAngle, _frameKey);
		dynamic_cast<CBullet*>(pObj)->Set_ShotRange(_fRange);
		return pObj;
	}

	template <typename T>
	CObj* Create_Bullet(float _fx, float _fy, const string& _frameKey)
	{
		CObj* pObj = CAbstractFactory<T>::Create(_fx, _fy,
			m_fAngle, _frameKey);
		return pObj;
	}

	template<typename T>
	CObj* Create_Shield(const string& _frameKey, float _fAngle)
	{
		CObj* pObj = CAbstractFactory<T>::Create(m_tInfo.fX, m_tInfo.fY,
			_fAngle, _frameKey);
		pObj->Set_Target(this);
		return pObj;
	}

public:
	// Setter
	void Set_bShot(bool _b) { m_bShot = _b; }
	void Dash_Off() { m_bDashInit = false; }
	void Restore_speed() { m_fSpeed = m_fRestoreSpeed;}
	void Set_PlayerState(CPlayer::STATE _eNewState);
	void Add_Player_Mana(int _iVal);
	void Sub_Gold(int _iGold) { m_iGold -= _iGold; if (m_iGold < 0) m_iGold = 0; }
	void Add_Gold(int _iGold) { m_iGold += _iGold; }

	// Getter
	bool	Get_bShot() const { return m_bShot; }
	int		Get_Gold() const { return m_iGold; }
	float	Get_Cur_MP_Ratio() const { return  (float)m_iMp / m_iMaxMp; }
	CPlayer::STATE Get_PlayerState() const { return m_eCurState; }
	virtual int	Get_Collision_Code() const { return CC_PLAYER; }
public:
	// Inherited via CObj
	virtual bool Initialize();
	virtual void Key_Check(float _fdTime);
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();
	void OffSet();

	// 애니매이션 Frame 관련
	virtual void Scene_Change();
	virtual void Move_Frame();

private:
	void Update_ShotAngle();
	void Change_HitRect();
	void Dash_Check(float _fdTime);

private:
	int			m_iGold;

	STATE		m_ePreState;
	STATE		m_eCurState;
	int			m_iHitStateCount;
	int			m_iMp;
	int			m_iMaxMp;
	//bool		m_bDeadInit;		// true일경우 SceneChange 에서 STATE를 DEAD로 바꾼다.
									// DEAD프레임 끝나면 플레이어 소멸시킨다.

	// 대쉬 관련
	LINEPOS		m_tBeforeDashPos;
	bool		m_bDashInit;
	float		m_fDashSpeed;
	float		m_fRestoreSpeed;

	// 이 부분 수정 필요
	bool		m_bSkillCool; // <-이거 나중에 지워준다!
	float		m_fCoolStart;
	float		m_fCoolLimit;
	float		m_fBulletTick;
	int			m_iBulletCount;


	// space : 0, LButton : 1, RButton : 2, Q : 3, E : 4, R : 5
	map<BYTE, int> m_mapKeyIdx;



	/////// 급조된 멤버
	bool m_bShot;
};

#endif // !__PLAYER_H__