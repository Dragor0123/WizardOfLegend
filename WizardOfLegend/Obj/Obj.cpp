#include "../stdafx.h"
#include "Obj.h"
#include "../Manager/ScrollMgr.h"

CObj::CObj()
	: m_pTarget(nullptr), m_fAngle(0.f), m_bDead(false), m_eID(OBJID::END),
	m_eRenderGroupID(GROUPID::GAMEOBJECT)
{
	ZeroMemory(&m_tInfo, sizeof(m_tInfo));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tFrame, sizeof(m_tFrame));
	ZeroMemory(&m_tHitInfo, sizeof(m_tHitInfo));
	ZeroMemory(&m_tHitRect, sizeof(m_tHitRect));
}

CObj::~CObj()
{
}

void CObj::Scene_Change()
{
}

void CObj::Move_Frame()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart = 0;
	}
}

void CObj::Update_Rect()
{
	m_tRect.left = LONG(m_tInfo.fX - (m_tInfo.iCX >> 1));
	m_tRect.top = LONG(m_tInfo.fY - (m_tInfo.iCY >> 1));
	m_tRect.right = LONG(m_tInfo.fX + (m_tInfo.iCX >> 1));
	m_tRect.bottom = LONG(m_tInfo.fY + (m_tInfo.iCY >> 1));
}

void CObj::Update_HitRect()
{
	//float fX = CScrollMgr::Get_Instance()->Get_ScrollX();
	//float fY = CScrollMgr::Get_Instance()->Get_ScrollY();
	m_tHitRect.left = LONG(m_tHitInfo.fX - (m_tHitInfo.iCX >> 1));
	m_tHitRect.top = LONG(m_tHitInfo.fY - (m_tHitInfo.iCY >> 1));
	m_tHitRect.right = LONG(m_tHitInfo.fX + (m_tHitInfo.iCX >> 1));
	m_tHitRect.bottom = LONG(m_tHitInfo.fY + (m_tHitInfo.iCY >> 1));
}

void CObj::Draw_HitBox(HDC _DC, float _fScrollX, float _fScrollY)
{
	if (g_HitBox_On)
	{
		MoveToEx(_DC, (int)(m_tHitRect.left + _fScrollX), (int)(m_tHitRect.top + _fScrollY), NULL);
		LineTo(_DC, (int)(m_tHitRect.left + _fScrollX), (int)(m_tHitRect.bottom + _fScrollY));
		LineTo(_DC, (int)(m_tHitRect.right + _fScrollX), (int)(m_tHitRect.bottom + _fScrollY));
		LineTo(_DC, (int)(m_tHitRect.right + _fScrollX), (int)(m_tHitRect.top + _fScrollY));
		LineTo(_DC, (int)(m_tHitRect.left + _fScrollX), (int)(m_tHitRect.top + _fScrollY));
	}
}

void CObj::Set_Pos(float _x, float _y)
{
	m_tInfo.fX = _x;
	m_tInfo.fY = _y;
	Equalize_HitPosInfoPos();
}
