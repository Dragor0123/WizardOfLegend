#pragma once

#ifndef __OBJ_H__
#define __OBJ_H__

//�߿�!!! �ݸ��� �ڵ� ¥�ߵȴ�
class CObj
{
public:
	CObj();
	// CObj(const CObj& _other);
	virtual ~CObj();

public:
	// �浹 �Ŵ������� dynamic_cast<> ���� �����ϱ�....
	virtual int	Get_Collision_Code() const { return 0x00; }
	const GROUPID::ID& Get_Group_ID() const { return m_eRenderGroupID; }
	const INFO& Get_Info() const { return m_tInfo; }
	const INFO& Get_HitInfo() const { return m_tHitInfo; }
	const RECT& Get_Rect() const { return m_tRect; }
	const RECT& Get_HitRect() const { return m_tHitRect; }
	const CObj* Get_Target() const { return m_pTarget; }
	float Get_PosX() const { return m_tInfo.fX; }
	float Get_PosY() const { return m_tInfo.fY; }
	float Get_Angle() const { return m_fAngle; }
	bool Get_Dead() const { return m_bDead; }
	const string& Get_FrameKey() const { return m_strFrameKey; }
	OBJID::ID Get_OBJID() const { return m_eID; }
	
	void Set_OBJID(OBJID::ID _id) { m_eID = _id; }
	void Set_FrameKey(const string& _s) { m_strFrameKey = _s; }
	void Set_Pos(float _x = float(WINCX >> 1), float _y = float(WINCY >> 1));
	void Assign_PosY(float _y) { m_tInfo.fY = _y;  m_tHitInfo.fY = _y; }
	void Assign_PosX(float _x) { m_tInfo.fY = _x;  m_tHitInfo.fY = _x; }
	void Set_PosY(float _y) { m_tInfo.fY += _y; m_tHitInfo.fY = m_tInfo.fY; }
	void Set_PosX(float _x) { m_tInfo.fX += _x; m_tHitInfo.fX = m_tInfo.fX; }
	void Set_Angle(float _fAngle) { m_fAngle = _fAngle; }
	void Set_Dead() { m_bDead = true; }
	void Set_Target(CObj* _pTarget) { m_pTarget = _pTarget; }

	const float Get_Distance(const CObj* _rhs) const;
	const float Get_Distance(CObj* _rhs) const;

public:
	virtual bool Initialize() = 0;
	virtual void Key_Check(float _fdTime) = 0;
	virtual int Update(float _fdTime) = 0;
	virtual void Late_Update(float _fdTime) = 0;
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f) = 0;
	virtual void Release() = 0;

	virtual void Scene_Change();
	virtual void Move_Frame();
	void Update_Rect();
	void Update_HitRect();

protected:
	void Equalize_HitPosInfoPos() { m_tHitInfo.fX = m_tInfo.fX; m_tHitInfo.fY = m_tInfo.fY; }
	void Equalize_HitSzInfoSz() { m_tHitInfo.iCX = m_tInfo.iCX; m_tHitInfo.iCY = m_tInfo.iCY; }
	void Draw_HitBox(HDC _DC, float _fScrollX = 0.f, float _fScrollY = 0.f);
	void Draw_HitCircle(HDC _DC, float _fScrollX = 0.f, float _fScrollY = 0.f);

protected:
	GROUPID::ID		m_eRenderGroupID;
	OBJID::ID		m_eID;
	// �׸� ��ü���� ��ǥ�� ũ�⸦ �����Ѵ�.
	// ����� ��ǥ�� ũ�⸦ ���� Rect�� �����.
	INFO	m_tInfo;		
	RECT	m_tRect;		

	// ��ü�� HitBox
	INFO	m_tHitInfo;
	RECT	m_tHitRect;

	CObj*	m_pTarget;
	float	m_fAngle;
	bool	m_bDead;
	FRAME	m_tFrame;

	// ��Ʈ���̹��� FrameKey
	string		m_strFrameKey;
};

#endif