#include "../stdafx.h"
#include "PlayerHPBar.h"
#include "../MyBitmap/BmpMgr.h"
#include "ObjMgr.h"
#include "Player.h"

CPlayerHPBar::CPlayerHPBar()
	: m_iManaBarCX(192), m_iManaBarCY(16)
{
	ZeroMemory(&m_tUserHPRC, sizeof(m_tUserHPRC));
	ZeroMemory(&m_tUserMPRC, sizeof(m_tUserMPRC));
}

CPlayerHPBar::~CPlayerHPBar()
{
	Release();
}

bool CPlayerHPBar::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/UI/UI_PLAYERBAR.bmp", "PlayerBar"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/UI/UI_HPBAR.bmp", "P_HPBar"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/UI/UI_MANABAR.bmp", "P_MPBar"))
		return false;
	if (!m_pTarget)
		return false;
	// 20, 16-> offset
	m_tInfo.iCX = 328;
	m_tInfo.iCY = 80;
	//m_tInfo.fX = 164.f + 20.f;
	//m_tInfo.fY = 40.f + 16.f;
	m_tInfo.fX = 780.f + 20.f;
	m_tInfo.fY = 644.f + 16.f;

	m_tUserHPRC = { 692 + 20, 616 + 16, 936 + 20, 648 + 16};
	m_tHitInfo.iCX = 244;
	m_tHitInfo.iCY = 32;

	m_tUserMPRC = { 692 + 20, 656 + 16, 884 + 20, 672 + 16};
	m_strFrameKey = "PlayerBar";

	Update_Rect();
	m_eRenderGroupID = GROUPID::UI;
	return true;
}

int CPlayerHPBar::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CPlayerHPBar::Late_Update(float _fdTime)
{
}

void CPlayerHPBar::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);
	HDC hHPMemDC = CBmpMgr::Get_Instance()->Find_Image("P_HPBar");
	HDC hMPMemDC = CBmpMgr::Get_Instance()->Find_Image("P_MPBar");

	float fHpRatio = static_cast<CPlayer*>(m_pTarget)->Get_Cur_HP_Ratio();
	float fMpRatio = static_cast<CPlayer*>(m_pTarget)->Get_Cur_MP_Ratio();
	int iHpRight = (int)(m_tHitInfo.iCX * fHpRatio);
	int iMpRight = (int)(m_iManaBarCX * fMpRatio);

	GdiTransparentBlt(_DC
		, (int)(m_tRect.left)
		, (int)(m_tRect.top)
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, 0
		, 0
		, m_tInfo.iCX, m_tInfo.iCY
		, MAGENTA_COLOR);

	BitBlt(_DC,
		(int)(m_tUserHPRC.left),
		(int)(m_tUserHPRC.top),
		iHpRight, m_tHitInfo.iCY,
		hHPMemDC,
		0, 0,
		SRCCOPY);

	BitBlt(_DC,
		(int)(m_tUserMPRC.left),
		(int)(m_tUserMPRC.top),
		iMpRight, m_iManaBarCY,
		hMPMemDC,
		0, 0,
		SRCCOPY);


	// 수정 폰트 크기 크게 키울것.
	TCHAR szBuff[32] = L"";
	swprintf_s(szBuff, 32, L" %3d / %3d", 
		static_cast<CPlayer*>(m_pTarget)->Get_Hp(), static_cast<CPlayer*>(m_pTarget)->Get_MaxHp());
	COLORREF oldFontColor = SetTextColor(_DC, RGB(255, 255, 255));
	SetBkMode(_DC, TRANSPARENT);
	TextOut(_DC, 852, 614, szBuff, lstrlen(szBuff));
	SetTextColor(_DC, oldFontColor);
}

void CPlayerHPBar::Release()
{
	CBmpMgr::Get_Instance()->Delete_Bmp("PlayerBar");
	CBmpMgr::Get_Instance()->Delete_Bmp("P_HPBar");
	CBmpMgr::Get_Instance()->Delete_Bmp("P_MPBar");
}

void CPlayerHPBar::Key_Check(float _fdTime)
{
}


/*

HBRUSH  oldBrush;
if (m_Target) {
wsprintf(szBuff, L"%ls[%d] HP ", dynamic_cast<CMonster*>(m_Target)->Get_Name(), dynamic_cast<CMonster*>(m_Target)->Get_MonsterID());
TextOut(_DC, 108, 18, szBuff, lstrlen(szBuff));
Rectangle(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

oldBrush = (HBRUSH)SelectObject(_DC, m_EnemyHpBrush);
int iHpRight = m_tRect.left + int(m_tInfo.iCX * m_fRate);
Rectangle(_DC, m_tRect.left, m_tRect.top, iHpRight, m_tRect.bottom);
SelectObject(_DC, oldBrush);
}
*/