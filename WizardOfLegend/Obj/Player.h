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
	void Init_Effect_Line();
	void Dash_Off() { m_bDashInit = false; }
	void Restore_speed() { m_fSpeed = m_fRestoreSpeed;}
	void Set_PlayerState(CPlayer::STATE _eNewState);
	void Add_MP(int _iVal);
	void Set_MP_Zero() { m_iMp = 0; }
	void Sub_Gold(int _iGold) { m_iGold -= _iGold; if (m_iGold < 0) m_iGold = 0; }
	void Add_Gold(int _iGold) { m_iGold += _iGold; }
	void Set_Robe_Color(const ROBE::COLOR& _eColor) { m_eRobeColor = _eColor; }
	void Set_Skill_Code(int _skill) { m_iSkillCode = _skill; }
	void Set_Signiture(bool _b) { m_bSigniture = _b; }
	void Set_DontDraw(bool _b) { m_bDontDraw = _b; }

	// Getter
	bool	Is_MP_Max() const { return m_iMp == m_iMaxMp; }
	int		Get_Skill_Code() const { return m_iSkillCode; }
	int		Get_Gold() const { return m_iGold; }
	float	Get_Cur_MP_Ratio() const { return  (float)m_iMp / m_iMaxMp; }
	CPlayer::STATE Get_PlayerState() const { return m_eCurState; }
	virtual int	Get_Collision_Code() const { return CC_PLAYER; }
	const ROBE::COLOR& Get_Robe_Color() const { return m_eRobeColor;}
	bool	Get_Signiture() const { return m_bSigniture; }

	//void Set_bShot(bool _b) { m_bPressingShot = _b; }
	//bool	Get_bShot() const { return m_bPressingShot; }
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
	void Skill_Button_KeyCheck(int _iKey);
	void Update_ShotAngle();
	void Change_HitRect();
	void Dash_Check(float _fdTime);
	void Render_Sig_Effect(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY);

private:
	ROBE::COLOR m_eRobeColor;
	int			m_iGold;
	STATE		m_ePreState;
	STATE		m_eCurState;
	int			m_iHitStateCount;
	int			m_iMp;
	int			m_iMaxMp;
	int			m_iSkillCode;
	bool		m_bNormalAttackFire;
	//bool		m_bDeadInit;		// true일경우 SceneChange 에서 STATE를 DEAD로 바꾼다.
									// DEAD프레임 끝나면 플레이어 소멸시킨다.

	bool		m_bDontDraw;
	// 대쉬 관련
	LINEPOS		m_tBeforeDashPos;
	bool		m_bDashInit;
	float		m_fDashSpeed;
	float		m_fRestoreSpeed;

	// 시그니쳐 관련
	bool		m_bSigniture;
	EFFECTLINE	m_tEffectLine[32];

	// space : 0, LButton : 1, RButton : 2, Q : 3, E : 4, R : 5
	map<BYTE, int> m_mapKeyIdx;
};

#endif // !__PLAYER_H__