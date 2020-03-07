#include "../stdafx.h"
#include "UISkillSet.h"
#include "../MyBitmap/BmpMgr.h"
#include "ObjMgr.h"
#include "../Manager/CardMgr.h"
#include "Inventory.h"
#include "ArcRel.h"

CUISkillSet::CUISkillSet()
{
	ZeroMemory(m_bCoolingArr, sizeof(m_bCoolingArr));
}

CUISkillSet::~CUISkillSet()
{
	Release();
}

bool CUISkillSet::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/UI/UI_SKILLSET.bmp", "UI_SKILLSET"))
		return false;

	if (CObjMgr::Get_Instance()->Get_listObj(OBJID::INVENTORY).empty())
		return false;
	
	CObj* pInventory = CObjMgr::Get_Instance()->Get_listObj(OBJID::INVENTORY).front();
	m_pInvenOutter = &static_cast<CInventory*>(pInventory)->m_aOutterArray;

	m_tInfo.iCX = 411;
	m_tInfo.iCY = 81;
	m_tInfo.fX = 205.f + 32.f;
	m_tInfo.fY = (float)WINCY - 40.5f - 16.f;

	m_strFrameKey = "UI_SKILLSET";
	Update_Rect();
	m_eRenderGroupID = GROUPID::UI;
	return true;
}

int CUISkillSet::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;
	// 쿨타임 받아오는게 업데이터 할 거임!
	return OBJ_NOEVENT;
}

void CUISkillSet::Late_Update(float _fdTime)
{

}

void CUISkillSet::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);

	GdiTransparentBlt(_DC
		, (int)(m_tRect.left)
		, (int)(m_tRect.top)
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, 0
		, 0
		, m_tInfo.iCX, m_tInfo.iCY
		, MAGENTA_COLOR);

	Render_Skill_Sets(_DC, _fdTime, _fScrollX, _fScrollY);
}

void CUISkillSet::Release()
{
}

void CUISkillSet::Render_Skill_Sets(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	HDC hMemDC_35;
	HFONT hMyFont, hOldFont;
	COLORREF oldFontColor = NULL;
	TCHAR szBuff[16] = L"";

	hMyFont = CreateFont(20, 0, 0, 0, FW_SEMIBOLD, 0, 0, 0, ANSI_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("Consolas"));

	for (size_t i = 0; i < m_pInvenOutter->size(); ++i)
	{
		int _iCardCode = (*m_pInvenOutter)[i];
		if (_iCardCode != -1)
		{
			hMemDC_35 = CCardMgr::Get_Instance()->Find_Image35(_iCardCode);
			float fX = float(m_tRect.left + 22.f + i * 60.f);
			float fY = m_tRect.top + 52.f;

			GdiTransparentBlt(_DC,
				(int)(fX - (35 >> 1)), (int)(fY - (35 >> 1)),
				35, 35,
				hMemDC_35,
				0, 0,
				35, 35,
				MAGENTA_COLOR);

			hOldFont = (HFONT)SelectObject(_DC, hMyFont);
			oldFontColor = SetTextColor(_DC, RGB(255, 255, 255));
			SetBkMode(_DC, TRANSPARENT);
			if (true == m_bCoolingArr[i])
			{
				CArcRel* pArc = CCardMgr::Get_Instance()->Find_ArcRel(_iCardCode);
				swprintf_s(szBuff, 16, L"%.1f", pArc->Get_Remained_Cool());
				TextOut(_DC, 
					(int)(fX - 15.f),
					(int)(fY + 19.f), 
					szBuff, lstrlen(szBuff));
			}
		}
	}

	SelectObject(_DC, hOldFont);
	DeleteObject(hMyFont);
	SetTextColor(_DC, oldFontColor);
	SetBkMode(_DC, OPAQUE);
}

void CUISkillSet::Key_Check(float _fdTime)
{
}