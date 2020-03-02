#include "../stdafx.h"
#include "Digit.h"
#include "../MyBitmap/BmpMgr.h"

const float CDigit::fLIFETIME_LIMIT = 0.85f;

CDigit::CDigit()
	:m_iNumber(0), m_fLifeTime(0.f), m_bMoving(true), m_bUsingLifeTime(true)
{
}

CDigit::~CDigit()
{
	Release();
}

bool CDigit::Initialize()
{
	m_tInfo.iCX = 24;
	m_tInfo.iCY = 24;

	if (m_strFrameKey == "")
		m_strFrameKey = "digit_white";

	m_fSpeed = 200.f;
	m_eRenderGroupID = GROUPID::UI;
	// iTemp = -1;
	Update_Rect();
	return true;
}

int CDigit::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_bUsingLifeTime)
		m_fLifeTime += _fdTime;
	if (m_bMoving)
		MoveYFromSpeed(_fdTime, MOVEDIR::MD_BACK);

	Update_Rect();
	return OBJ_NOEVENT;
}

void CDigit::Late_Update(float _fdTime)
{
	if (m_fLifeTime > fLIFETIME_LIMIT)
		m_bDead = true;
}

void CDigit::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);
	GdiTransparentBlt(_DC
		, (int)(m_tRect.left + _fScrollX)
		, (int)(m_tRect.top + _fScrollY)
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_iNumber * m_tInfo.iCX, 0
		, m_tInfo.iCX, m_tInfo.iCY
		, MAGENTA_COLOR);
}

void CDigit::Release()
{
}

void CDigit::Key_Check(float _fdTime)
{
}
