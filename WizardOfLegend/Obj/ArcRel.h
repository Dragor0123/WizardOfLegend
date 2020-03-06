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
	//Getter
	bool Is_Cool_End() { return !m_bCool; }
	bool Is_In_Inventory() const { m_bInner || m_bOutter; }
	bool Is_Fire_Available() const { return m_bOutter; }

	float Get_Remained_Cool() const;
	float Get_Remained_Cool_Ratio() const;
	const string& Get_FrameKey() const { return m_strFrameKey; }
	const string  Get_Card_FrameKey() const { return string(m_strFrameKey + "Card"); }

	// Setter

	void Set_Target(CObj* _pTarget) { m_pTarget = _pTarget; }
	void Set_FrameKey(const string& _s) { m_strFrameKey = _s; }
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

	// 시그니쳐 관련
	bool		m_bSigniture;
	float		m_fSigEffectTime;
	const float m_fSigEffectTLimit;

	bool		m_bCool;	//false 이면 쿨이 끝나서 스킬을 쓸 수 있다는 의미
	float		m_fCoolTime;
	float		m_fCoolTLimit;
	CObj*		m_pTarget;				// 소유자 -> 대개 플레이어
	bool		m_bDead;
	string		m_strFrameKey;
};

#endif


/*
GaiaShield
if (m_strFrameKey == "GaiaShieldCard") //o
m_iCardCode = 0;
if (m_strFrameKey == "DragonArcCard") //o DragonArc
m_iCardCode = 1;
if (m_strFrameKey == "FireBallCard") //o FireBall
m_iCardCode = 2;
if (m_strFrameKey == "IceSphereCard") //o IceSphere
m_iCardCode = 3;
if (m_strFrameKey == "NormalDashCard") //o 
m_iCardCode = 4;
if (m_strFrameKey == "FrostFanCard") //o FrostFan
m_iCardCode = 5;
if (m_strFrameKey == "NormalAttackCard") //o NormalAttack
m_iCardCode = 6;
if (m_strFrameKey == "EarthDrillCard")
m_iCardCode = 7;
if (m_strFrameKey == "WindFalconCard")
m_iCardCode = 8;

*/