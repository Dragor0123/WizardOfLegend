#pragma once

#ifndef __ARCREL_H__
#define __ARCREL_H__

class CObj;
class CArcRel
{
public:
	CArcRel();
	virtual ~CArcRel();

public:
	// Inherited via CStopObj
	virtual bool Initialize() = 0;
	virtual int Update(float _fdTime) = 0;
	virtual void Late_Update(float _fdTime) = 0;
	virtual void Release();

public:
	bool Is_Cool_End() { return !m_bCool; }
	float Get_Remained_Cool() const;
	float Get_Remained_Cool_Ratio() const;

	void Set_Target(CObj* _pTarget) { m_pTarget = _pTarget; }
	void Set_FrameKey(const string& _s) { m_strFrameKey = _s; }

	bool Is_In_Inventory() const { m_bInner || m_bOutter; }
	bool Is_Fire_Available() const { return m_bOutter; }

	void Set_Outter_On() { m_bInner = false; m_bOutter = true; }
	void Set_Inner_On() { m_bInner = true; m_bOutter = false; }
	void Set_Outter_Inner_Off() { m_bInner = m_bOutter = false; }

public:
	virtual void Fire_Skill() = 0;
	virtual void Key_Up_Action() = 0;
	virtual void Passive_Effect() = 0;

protected:
	bool		m_bOutter;
	bool		m_bInner;

	bool		m_bCool;	//false 이면 쿨이 끝나서 스킬을 쓸 수 있다는 의미
	float		m_fCoolTime;
	float		m_fCoolTLimit;
	CObj*		m_pTarget;				// 소유자 -> 대개 플레이어
	bool		m_bDead;
	string		m_strFrameKey;
};

#endif