#include "../stdafx.h"
#include "Bullet.h"


CBullet::CBullet()
	: m_iAtt(1), m_fShotRange(700.f), m_bCollision(false), m_bMonsters(false)
{
	ZeroMemory(m_tRotPoint, sizeof(m_tRotPoint));
}


CBullet::~CBullet()
{
	Release();
}

bool CBullet::Initialize()
{
	CMoveObj::Initialize();
	m_tStartPt.fX = m_tInfo.fX;
	m_tStartPt.fY = m_tInfo.fY;
	return true;
}

int CBullet::Update(float _fdTime)
{
	if (OBJ_DEAD == CMoveObj::Update(_fdTime))
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CBullet::Late_Update(float _fdTime)
{
	if (m_bCollision) {
		m_eCurState = CBullet::COLLISION;
		m_tHitInfo.iCX = 0;
		m_tHitInfo.iCY = 0;
	}

	float fdX = 0.f, fdY = 0.f;
	fdX = m_tInfo.fX - m_tStartPt.fX;
	fdY = m_tInfo.fY - m_tStartPt.fY;

	float fdist = sqrtf(fdX * fdX + fdY * fdY);

	if (fdist > m_fShotRange) {
		m_eCurState = CBullet::COLLISION;
		m_tHitInfo.iCX = 0;
		m_tHitInfo.iCY = 0;
	}
}

void CBullet::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
}

void CBullet::Release()
{
}

void CBullet::Move_Frame()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();

		if (m_ePreState == CBullet::FIRE) {
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_tFrame.iFrameStart = 0;
		}
		else if (m_ePreState == CBullet::COLLISION) {
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_bDead = true;
		}
	}
}
