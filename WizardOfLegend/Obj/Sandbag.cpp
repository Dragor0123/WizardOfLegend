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
	m_tInfo.iCX = 97;
	m_tInfo.iCY = 86;
	Equalize_HitPosInfoPos();
	Equalize_HitSzInfoSz();
	m_fSpeed = 0.f;
	return true;
}

int CSandbag::Update(float _fdTime)
{
	if (OBJ_DEAD == CMoveObj::Update(_fdTime))
		return OBJ_DEAD;
	// 특정 조건 때문에 OBJ_DEAD일경우 그걸 리턴

	Update_Rect();
	Update_HitRect();
	return OBJ_NOEVENT;
}

void CSandbag::Late_Update(float _fdTime)
{
	Update_Rect();
	Update_HitRect();
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
