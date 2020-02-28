#include "../stdafx.h"
#include "Inventory.h"
#include "../MyBitmap/BmpMgr.h"
#include "../Manager/CtrlOwnerMgr.h"
#include "../Manager/KeyMgr.h"
#include "../Manager/ScrollMgr.h"
#include "../Manager/CardMgr.h"

CInventory::CInventory()
	: ROWSIZE(2), COLSIZE(6),
	INVEN_INARR_MAXNUM(ROWSIZE * COLSIZE),
	fInnerSlot_X_Offset(60.f), fInnerSlot_Y_Offset(244.f),
	fOutterSlot_X_Offset(60.f), fOutterSlot_Y_Offset(70.f),
	InnerSlotCX(46), InnerSlotCY(46),
	OutterSlotCX(35), OutterSlotCY(35),
	fXInnerGap(58.f), fYInnerGap(59.f),
	fXOuterGap(60.f), m_iMouseCnt(0),
	m_iSlotChangeCX(58), m_iSlotChangeCY(60)
{
	for (size_t i = 0; i < m_aOutterArray.size(); ++i)
		m_aOutterArray[i] = -1;
	for (size_t i = 0; i < (size_t)INVEN_INARR_MAXNUM; ++i)
		m_aInnerArray[i] = -1;
	ZeroMemory(&m_tInnerFirstPOS, sizeof(LINEPOS));
	ZeroMemory(&m_tOutterFirstPOS, sizeof(LINEPOS));
	//memset(m_iIdxInner, -1, sizeof(m_iIdxInner));
	//memset(m_iIdxOutter, -1, sizeof(m_iIdxOutter));
}

CInventory::~CInventory()
{
}

bool CInventory::Initialize()
{
	m_tInfo.fX = 228.f;
	m_tInfo.fY = 342;
	m_tInfo.iCX = 456;
	m_tInfo.iCY = 684;

	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/UI/Inventory_456_684.bmp", "Inven"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/UI/InslotMouse_all.bmp", "InSlotChange"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/UI/InSlotMouse_43_42.bmp", "OutSlotChange"))
		return false;

	m_iIdxInner = -1;
	m_iIdxOutter = -1;
	m_iRemberIdx = -1;

	m_iLastMouseOnIdx = -1;
	m_iClickedIdx = -1;

	m_strFrameKey = "Inven";
	Equalize_HitPosInfoPos();
	Equalize_HitSzInfoSz();
	Update_Rect();
	Update_HitRect();

	m_aOutterArray[1] = 4;
	return true;
}

void CInventory::Key_Check(float _fdTime)
{
	if (!CCtrlOwnerMgr::Get_Instance()->Is_Key_Available(KEYOWN::KS_INVENTORY))
		return;

	if (KEY_DOWN(VK_ESCAPE))
	{
		//
	}

	if (KEY_DOWN(VK_TAB))
	{
		Pull_InArrayItems();
		CCtrlOwnerMgr::Get_Instance()->Inventory_Off();
		m_iMouseCnt = 0;
		m_iClickedIdx = -1;
		m_iLastMouseOnIdx = -1;
	}

	//////  안쪽 사각형 배열 판정...
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	pt.x -= (LONG)(CScrollMgr::Get_Instance()->Get_ScrollX());
	pt.y -= (LONG)(CScrollMgr::Get_Instance()->Get_ScrollY());
	RECT rc = { m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom };
	rc.left -= (LONG)(CScrollMgr::Get_Instance()->Get_ScrollX());
	rc.top -= (LONG)(CScrollMgr::Get_Instance()->Get_ScrollY());
	rc.right -= (LONG)(CScrollMgr::Get_Instance()->Get_ScrollX());
	rc.bottom -= (LONG)(CScrollMgr::Get_Instance()->Get_ScrollY());
	////

	m_tInnerFirstPOS.fX = (float)rc.left + fInnerSlot_X_Offset + (InnerSlotCX / 2.f);
	m_tInnerFirstPOS.fY = (float)rc.top + fInnerSlot_Y_Offset + (InnerSlotCY / 2.f);

	m_tOutterFirstPOS.fX = (float)rc.left + fOutterSlot_X_Offset + (OutterSlotCX / 2.f);
	m_tOutterFirstPOS.fY = (float)rc.top + fOutterSlot_Y_Offset + (OutterSlotCY / 2.f);



	//m_iLastMouseOnIdx = -1;
	//m_iClickedIdx = -1;
	BOOL bPtInSlot = FALSE;
	if (bPtInSlot = PtInRect(&rc, pt))
	{
		// InnerArray 클릭 검사
		for (int i = 0; i < ROWSIZE; ++i)
		{
			for (int j = 0; j < COLSIZE; ++j)
			{
				float fX = m_tInnerFirstPOS.fX + j * fXInnerGap;
				float fY = m_tInnerFirstPOS.fY + i * fYInnerGap;
				RECT slot = { (int)fX - (InnerSlotCX >> 1),
					(int)fY - (InnerSlotCY >> 1),
					(int)fX + (InnerSlotCX >> 1),
					(int)fY + (InnerSlotCY >> 1) };

				if (PtInRect(&slot, pt))
				{
					if (KEY_DOWN(VK_LBUTTON))		// 누르면...
					{
						m_iIdxInner = i * COLSIZE + j;
						/*if (m_iMouseCnt == 0 && m_aInnerArray[m_iIdxInner] == -1)
							break;*/
						if (m_iMouseCnt == 0)
							m_iClickedIdx = m_iIdxInner + 6;

						if (m_iMouseCnt == 1)
						{
							if (m_iRemberIdx > 5) // 이전 클릭 했던게 InnerArray에 있던거
								Swap(&m_aInnerArray[m_iRemberIdx - 6], &m_aInnerArray[m_iIdxInner]);
							else if (m_iRemberIdx != 1)
								Swap(&m_aOutterArray[m_iRemberIdx], &m_aInnerArray[m_iIdxInner]);
						}
						++m_iMouseCnt;
						if (m_iMouseCnt > 1)
						{
							m_iMouseCnt = 0;
							m_iClickedIdx = -1;
						}
						m_iRemberIdx = m_iIdxInner + 6;
					}
					else
					{
						m_iLastMouseOnIdx = i * COLSIZE + j + 6;
					}
				}
			}
		}
	
		// OutterArray 클릭 검사
		for (size_t i = 0; i < m_aOutterArray.size(); ++i)
		{
			float fX = m_tOutterFirstPOS.fX + i * fXOuterGap;
			float fY = m_tOutterFirstPOS.fY;
			RECT slot = { (int)fX - (OutterSlotCX >> 1),
				(int)fY - (OutterSlotCY >> 1),
				(int)fX + (OutterSlotCX >> 1),
				(int)fY + (OutterSlotCY >> 1) };

			if (PtInRect(&slot, pt))
			{
				if (KEY_DOWN(VK_LBUTTON)) // 누르면...
				{
					m_iIdxOutter = i;
					
					if (m_iMouseCnt == 0)
						m_iClickedIdx = m_iIdxOutter;
					if (m_iMouseCnt == 1)
					{
						if (m_iIdxOutter != 1) {
							if (m_iRemberIdx > 5) // 이전 클릭 했던게 InnerArray에 있던거
								Swap(&m_aInnerArray[m_iRemberIdx - 6], &m_aOutterArray[m_iIdxOutter]);
							else
								Swap(&m_aOutterArray[m_iRemberIdx], &m_aOutterArray[m_iIdxOutter]);
						}
					}
					++m_iMouseCnt;
					if (m_iMouseCnt > 1)
					{
						m_iMouseCnt = 0;
						m_iClickedIdx = -1;
					}
					if (m_iIdxOutter != 1)
						m_iRemberIdx = m_iIdxOutter;
				}
				else
				{
					m_iLastMouseOnIdx = i;
				}
			}
		}
	}
	if (!bPtInSlot)
		m_iLastMouseOnIdx = -1;
}

int CInventory::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Rect();
	Update_HitRect();
	Key_Check(_fdTime);

	return OBJ_NOEVENT;
}

void CInventory::Late_Update(float _fdTime)
{
	Update_Rect();
	Update_HitRect();
}

// 스크롤 먹이면 안된다!
void CInventory::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	if (CCtrlOwnerMgr::Get_Instance()->Is_Key_Available(KEYOWN::KS_INVENTORY))
	{
		
		HDC hMemSlotDC = CBmpMgr::Get_Instance()->Find_Image("InSlotChange");
		HDC hMemSlotDC2 = CBmpMgr::Get_Instance()->Find_Image("OutSlotChange");
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);
		GdiTransparentBlt(_DC,
			(int)m_tRect.left, (int)m_tRect.top,
			m_tInfo.iCX, m_tInfo.iCY,
			hMemDC,
			0, 0,
			m_tInfo.iCX, m_tInfo.iCY,
			RGB(255, 0, 255));

		for (int i = 0; (size_t)i < m_aOutterArray.size(); ++i)
		{
			BitBlt(_DC,
				(int)(m_tRect.left + 57 + fXOuterGap * i),
				(int)(m_tRect.top + 68),
				43, 42,
				hMemSlotDC2, 0, 0, SRCCOPY);
		}

		for (int i = 0; i < ROWSIZE; ++i)
		{
			for (int j = 0; j < COLSIZE; ++j)
			{
				BitBlt(_DC,
					(int)(m_tRect.left + 52) + 58 * j,
					(int)(m_tRect.top + 240) + 60 * i,
					m_iSlotChangeCX, m_iSlotChangeCY,
					hMemSlotDC, 0, 0, SRCCOPY);
			}
		}

		if (m_iLastMouseOnIdx != -1)
		{
			if (m_iLastMouseOnIdx > 5)
			{
				int i = (m_iLastMouseOnIdx - 6) / COLSIZE;
				int j = (m_iLastMouseOnIdx - 6) % COLSIZE;
				BitBlt(_DC,
					(int)(m_tRect.left + 52) + 58 * j,
					(int)(m_tRect.top + 240) + 60 * i,
					m_iSlotChangeCX, m_iSlotChangeCY,
					hMemSlotDC, m_iSlotChangeCX, 0, SRCCOPY);
			}
			else
			{
				int j = m_iLastMouseOnIdx;
				BitBlt(_DC,
					(int)(m_tRect.left + 57+ fXOuterGap * j),
					(int)(m_tRect.top + 68),
					43, 42,
					hMemSlotDC2, 43, 0, SRCCOPY);
			}
		}

		if (m_iClickedIdx != -1)
		{
			if (m_iClickedIdx > 5)
			{
				int i = (m_iClickedIdx - 6) / COLSIZE;
				int j = (m_iClickedIdx - 6) % COLSIZE;
				BitBlt(_DC,
					(int)(m_tRect.left + 52) + 58 * j,
					(int)(m_tRect.top + 240) + 60 * i,
					m_iSlotChangeCX, m_iSlotChangeCY,
					hMemSlotDC, m_iSlotChangeCX * 2, 0, SRCCOPY);
			}
			else
			{
				int j = m_iClickedIdx;
				BitBlt(_DC,
					(int)(m_tRect.left + 57 + fXOuterGap * j),
					(int)(m_tRect.top + 68),
					43, 42,
					hMemSlotDC2, 43 * 2, 0, SRCCOPY);
			}
		}

		Render_Inventory_Items(_DC, _fdTime, _fScrollX, _fScrollY);

		TCHAR szText[32] = L"";
		HDC hOneTileDC = GetDC(g_hWnd);
		Rectangle(hOneTileDC, 1030, 110, 1300, 190);
		swprintf_s(szText, L"위쪽배열 인덱스[%d], 값 : [%d]", m_iIdxOutter, (m_iIdxOutter != -1) ? m_aOutterArray[m_iIdxOutter] : -999);
		TextOut(hOneTileDC, 1060, 130, szText, lstrlen(szText));
		swprintf_s(szText, L"내부배열 인덱스[%d], 값 : [%d]", m_iIdxInner, (m_iIdxInner != -1) ? m_aInnerArray[m_iIdxInner] : -999);
		TextOut(hOneTileDC, 1060, 150, szText, lstrlen(szText));
		ReleaseDC(g_hWnd, hOneTileDC);
	}
}

void CInventory::Release()
{
}

bool CInventory::Is_Outter_Empty()
{
	bool bRes = true;
	for (size_t i = 0; i < m_aOutterArray.size(); ++i)
	{
		if (m_aOutterArray[i] != -1) {
			bRes = false;
			break;
		}
	}
	return bRes;
}

bool CInventory::Is_Inner_Empty()
{
	bool bRes = true;
	for (size_t i = 0; i < m_aInnerArray.size(); ++i)
	{
		if (m_aInnerArray[i] != -1) {
			bRes = false;
			break;
		}
	}
	return bRes;
}

bool CInventory::Is_Outter_Full()
{
	bool bRes = true;
	for (size_t i = 0; i < m_aOutterArray.size(); ++i)
	{
		if (m_aOutterArray[i] == -1) {
			bRes = false;
			break;
		}
	}
	return bRes;
}



bool CInventory::Is_Inner_Full()
{
	bool bRes = true;
	for (size_t i = 0; i < m_aInnerArray.size(); ++i)
	{
		if (m_aInnerArray[i] == -1) {
			bRes = false;
			break;
		}
	}

	return bRes;
}

int CInventory::Push_Item_In_OuterArr(int _code)
{
	if (_code < 100)	// 스킬일 경우
	{
		for (size_t i = 0; i < m_aOutterArray.size(); ++i)
		{
			if (m_aOutterArray[i] == -1) {
				m_aOutterArray[i] = _code;
				return 1;
			}
		}
		return Push_Item_In_InnerArr(_code);
	}
	else
	{
		return Push_Item_In_InnerArr(_code);
	}
}

int CInventory::Push_Item_In_InnerArr(int _code)
{
	for (int i = 0; i < INVEN_INARR_MAXNUM; ++i)
	{
		if (m_aInnerArray[i] == -1) {
			m_aInnerArray[i] = _code;
			return 2;
		}
	}
	return 0;
}

void CInventory::Pull_InArrayItems()
{
	for (size_t i = 0; i < m_aInnerArray.size() - 1; ++i)
	{
		if (m_aInnerArray[i] == -1)
		{
			for (size_t j = i + 1; j < m_aInnerArray.size(); ++j)
			{
				if (m_aInnerArray[j] != -1)
				{
					m_aInnerArray[i] = m_aInnerArray[j];
					m_aInnerArray[j] = -1;
					break;
				}
			}
		}
	}
}

void CInventory::Render_Inventory_Items(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	HDC hMemDC_35, hMemDC_46;

	for (size_t i = 0; i < m_aOutterArray.size(); ++i)
	{
		if (m_aOutterArray[i] != -1)
		{
			hMemDC_35 = CCardMgr::Get_Instance()->Find_Image35(m_aOutterArray[i]);
			float fX = m_tOutterFirstPOS.fX + i * fXOuterGap;
			float fY = m_tOutterFirstPOS.fY;
			fX += _fScrollX;
			fY += _fScrollY;
			GdiTransparentBlt(_DC,
				(int)(fX - (OutterSlotCX >> 1)), (int)(fY - (OutterSlotCY >> 1)),
				OutterSlotCX, OutterSlotCY,
				hMemDC_35,
				0, 0,
				OutterSlotCX, OutterSlotCY,
				MAGENTA_COLOR);
		}
	}

	for (int i = 0; i < INVEN_INARR_MAXNUM; ++i)
	{
		if (m_aInnerArray[i] != -1)
		{
			hMemDC_46 = CCardMgr::Get_Instance()->Find_Image46(m_aInnerArray[i]);
			float fX = m_tInnerFirstPOS.fX + (i % COLSIZE) * fXInnerGap;
			float fY = m_tInnerFirstPOS.fY + (i / COLSIZE) * fYInnerGap;
			fX += _fScrollX;
			fY += _fScrollY;
			GdiTransparentBlt(_DC,
				(int)(fX - (InnerSlotCX >> 1)), (int)(fY - (InnerSlotCY >> 1)),
				InnerSlotCX, InnerSlotCY,
				hMemDC_46,
				0, 0,
				InnerSlotCX, InnerSlotCY,
				MAGENTA_COLOR);
		}
	}
}
