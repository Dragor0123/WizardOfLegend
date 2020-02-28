#include "../stdafx.h"
#include "LineBullet.h"
#include "../MyBitmap/BmpMgr.h"

CLineBullet::CLineBullet()
{
}


CLineBullet::~CLineBullet()
{
	Release();
}

bool CLineBullet::Initialize()
{
/*
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Skill/dragon_arc116.bmp", "DragonArc"))
		return false;*/

	CBullet::Initialize();

	m_fSpeed = 750.f;
	m_tInfo.iCX = 30;
	m_tInfo.iCY = 30;
	Equalize_HitPosInfoPos();
	Equalize_HitSzInfoSz();

	// if (m_strFrameKey == "")
	// 	m_strFrameKey = string("DragonArc");

	m_ePreState = END;
	m_eCurState = FIRE;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();

	return true;
}

int CLineBullet::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_tInfo.fX += cosf(Degree_To_Radian(m_fAngle)) * m_fSpeed * _fdTime;
	m_tInfo.fY -= sinf(Degree_To_Radian(m_fAngle)) * m_fSpeed * _fdTime;

	Equalize_HitPosInfoPos();

	Move_Frame();
	Scene_Change();
	Change_HitRect();
	Update_Rect();
	Update_HitRect();
	return OBJ_NOEVENT;
}

void CLineBullet::Late_Update(float _fdTime)
{
	CBullet::Late_Update(_fdTime);
}

void CLineBullet::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	Ellipse(_DC,
		(int)(m_tHitRect.left + _fScrollX),
		(int)(m_tHitRect.top + _fScrollY),
		(int)(m_tHitRect.right + _fScrollX),
		(int)(m_tHitRect.bottom + _fScrollY));

	Draw_HitBox(_DC, _fScrollX, _fScrollY);
}

void CLineBullet::Release()
{
}

void CLineBullet::Move_Frame()
{
	CBullet::Move_Frame();
}

void CLineBullet::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CBullet::FIRE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CBullet::COLLISION:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CLineBullet::Change_HitRect()
{
}
