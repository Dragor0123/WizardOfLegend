#include "../stdafx.h"
#include "Wardrobe.h"
#include "../MyBitmap/BmpMgr.h"
#include "FButton.h"
#include "ObjMgr.h"
#include "../Manager/CtrlOwnerMgr.h"
#include "WardInven.h"

const DWORD CWardrobe::IDLE_FRAME_SPEED = 85L;
const DWORD CWardrobe::OPEN_FRAME_SPEED = 40L;

CWardrobe::CWardrobe()
	: m_pWardInven(nullptr)
{
}

CWardrobe::~CWardrobe()
{
	Release();
}

bool CWardrobe::Initialize()
{
	if (!CFAble::Initialize())
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Fable/Wardrobe.bmp", "Wardrobe"))
		return false;

	m_tInfo.iCX = 256;
	m_tInfo.iCY = 256;

	Equalize_HitPosInfoPos();
	m_tHitInfo.iCX = 228;
	m_tHitInfo.iCY = 240;

	m_strFrameKey = "Wardrobe";
	m_ePreState = WARD_STATE::END;
	m_eCurState = WARD_STATE::IDLE;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 5;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = IDLE_FRAME_SPEED;
	m_tFrame.dwFrameTime = GetTickCount();

	CObj* pFButton = CAbstractFactory<CFButton>::Create(m_tInfo.fX, m_tInfo.fY - 134.f,
		OBJID::PLAZA_UI);
	Set_fButton(pFButton);
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAZA_UI, pFButton);

	m_pWardInven = CAbstractFactory<CWardInven>::Create(this);
	CObjMgr::Get_Instance()->Add_Object(OBJID::INVENTORY,
		m_pWardInven);

	return true;
}

int CWardrobe::Update(float _fdTime)
{
	if (OBJ_DEAD == CFAble::Update(_fdTime))
		return OBJ_DEAD;

	Move_Frame();
	Scene_Change();
	Update_Rect();
	Update_HitRect();
	return OBJ_NOEVENT;
}

void CWardrobe::Late_Update(float _fdTime)
{
	CFAble::Key_Check(_fdTime);
}

void CWardrobe::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);

	GdiTransparentBlt(_DC
		, (int)(m_tRect.left + _fScrollX)
		, (int)(m_tRect.top + _fScrollY)
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * m_tInfo.iCX
		, m_tFrame.iFrameScene * m_tInfo.iCY
		, m_tInfo.iCX, m_tInfo.iCY
		, MAGENTA_COLOR);

	//히트박스 조건 걸자
	Draw_HitBox(_DC, _fScrollX, _fScrollY);

	//TCHAR szText[64] = L"";
	//HDC hTempDC = GetDC(g_hWnd);
	//Rectangle(hTempDC, 1030, 110, 1600, 200);
	//swprintf_s(szText, L"이전상태: %d, 현재상태: %d, 버튼:[%c]", (int)m_ePreState, (int)m_eCurState,
	//	(dynamic_cast<CFButton*>(m_pFbutton)->Is_Enable()) ? 'T':'F');
	//TextOut(hTempDC, 1060, 150, szText, lstrlen(szText));
	//swprintf_s(szText, L"프레임start: %d, 프레임End: %d", m_tFrame.iFrameStart, m_tFrame.iFrameEnd);
	//TextOut(hTempDC, 1060, 180, szText, lstrlen(szText));
	//ReleaseDC(g_hWnd, hTempDC);
}

void CWardrobe::Release()
{
	if (m_pWardInven)
		CObjMgr::Get_Instance()->Delete_A_Obj(OBJID::INVENTORY, m_pWardInven);

	CBmpMgr::Get_Instance()->Delete_Bmp("Wardrobe");
}

void CWardrobe::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CWardrobe::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = IDLE_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CWardrobe::OPEN:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameSpeed = OPEN_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CWardrobe::CLOSE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 2;
			m_tFrame.dwFrameSpeed = OPEN_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CWardrobe::Move_Frame()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();

		if (m_ePreState == CWardrobe::IDLE)
		{
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_tFrame.iFrameStart = 0;
		}
		else if (m_ePreState == CWardrobe::OPEN)
		{
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				m_tFrame.iFrameStart = m_tFrame.iFrameEnd;
				CCtrlOwnerMgr::Get_Instance()->WardInven_On();
			}
		}
		else if (m_ePreState == CWardrobe::CLOSE)
		{
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				m_eCurState = CWardrobe::IDLE;
			}
		}
	}
}

void CWardrobe::Do_FButton_Action(float _fdTime)
{
	if (m_ePreState == CWardrobe::IDLE)
	{
		m_eCurState = CWardrobe::OPEN;
		// open 이 완료되면 인벤토리를 띄운다!
	}
	//if (m_ePreState == CWardrobe::OPEN && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
	//{
	//	m_eCurState = CWardrobe::CLOSE;
	//}
}
