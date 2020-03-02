#include "../stdafx.h"
#include "Sandbag.h"
#include "../MyBitmap/BmpMgr.h"

CSandbag::CSandbag()
{
}


CSandbag::~CSandbag()
{
}

bool CSandbag::Initialize()
{
	CMoveObj::Initialize();
	m_tInfo.iCX = 80;
	m_tInfo.iCY = 116;
	Equalize_HitPosInfoPos();
	Equalize_HitSzInfoSz();
	m_fSpeed = 0.f;

	m_iMaxHp = std::numeric_limits<int>::max();
	m_iHp = m_iMaxHp;

	Update_Rect();
	Update_HitRect();

	return true;
}

int CSandbag::Update(float _fdTime)
{
	return CMoveObj::Update(_fdTime);
}

void CSandbag::Late_Update(float _fdTime)
{
	CMonster::Late_Update(_fdTime);

	if (m_eMez != MEZ::MZ_FROZEN)
	{
		m_tHitInfo.iCX = 80;
		m_tHitInfo.iCY = 116;
	}
}

void CSandbag::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	//HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image("Zeal2");

	//GdiTransparentBlt(_DC,
	//	(int)(m_tRect.left + _fScrollX),
	//	(int)(m_tRect.top + _fScrollY),
	//	m_tInfo.iCX, m_tInfo.iCY,
	//	hMemDC,
	//	0,
	//	0,
	//	m_tInfo.iCX, m_tInfo.iCY,
	//	MAGENTA_COLOR);
	Rectangle(_DC,
		(int)(m_tHitRect.left + _fScrollX),
		(int)(m_tHitRect.top + _fScrollY),
		(int)(m_tHitRect.right + _fScrollX),
		(int)(m_tHitRect.bottom + _fScrollY)
	);

	Draw_HitBox(_DC, _fScrollX, _fScrollY);
}

void CSandbag::Release()
{
}

void CSandbag::Scene_Change()
{
}

void CSandbag::Change_HitRect()
{
}

CObj * CSandbag::Create_Bullet(const string & _frameKey)
{
	return nullptr;
}

void CSandbag::Move_Frame()
{
}
