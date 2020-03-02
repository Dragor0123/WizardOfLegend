#include "../stdafx.h"
#include "BossHPBar.h"
#include "../MyBitmap/BmpMgr.h"
#include "ObjMgr.h"
#include "Monster.h"

//	m_strFrameKey = "FireBoss";
// 	m_strFrameKey = "EarthBoss";

CBossHPBar::CBossHPBar()
	: m_fDeadTime(0.f)
{
	ZeroMemory(&m_tBossHPRC, sizeof(m_tBossHPRC));
}


CBossHPBar::~CBossHPBar()
{
	Release();
}

bool CBossHPBar::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/UI/FIREBOSS_NAMEBAR.bmp", "Zeal_NameBar"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/UI/EARTHBOSS_NAMEBAR.bmp", "Atalas_NameBar"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/UI/BOSS_HPBAR.bmp", "Boss_HPBar"))
		return false;
	if (!m_pTarget)
		return false;

	m_tInfo.iCX = 452;
	m_tInfo.iCY = 90;
	m_tInfo.fX = (float)(WINCX >> 1);
	m_tInfo.fY = 44.f + 20.f;
	Update_Rect();

	m_tHitInfo.iCX = 356;
	m_tHitInfo.iCY = 28;
	m_tBossHPRC = { m_tRect.left + 48, m_tRect.top + 41,m_tRect.left + 48 + 356, m_tRect.top + 41 + 258};


	if ("FireBoss" == m_pTarget->Get_FrameKey())
		m_strFrameKey = "Zeal_NameBar";
	else if ("EarthBoss" == m_pTarget->Get_FrameKey())
		m_strFrameKey = "Atalas_NameBar";

	m_eRenderGroupID = GROUPID::UI;
	return true;
}

int CBossHPBar::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_pTarget->Get_Dead())
		m_fDeadTime += _fdTime;

	return OBJ_NOEVENT;
}

void CBossHPBar::Late_Update(float _fdTime)
{
	if (m_fDeadTime > 0.165f)
		m_bDead = true;
}

void CBossHPBar::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);
	HDC hHPMemDC = CBmpMgr::Get_Instance()->Find_Image("Boss_HPBar");
	
	float fHpRatio = 1.f;
	if (!m_pTarget->Get_Dead())
		fHpRatio = static_cast<CMonster*>(m_pTarget)->Get_Cur_HP_Ratio();
	else
		fHpRatio = 0.f;
	int iHpRight = (int)(m_tHitInfo.iCX * fHpRatio);

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
		(int)(m_tBossHPRC.left),
		(int)(m_tBossHPRC.top),
		iHpRight, m_tHitInfo.iCY,
		hHPMemDC,
		0, 0,
		SRCCOPY);
}

void CBossHPBar::Release()
{
	CBmpMgr::Get_Instance()->Delete_Bmp("Zeal_NameBar");
	CBmpMgr::Get_Instance()->Delete_Bmp("Atalas_NameBar");
	CBmpMgr::Get_Instance()->Delete_Bmp("Boss_HPBar");
}

void CBossHPBar::Key_Check(float _fdTime)
{

}
