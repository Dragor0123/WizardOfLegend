#include "../stdafx.h"
#include "Gold.h"
#include "../MyBitmap/BmpMgr.h"
#include "ObjMgr.h"

CGold::CGold()
	: m_iGold(0)
{
}

CGold::~CGold()
{
}

bool CGold::Initialize()
{
	m_iGold = rand() % 9 + 3;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Item/Gold.bmp", "Gold"))
		return false;
	if (CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).empty())
		return false;

	m_pTarget = CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).front();
	m_tInfo.iCX = 16;
	m_tInfo.iCY = 16;
	m_fSpeed = 650.f;
	m_strFrameKey = "Gold";

	Equalize_HitPosInfoPos();
	Equalize_HitSzInfoSz();

	return true;
}

int CGold::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	float fX = 0.f, fY = 0.f, fDis = 0.f, fRadian = 0.f;
	fX = m_pTarget->Get_Info().fX - m_tInfo.fX;
	fY = m_pTarget->Get_Info().fY - m_tInfo.fY;
	fDis = sqrtf(fX * fX + fY * fY);
	fRadian = acosf(fX / fDis);

	if (m_tInfo.fY < m_pTarget->Get_Info().fY)
		fRadian = 2 * PI - fRadian;
	m_fAngle = fRadian * 180.f / PI;

	MoveAngle(_fdTime);

	Update_Rect();
	Update_HitRect();

	return OBJ_NOEVENT;
}

void CGold::Late_Update(float _fdTime)
{
}

void CGold::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);

	GdiTransparentBlt(_DC
		, (int)(m_tRect.left + _fScrollX)
		, (int)(m_tRect.top + _fScrollY)
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, 0
		, 0
		, m_tInfo.iCX, m_tInfo.iCY
		, MAGENTA_COLOR);

	Draw_HitBox(_DC, _fScrollX, _fScrollY);
}

void CGold::Release()
{
}
