#include "../stdafx.h"
#include "MoveObj.h"


CMoveObj::CMoveObj()
	: m_fSpeed(0.f), m_eMoveDir(MOVEDIR::MD_MIDDLE)
{
}


CMoveObj::~CMoveObj()
{
}

bool CMoveObj::Initialize()
{
	m_fSpeed = 400.f;
	return true;
}

void CMoveObj::Key_Check(float _fdTime)
{
}

int CMoveObj::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Rect();
	Update_HitRect();
	return OBJ_NOEVENT;
}

void CMoveObj::Late_Update(float _fdTime)
{
	// Move Obj들의 공통된 Late_Update 처리
}

void CMoveObj::Move_Frame()
{
	CObj::Move_Frame();
}

// m_eMoveDir 갱신 안함
void CMoveObj::Move(float x, float y)
{
	m_tInfo.fX += x;
	m_tInfo.fY += y;
	Equalize_HitPosInfoPos();
}

// m_eMoveDir 갱신 안함
void CMoveObj::Move(float x, float y, float fDeltaTime)
{
	m_tInfo.fX += x * fDeltaTime;
	m_tInfo.fY += y * fDeltaTime;
	Equalize_HitPosInfoPos();
}

void CMoveObj::MoveXFromSpeed(float fDeltaTime, MOVEDIR::FB  eDir)
{
	m_tInfo.fX += m_fSpeed * fDeltaTime * eDir;
	Equalize_HitPosInfoPos();
	if (eDir == MOVEDIR::MD_BACK)
		m_eMoveDir = MOVEDIR::MD_LEFT;
	else if (eDir == MOVEDIR::MD_FRONT)
		m_eMoveDir = MOVEDIR::MD_RIGHT;
}

void CMoveObj::MoveYFromSpeed(float fDeltaTime, MOVEDIR::FB  eDir)
{
	m_tInfo.fY += m_fSpeed * fDeltaTime * eDir;
	Equalize_HitPosInfoPos();
	if (eDir == MOVEDIR::MD_BACK)
		m_eMoveDir = MOVEDIR::MD_TOP;
	else if (eDir == MOVEDIR::MD_FRONT)
		m_eMoveDir = MOVEDIR::MD_BOT;
}

void CMoveObj::MoveX(float x)
{
	m_tInfo.fX += x;
	Equalize_HitPosInfoPos();

	if (x < 0)
		m_eMoveDir = MOVEDIR::MD_LEFT;
	else if (x > 0)
		m_eMoveDir = MOVEDIR::MD_RIGHT;
}

void CMoveObj::MoveX(float yourSpeed, float fDeltaTime, MOVEDIR::FB eDir)
{
	m_tInfo.fX += yourSpeed * fDeltaTime * eDir;
	Equalize_HitPosInfoPos();

	if (eDir == MOVEDIR::MD_BACK)
		m_eMoveDir = MOVEDIR::MD_LEFT;
	else if (eDir == MOVEDIR::MD_FRONT)
		m_eMoveDir = MOVEDIR::MD_RIGHT;
}

void CMoveObj::MoveY(float y)
{
	m_tInfo.fY += y;
	Equalize_HitPosInfoPos();
	if (y < 0)
		m_eMoveDir = MOVEDIR::MD_TOP;
	else if (y > 0)
		m_eMoveDir = MOVEDIR::MD_BOT;
}

void CMoveObj::	MoveY(float yourSpeed, float fDeltaTime, MOVEDIR::FB eDir)
{
	m_tInfo.fY += yourSpeed * fDeltaTime * eDir;
	Equalize_HitPosInfoPos();
	if (eDir == MOVEDIR::MD_BACK)
		m_eMoveDir = MOVEDIR::MD_TOP;
	else if (eDir == MOVEDIR::MD_FRONT)
		m_eMoveDir = MOVEDIR::MD_BOT;
}

// m_fAngle is Radian angle
void CMoveObj::MoveAngle()
{
	m_tInfo.fX += cosf(Degree_To_Radian(m_fAngle)) * m_fSpeed;
	m_tInfo.fY -= sinf(Degree_To_Radian(m_fAngle)) * m_fSpeed;
	Equalize_HitPosInfoPos();
}

// m_fAngle is Radian angle
void CMoveObj::MoveAngle(float fDeltaTime)
{
	m_tInfo.fX += cosf(Degree_To_Radian(m_fAngle)) * m_fSpeed * fDeltaTime;
	m_tInfo.fY -= sinf(Degree_To_Radian(m_fAngle)) * m_fSpeed * fDeltaTime;
	Equalize_HitPosInfoPos();
}

void CMoveObj::MoveLeftTop(float fDeltaTime)
{
	m_tInfo.fX -= fDeltaTime * m_fSpeed / sqrtf(2.f);
	m_tInfo.fY -= fDeltaTime * m_fSpeed / sqrtf(2.f);
	Equalize_HitPosInfoPos();
	m_eMoveDir = MOVEDIR::MD_LT;
}

void CMoveObj::MoveLeftBottom(float fDeltaTime)
{
	m_tInfo.fX -= fDeltaTime * m_fSpeed / sqrtf(2.f);
	m_tInfo.fY += fDeltaTime * m_fSpeed / sqrtf(2.f);
	Equalize_HitPosInfoPos();
	m_eMoveDir = MOVEDIR::MD_LB;
}

void CMoveObj::MoveRightTop(float fDeltaTime)
{
	m_tInfo.fX += fDeltaTime * m_fSpeed / sqrtf(2.f);
	m_tInfo.fY -= fDeltaTime * m_fSpeed / sqrtf(2.f);
	Equalize_HitPosInfoPos();
	m_eMoveDir = MOVEDIR::MD_RT;
}

void CMoveObj::MoveRightBottom(float fDeltaTime)
{
	m_tInfo.fX += fDeltaTime * m_fSpeed / sqrtf(2.f);
	m_tInfo.fY += fDeltaTime * m_fSpeed / sqrtf(2.f);
	Equalize_HitPosInfoPos();
	m_eMoveDir = MOVEDIR::MD_RB;
}

void CMoveObj::MoveLeftTop(float yourspeed, float fDeltaTime)
{
	m_tInfo.fX -= fDeltaTime * yourspeed / sqrtf(2.f);
	m_tInfo.fY -= fDeltaTime * yourspeed / sqrtf(2.f);
	Equalize_HitPosInfoPos();
	m_eMoveDir = MOVEDIR::MD_LT;
}

void CMoveObj::MoveLeftBottom(float yourspeed, float fDeltaTime)
{
	m_tInfo.fX -= fDeltaTime * yourspeed / sqrtf(2.f);
	m_tInfo.fY += fDeltaTime * yourspeed / sqrtf(2.f);
	Equalize_HitPosInfoPos();
	m_eMoveDir = MOVEDIR::MD_LB;
}

void CMoveObj::MoveRightTop(float yourspeed, float fDeltaTime)
{
	m_tInfo.fX += fDeltaTime * yourspeed / sqrtf(2.f);
	m_tInfo.fY -= fDeltaTime * yourspeed / sqrtf(2.f);
	Equalize_HitPosInfoPos();
	m_eMoveDir = MOVEDIR::MD_RT;
}

void CMoveObj::MoveRightBottom(float yourspeed, float fDeltaTime)
{
	m_tInfo.fX += fDeltaTime * yourspeed / sqrtf(2.f);
	m_tInfo.fY += fDeltaTime * yourspeed / sqrtf(2.f);
	Equalize_HitPosInfoPos();
	m_eMoveDir = MOVEDIR::MD_RB;
}
