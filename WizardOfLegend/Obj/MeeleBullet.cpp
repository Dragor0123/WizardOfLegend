#include "../stdafx.h"
#include "MeeleBullet.h"
#include "../MyBitmap/BmpMgr.h"

CMeeleBullet::CMeeleBullet()
{
}


CMeeleBullet::~CMeeleBullet()
{
	Release();
}

bool CMeeleBullet::Initialize()
{

	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/Swordman/Swordman_Attack.bmp", "SwordmanAtt"))
		return false;

	m_fSpeed = 0.f;

	Equalize_HitPosInfoPos();

	if (m_strFrameKey == "")
	{
		m_tInfo.iCX = 30;
		m_tInfo.iCY = 30;
		Equalize_HitSzInfoSz();
	}
	else if (m_strFrameKey == "SwordmanAtt")
	{
		m_tInfo.iCX = 160;
		m_tInfo.iCY = 160;
		m_tHitInfo.iCX = 128;
		m_tHitInfo.iCY = 137;
		m_iAtt = 20;
	}

	// 쓸모 없음.
	m_ePreState = m_eCurState = CBullet::END;

	m_ePreDir = END;
	m_eCurDir = LEFT;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 80;
	m_tFrame.dwFrameTime = GetTickCount();
	return true;
}

int CMeeleBullet::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	Move_Frame();
	Scene_Change();
	Change_HitRect();
	Update_Rect();
	Update_HitRect();
	return OBJ_NOEVENT;
}

void CMeeleBullet::Late_Update(float _fdTime)
{
	Update_Rect();
	Update_HitRect();
}

void CMeeleBullet::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	if (m_strFrameKey == "")
	{

		HPEN hOldPen, hMyPen;
		HBRUSH hOldBrush;
		hMyPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
		hOldPen = (HPEN)SelectObject(_DC, hMyPen);
		hOldBrush = (HBRUSH)SelectObject(_DC, GetStockObject(NULL_BRUSH));

		Rectangle(_DC,
			(int)(m_tHitRect.left + _fScrollX),
			(int)(m_tHitRect.top + _fScrollY),
			(int)(m_tHitRect.right + _fScrollX),
			(int)(m_tHitRect.bottom + _fScrollY)
			);

		SelectObject(_DC, hOldPen);
		SelectObject(_DC, hOldBrush);
		DeleteObject(hMyPen);
	}
	else {
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);

		GdiTransparentBlt(_DC,
			(int)(m_tRect.left + _fScrollX),
			(int)(m_tRect.top + _fScrollY),
			m_tInfo.iCX, m_tInfo.iCY,
			hMemDC,
			m_tFrame.iFrameStart * m_tInfo.iCX,
			m_tFrame.iFrameScene * m_tInfo.iCY,
			m_tInfo.iCX, m_tInfo.iCY,
			MAGENTA_COLOR);

		Draw_HitBox(_DC, _fScrollX, _fScrollY);
	}
}

void CMeeleBullet::Release()
{

}

void CMeeleBullet::Move_Frame()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();

		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			m_tHitInfo.iCX = m_tHitInfo.iCY = 0;

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_bDead = true;
	}
}

void CMeeleBullet::Scene_Change()
{
	if (m_ePreDir != m_eCurDir)
	{
		switch (m_eCurDir)
		{
		case CMeeleBullet::LEFT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 80;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CMeeleBullet::TOP:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameSpeed = 80;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CMeeleBullet::RIGHT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 2;
			m_tFrame.dwFrameSpeed = 80;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CMeeleBullet::BOTTOM:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 3;
			m_tFrame.dwFrameSpeed = 80;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		}
		m_ePreDir = m_eCurDir;
	}
}

void CMeeleBullet::Change_HitRect()
{
	switch (m_ePreDir)
	{
	case CMeeleBullet::LEFT:
		m_tHitInfo.iCX = 130;
		m_tHitInfo.iCY = 137;
		break;
	case CMeeleBullet::TOP:
		m_tHitInfo.iCX = 132;
		m_tHitInfo.iCY = 80;
		break;
	case CMeeleBullet::RIGHT:
		m_tHitInfo.iCX = 130;
		m_tHitInfo.iCY = 133;
		break;
	case CMeeleBullet::BOTTOM:
		m_tHitInfo.iCX = 136;
		m_tHitInfo.iCY = 80;
		break;
	}
}

int CMeeleBullet::Get_Collision_Code() const
{
	return CC_MBULLET_NWALL_PUSH;
}
