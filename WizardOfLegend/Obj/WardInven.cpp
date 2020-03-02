#include "../stdafx.h"
#include "WardInven.h"
#include "../MyBitmap/BmpMgr.h"
#include "../Manager/CtrlOwnerMgr.h"
#include "../Manager/KeyMgr.h"
#include "Wardrobe.h"
#include "ObjMgr.h"
#include "RobeIcon.h"

const float CWardInven::m_fRADIUS = 156.f;
const float CWardInven::m_fCircleY = 231.f;
const float CWardInven::m_fOFFSET_X = 23.f;
const float CWardInven::m_fOFFSET_Y = 32.f;

CWardInven::CWardInven()
{
}

CWardInven::~CWardInven()
{
	Release();
}

bool CWardInven::Initialize()
{
	m_tInfo.fX = 297.f + m_fOFFSET_X;
	m_tInfo.fY = 324.f + m_fOFFSET_Y;
	m_tInfo.iCX = 595;
	m_tInfo.iCY = 648;

	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/UI/WardInven_595_648.bmp", "WardInven"))
		return false;

	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/UI/robe_icon.bmp", "robe_icon"))
		return false;
	
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/UI/WardInven_arrow.bmp", "WardInven_arrow"))
		return false;

	if (m_pTarget == nullptr)
		return false;

	m_strFrameKey = "WardInven";

	Equalize_HitPosInfoPos();
	Equalize_HitSzInfoSz();
	Update_Rect();
	Update_HitRect();
	
	m_fAngle = 90.f;

	m_eRenderGroupID = GROUPID::UI;
	Insert_Robe_List();
	return true;
}

int CWardInven::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	Key_Check(_fdTime);
	Update_Robe_List(_fdTime);
	return OBJ_NOEVENT;
}

void CWardInven::Late_Update(float _fdTime)
{
	Late_Update_Robe_List(_fdTime);
}

void CWardInven::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	if (CCtrlOwnerMgr::Get_Instance()->Is_Key_Available(KEYOWN::KS_WARDINVEN))
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);
		HDC hArrowDC = CBmpMgr::Get_Instance()->Find_Image("WardInven_arrow");
		float fArrowYTop = m_fCircleY + m_fOFFSET_Y - sinf(Degree_To_Radian(90.f)) * m_fRADIUS - 29.f - 17.f;

		GdiTransparentBlt(_DC,
			(int)m_tRect.left, (int)m_tRect.top,
			m_tInfo.iCX, m_tInfo.iCY,
			hMemDC,
			0, 0,
			m_tInfo.iCX, m_tInfo.iCY,
			RGB(255, 0, 255));

		BitBlt(_DC,
			(int)(m_tInfo.fX - 8.5f), (int)fArrowYTop,
			17, 15,
			hArrowDC,
			0, 0,
			SRCCOPY);

		list<CObj*>::iterator iter = m_listRobe.begin();
		for (; iter != m_listRobe.end(); ++iter)
			(*iter)->Render(_DC, _fdTime);

		//
		TCHAR szText[64] = L"";
		HDC hTempDC = GetDC(g_hWnd);
		Rectangle(hTempDC, 1030, 110, 1600, 200);

		iter = find_if(m_listRobe.begin(), m_listRobe.end(), [=](CObj* p) { return p->Get_PosX() == m_tInfo.fX; });
		swprintf_s(szText, 64, L"현재 로브 : [%d]", dynamic_cast<CRobeIcon*>(*iter)->Get_DrawID());
		TextOut(hTempDC, 1060, 130, szText, lstrlen(szText));

		ReleaseDC(g_hWnd, hTempDC);
	}
}

void CWardInven::Release()
{
	CBmpMgr::Get_Instance()->Delete_Bmp("WardInven");
	CBmpMgr::Get_Instance()->Delete_Bmp("robe_icon");
	CBmpMgr::Get_Instance()->Delete_Bmp("WardInven_arrow");
	Safe_Delete_VecList(m_listRobe);
}

void CWardInven::Update_Robe_List(float _fdTime)
{
	list<CObj*>::iterator iterBeg = m_listRobe.begin();
	for (; iterBeg != m_listRobe.end(); )
	{
		int iEvent = (*iterBeg)->Update(_fdTime);

		if (OBJ_DEAD == iEvent)
		{
			Safe_Delete<CObj*>(*iterBeg);
			iterBeg = m_listRobe.erase(iterBeg);
		}
		else
		{
			++iterBeg;
		}
	}
}

void CWardInven::Late_Update_Robe_List(float _fdTime)
{
	for (auto& pObj : m_listRobe)
	{
		pObj->Late_Update(_fdTime);
	}
}

void CWardInven::Insert_Robe_List()
{
	float fX = 0.f; 
	float fY = 0.f;
	for (int i = 0; i < 9; ++i)
	{
		fX = m_tInfo.fX + cosf(Degree_To_Radian(m_fAngle - 40 * i)) * m_fRADIUS;
		fY = m_fCircleY + m_fOFFSET_Y - sinf(Degree_To_Radian(m_fAngle - 40 * i)) * m_fRADIUS;
		CObj* pRobe = CAbstractFactory<CRobeIcon>::Create(fX, fY, "robe_icon");
		static_cast<CRobeIcon*>(pRobe)->Set_DrawID(i);
		m_listRobe.push_back(pRobe);
	}
}

void CWardInven::Key_Check(float _fdTime)
{
	if (!CCtrlOwnerMgr::Get_Instance()->Is_Key_Available(KEYOWN::KS_WARDINVEN))
		return;

	if (KEY_DOWN(VK_ESCAPE) || KEY_DOWN('F'))
	{
		CCtrlOwnerMgr::Get_Instance()->WardInven_Off();
		dynamic_cast<CWardrobe*>(m_pTarget)->Close_Wardrobe();
	}


	if (!m_listRobe.empty())
	{
		float firstPosX = 0.f, firstPosY = 0.f;
		
		if (KEY_DOWN('A') || KEY_DOWN(VK_LEFT))
		{
			list<CObj*>::reverse_iterator rIterBeg = m_listRobe.rbegin();
			list<CObj*>::reverse_iterator rIterEnd = m_listRobe.rend();
			firstPosX = (*rIterBeg)->Get_PosX();
			firstPosY = (*rIterBeg)->Get_PosY();
			--rIterEnd;
			for (; rIterBeg != rIterEnd; ++rIterBeg)
			{
				auto& nextIter = ++rIterBeg;
				float fNewX = (*nextIter)->Get_PosX();
				float fNewY = (*nextIter)->Get_PosY();
				--rIterBeg;
				(*rIterBeg)->Set_Pos(fNewX, fNewY);
			}
			(*rIterBeg)->Set_Pos(firstPosX, firstPosY);
		}
		else if (KEY_DOWN('D') || KEY_DOWN(VK_RIGHT))
		{
			list<CObj*>::iterator IterBeg = m_listRobe.begin();
			list<CObj*>::iterator IterEnd = m_listRobe.end();
			firstPosX = (*IterBeg)->Get_PosX();
			firstPosY = (*IterBeg)->Get_PosY();
			--IterEnd;
			for (; IterBeg != IterEnd; ++IterBeg)
			{
				auto& nextIter = ++IterBeg;
				float fNewX = (*nextIter)->Get_PosX();
				float fNewY = (*nextIter)->Get_PosY();
				--IterBeg;
				(*IterBeg)->Set_Pos(fNewX, fNewY);
			}
			(*IterBeg)->Set_Pos(firstPosX, firstPosY);
		}

		if (KEY_DOWN(VK_SPACE) || KEY_DOWN(VK_RETURN))
		{
			//
		}
	}
}
