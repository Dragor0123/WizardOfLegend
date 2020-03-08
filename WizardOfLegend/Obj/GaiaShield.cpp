#include "../stdafx.h"
#include "GaiaShield.h"
#include "../MyBitmap/BmpMgr.h"
#include "../Manager/SoundMgr.h"

CGaiaShield::CGaiaShield()
{
}

CGaiaShield::~CGaiaShield()
{
	Release();
}

bool CGaiaShield::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Skill/GaiaShield.bmp", "GaiaShield"))
		return false;
	CShield::Initialize();


	m_fSpeed = 100.f;
	m_fDis = 160.f;
	m_tInfo.iCX = 100;
	m_tInfo.iCY = 100;

	m_tHitInfo.iCX = 80;
	m_tHitInfo.iCY = 80;

	if (m_strFrameKey == "")
		m_strFrameKey = "GaiaShield";

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 11;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 140;
	m_tFrame.dwFrameTime = GetTickCount();

	m_fLifeTLimit = 4.0f;
	return true;
}

int CGaiaShield::Update(float _fdTime)
{
	int iRes = CShield::Update(_fdTime);

	Move_Frame();
	Scene_Change();
	Change_HitRect();
	Update_Rect();
	Update_HitRect();

	return iRes;
}

void CGaiaShield::Late_Update(float _fdTime)
{
	CShield::Late_Update(_fdTime);
	if (true == m_bDead)
	{
		STOP_SOUND(CSoundMgr::EFFECT);
		PLAY_SOUND(L"GAIA_ARMOR_END.wav", CSoundMgr::EFFECT);
	}

	Update_Rect();
	Update_HitRect();
}

void CGaiaShield::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);
	GdiTransparentBlt(_DC
		, (int)(m_tRect.left + _fScrollX)
		, (int)(m_tRect.top + _fScrollY)
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * m_tInfo.iCX, 0
		, m_tInfo.iCX, m_tInfo.iCY
		, MAGENTA_COLOR);

	//히트박스 조건 걸자
	Draw_HitBox(_DC, _fScrollX, _fScrollY);
}

void CGaiaShield::Release()
{
}

void CGaiaShield::Change_HitRect()
{
}

void CGaiaShield::Move_Frame()
{
	if ((m_fAngle >= 0.f && m_fAngle < 15.f) || (m_fAngle >= 345.f && m_fAngle < 360.f))
	{
		m_tFrame.iFrameStart = 3;
	}
	else if (m_fAngle >= 15.f && m_fAngle < 45.f)
	{
		m_tFrame.iFrameStart = 4;
	}
	else if (m_fAngle >= 45.f && m_fAngle < 75.f)
	{
		m_tFrame.iFrameStart = 5;
	}
	else if (m_fAngle >= 75.f && m_fAngle < 105.f)
	{
		m_tFrame.iFrameStart = 6;
	}
	else if (m_fAngle >= 105.f && m_fAngle < 135.f)
	{
		m_tFrame.iFrameStart = 7;
	}
	else if (m_fAngle >= 135.f && m_fAngle < 165.f)
	{
		m_tFrame.iFrameStart = 8;
	}
	else if (m_fAngle >= 165.f && m_fAngle < 195.f)
	{
		m_tFrame.iFrameStart = 9;
	}
	else if (m_fAngle >= 195.f && m_fAngle < 225.f)
	{
		m_tFrame.iFrameStart = 10;
	}
	else if (m_fAngle >= 225.f && m_fAngle < 255.f)
	{
		m_tFrame.iFrameStart = 11;
	}
	else if (m_fAngle >= 255.f && m_fAngle < 285.f)
	{
		m_tFrame.iFrameStart = 0;
	}
	else if (m_fAngle >= 285.f && m_fAngle < 315.f)
	{
		m_tFrame.iFrameStart = 1;
	}
	else if (m_fAngle >= 315.f && m_fAngle < 345.f)
	{
		m_tFrame.iFrameStart = 2;
	}
}

void CGaiaShield::Scene_Change()
{
}
