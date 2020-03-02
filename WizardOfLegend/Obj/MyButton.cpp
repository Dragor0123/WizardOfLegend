#include "../stdafx.h"
#include "MyButton.h"
#include "../Manager/KeyMgr.h"
#include "../Manager/SceneMgr.h"
#include "../MyBitmap/BmpMgr.h"
#include "../Manager/CtrlOwnerMgr.h"

CMyButton::CMyButton()
	: m_iDrawID(0)
{
}


CMyButton::~CMyButton()
{
	Release();
}

bool CMyButton::Initialize()
{
	m_tInfo.iCX = 140;
	m_tInfo.iCY = 32;
	Equalize_HitSzInfoSz();
	m_eRenderGroupID = GROUPID::UI;
	return true;
}


int CMyButton::Update(float _fdTime)
{
	if (OBJ_DEAD == CStopObj::Update(_fdTime))
		return OBJ_DEAD;
	return OBJ_NOEVENT;
}

void CMyButton::Late_Update(float _fdTime)
{
	Update_Rect();
	Update_HitRect();
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_tRect, pt))
	{
		if (CCtrlOwnerMgr::Get_Instance()->Is_Key_Available(KEYOWN::KS_TITLEMENU))
		{
			if (KEY_DOWN(VK_LBUTTON))
			{
				if ("Start" == m_strFrameKey)
					CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_PLAZA);
				else if ("Edit" == m_strFrameKey)
					CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_EDIT);
				//else if ("Developer" == m_strFrameKey)
				else if ("Exit" == m_strFrameKey)
					DestroyWindow(g_hWnd);
				return;
			}
		}
		m_iDrawID = 1;
	}
	else {
		m_iDrawID = 0;
	}
}

void CMyButton::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);
	GdiTransparentBlt(_DC
		, (int)m_tRect.left, (int)m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_iDrawID * m_tInfo.iCX, 0
		, m_tInfo.iCX, m_tInfo.iCY
		, RGB(255, 0, 255));
}

void CMyButton::Release()
{
}

void CMyButton::Key_Check(float _fdTime)
{
}
