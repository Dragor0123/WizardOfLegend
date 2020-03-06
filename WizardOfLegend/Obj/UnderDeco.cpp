#include "../stdafx.h"
#include "UnderDeco.h"
#include "../MyBitmap/BmpMgr.h"
#include "Meteor.h"

namespace UnderDeco_Space
{
	const DWORD CREVIS_FRAMESPEED = 120;
	const float	CASTINGCIRCLE_SPEED = 140.f;
}

using namespace UnderDeco_Space;

CUnderDeco::CUnderDeco()
	: m_iDrawID(0)
{
	ZeroMemory(&m_tRotPoint, sizeof(m_tRotPoint));
}


CUnderDeco::~CUnderDeco()
{
	Release();
}

bool CUnderDeco::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/EarthBoss/EarthCrevis.bmp", "Crevis"))
		return false;

	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Skill/CastingCircle.bmp", "CastingCircle"))
		return false;

	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Skill/rot400.bmp", "rot400"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Skill/empty400.bmp", "empty400"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/DecoEffect/Carpet_0.bmp", "Carpet_0"))
		return false;

	if (m_strFrameKey == "")
		return false;

	if (m_strFrameKey == "Crevis")
	{
		m_tInfo.iCX = 100;
		m_tInfo.iCY = 80;
		Equalize_HitPosInfoPos();
		m_tHitInfo.iCX = 0;
		m_tHitInfo.iCY = 0;

		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 4;
		m_tFrame.iFrameScene = 0;
		m_tFrame.dwFrameSpeed = CREVIS_FRAMESPEED;
	}
	else if (m_strFrameKey == "CastingCircle")
	{
		m_tInfo.iCX = 400;
		m_tInfo.iCY = 400;
		Equalize_HitPosInfoPos();
		m_tHitInfo.iCX = 0;
		m_tHitInfo.iCY = 0;

		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 0;
		m_tFrame.iFrameScene = 0;
		m_tFrame.dwFrameSpeed = 100;
		m_fAngle = 0.f;
	}
	else if (m_strFrameKey == "Carpet_0")
	{
		m_tInfo.iCX = 480;
		m_tInfo.iCY = 162;
		Equalize_HitPosInfoPos();
		m_tHitInfo.iCX = 0;
		m_tHitInfo.iCY = 0;

		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 0;
		m_tFrame.iFrameScene = 0;
		m_tFrame.dwFrameSpeed = 100;
	}

	m_tFrame.dwFrameTime = GetTickCount();
	m_eRenderGroupID = GROUPID::BACKGROUND;

	return true;
}

void CUnderDeco::Key_Check(float _fdTime)
{
}

int CUnderDeco::Update(float _fdTime)
{
	// crevis 사라질때 alpha blending 써서 페이드아웃??
	if (m_bDead)
		return OBJ_DEAD;

	if (m_strFrameKey == "CastingCircle")
		m_fAngle += CASTINGCIRCLE_SPEED * _fdTime;

	Move_Frame();
	Scene_Change();
	Update_Rect();
	return OBJ_NOEVENT;
}

void CUnderDeco::Late_Update(float _fdTime)
{
	Update_Rect();
	if (m_strFrameKey == "CastingCircle")
	{
		if (m_iDrawID == 1)
		{
			if (m_fAngle >= 270.f || !m_pTarget || m_pTarget->Get_Dead())
				m_bDead = true;
		}


		float	fDis = sqrtf(float((m_tInfo.iCX >> 1) * (m_tInfo.iCX >> 1) + (m_tInfo.iCY >> 1) * (m_tInfo.iCY >> 1)));
		m_tRotPoint[0].x = LONG((m_tInfo.iCX >> 1) + (cosf((m_fAngle + 135.f) * PI / 180.f) * fDis));
		m_tRotPoint[0].y = LONG((m_tInfo.iCY >> 1) - (sinf((m_fAngle + 135.f) * PI / 180.f) * fDis));
		m_tRotPoint[1].x = LONG((m_tInfo.iCX >> 1) + (cosf((m_fAngle + 45.f) * PI / 180.f) * fDis));
		m_tRotPoint[1].y = LONG((m_tInfo.iCY >> 1) - (sinf((m_fAngle + 45.f) * PI / 180.f) * fDis));
		m_tRotPoint[2].x = LONG((m_tInfo.iCX >> 1) + (cosf((m_fAngle + 225.f) * PI / 180.f) * fDis));
		m_tRotPoint[2].y = LONG((m_tInfo.iCY >> 1) - (sinf((m_fAngle + 225.f) * PI / 180.f) * fDis));
	}
}

void CUnderDeco::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);

	if (m_strFrameKey == "Crevis")
	{
		GdiTransparentBlt(_DC,
			(int)(m_tRect.left + _fScrollX),
			(int)(m_tRect.top + _fScrollY),
			m_tInfo.iCX, m_tInfo.iCY,
			hMemDC,
			m_tFrame.iFrameStart * m_tInfo.iCX,
			0,
			m_tInfo.iCX, m_tInfo.iCY,
			MAGENTA_COLOR);
	}
	else if (m_strFrameKey == "CastingCircle")
	{
		HDC hRotDC = CBmpMgr::Get_Instance()->Find_Image("rot400");
		HDC hEmpDC = CBmpMgr::Get_Instance()->Find_Image("empty400");
		
		PlgBlt(hRotDC, m_tRotPoint,
			hMemDC,
			m_iDrawID * m_tInfo.iCX,
			0,
			m_tInfo.iCX, m_tInfo.iCY,
			NULL, NULL, NULL);

		GdiTransparentBlt(_DC,
			(int)(m_tRect.left + _fScrollX),
			(int)(m_tRect.top + _fScrollY),
			m_tInfo.iCX, m_tInfo.iCY,
			hRotDC,
			0, 0,
			m_tInfo.iCX, m_tInfo.iCY,
			MAGENTA_COLOR);

		BitBlt(hRotDC, 0, 0, m_tInfo.iCX, m_tInfo.iCY, hEmpDC, 0, 0, SRCCOPY);
	}
	else
	{
		GdiTransparentBlt(_DC,
			(int)(m_tRect.left + _fScrollX),
			(int)(m_tRect.top + _fScrollY),
			m_tInfo.iCX, m_tInfo.iCY,
			hMemDC,
			0,
			0,
			m_tInfo.iCX, m_tInfo.iCY,
			MAGENTA_COLOR);
	}
}

void CUnderDeco::Release()
{
}

void CUnderDeco::Move_Frame()
{
	if (m_strFrameKey == "CastingCircle" || m_strFrameKey == "Carpet_0")
		return;
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();
		if (m_strFrameKey == "Crevis")
		{
			m_tFrame.dwFrameSpeed = CREVIS_FRAMESPEED;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_bDead = true;
		}
	}
}

void CUnderDeco::Scene_Change()
{
	if (m_strFrameKey == "Crevis")
		return;
	if (m_strFrameKey == "CastingCircle")
		return;
	if (m_strFrameKey == "Carpet_0")
		return;
}
