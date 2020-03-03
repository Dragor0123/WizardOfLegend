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

COLORREF CWardInven::tRobeColor[ROBE::END][21];

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
	Init_RobeColor();
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


void CWardInven::Init_RobeColor()
{
	// 0 -> RED
	tRobeColor[0][0] = RGB(101, 203, 195);
	tRobeColor[0][1] = RGB(244, 202, 1);
	tRobeColor[0][2] = RGB(251, 207, 0);
	tRobeColor[0][3] = RGB(2, 2, 2);
	tRobeColor[0][4] = RGB(89, 41, 37);
	tRobeColor[0][5] = RGB(69, 31, 29);
	tRobeColor[0][6] = RGB(147, 86, 17);
	tRobeColor[0][7] = RGB(125, 80, 29);
	tRobeColor[0][8] = RGB(106, 69, 27);
	tRobeColor[0][9] = RGB(77, 45, 8);
	tRobeColor[0][10] = RGB(234, 200, 15);
	tRobeColor[0][11] = RGB(196, 161, 11);
	tRobeColor[0][12] = RGB(100, 100, 100);
	tRobeColor[0][13] = RGB(166, 51, 57);
	tRobeColor[0][14] = RGB(147, 37, 43);
	tRobeColor[0][15] = RGB(136, 21, 29);
	tRobeColor[0][16] = RGB(125, 23, 33);
	tRobeColor[0][17] = RGB(111, 22, 31);
	tRobeColor[0][18] = RGB(102, 27, 35);
	tRobeColor[0][19] = RGB(77, 23, 29);
	tRobeColor[0][20] = RGB(41, 13, 17);

	// 1 -> blue (인내)
	tRobeColor[1][0] = RGB(255, 255, 252);
	tRobeColor[1][1] = RGB(232, 141, 10);
	tRobeColor[1][2] = RGB(211, 131, 18);
	tRobeColor[1][3] = RGB(26, 42, 54);
	tRobeColor[1][4] = RGB(82, 86, 99);
	tRobeColor[1][5] = RGB(66, 70, 80);
	tRobeColor[1][6] = RGB(163, 101, 76);
	tRobeColor[1][7] = RGB(121, 81, 69);
	tRobeColor[1][8] = RGB(105, 71, 62);
	tRobeColor[1][9] = RGB(80, 46, 36);
	tRobeColor[1][10] = RGB(255, 219, 167);
	tRobeColor[1][11] = RGB(242, 206, 157);
	tRobeColor[1][12] = RGB(122, 118, 113);
	tRobeColor[1][13] = RGB(57, 105, 154);
	tRobeColor[1][14] = RGB(41, 89, 138);
	tRobeColor[1][15] = RGB(30, 79, 123);
	tRobeColor[1][16] = RGB(30, 78, 119);
	tRobeColor[1][17] = RGB(29, 70, 104);
	tRobeColor[1][18] = RGB(31, 66, 95);
	tRobeColor[1][19] = RGB(27, 53, 75);
	tRobeColor[1][20] = RGB(24, 53, 88);

	// 2 노랑 (경외)
	tRobeColor[2][0] = RGB(255, 255, 255);
	tRobeColor[2][1] = RGB(255, 229, 0);
	tRobeColor[2][2] = RGB(254, 208, 5);
	tRobeColor[2][3] = RGB(2, 2, 2);
	tRobeColor[2][4] = RGB(138, 92, 58);
	tRobeColor[2][5] = RGB(116, 77, 49);
	tRobeColor[2][6] = RGB(155, 97, 29);
	tRobeColor[2][7] = RGB(121, 77, 28);
	tRobeColor[2][8] = RGB(105, 68, 27);
	tRobeColor[2][9] = RGB(80, 47, 10);
	tRobeColor[2][10] = RGB(255, 223, 170);
	tRobeColor[2][11] = RGB(245, 209, 159);
	tRobeColor[2][12] = RGB(120, 117, 112);
	tRobeColor[2][13] = RGB(224, 184, 38);
	tRobeColor[2][14] = RGB(214, 172, 26);
	tRobeColor[2][15] = RGB(200, 153, 4);
	tRobeColor[2][16] = RGB(189, 141, 8);
	tRobeColor[2][17] = RGB(172, 132, 10);
	tRobeColor[2][18] = RGB(168, 128, 19);
	tRobeColor[2][19] = RGB(139, 105, 24);
	tRobeColor[2][20] = RGB(84, 50, 49);

	// 3. 주황 Fury
	tRobeColor[3][0] = RGB(236, 32, 68);
	tRobeColor[3][1] = RGB(221, 224, 227);
	tRobeColor[3][2] = RGB(173, 172, 173);
	tRobeColor[3][3] = RGB(49, 18, 23);
	tRobeColor[3][4] = RGB(97, 80, 72);
	tRobeColor[3][5] = RGB(80, 68, 61);
	tRobeColor[3][6] = RGB(52, 46, 40);
	tRobeColor[3][7] = RGB(112, 73, 32);
	tRobeColor[3][8] = RGB(96, 63, 29);
	tRobeColor[3][9] = RGB(45, 43, 32);
	tRobeColor[3][10] = RGB(249, 205, 160);
	tRobeColor[3][11] = RGB(239, 197, 154);
	tRobeColor[3][12] = RGB(110, 52, 47);
	tRobeColor[3][13] = RGB(237, 153, 41);
	tRobeColor[3][14] = RGB(230, 136, 26);
	tRobeColor[3][15] = RGB(215, 125, 23);
	tRobeColor[3][16] = RGB(159, 88, 21);
	tRobeColor[3][17] = RGB(181, 98, 21);
	tRobeColor[3][18] = RGB(188, 101, 19);
	tRobeColor[3][19] = RGB(134, 72, 16);
	tRobeColor[3][20] = RGB(43, 19, 23);

	// 4.초록 템포(완급)
	tRobeColor[4][0] = RGB(255, 255, 252);
	tRobeColor[4][1] = RGB(255, 231, 0);
	tRobeColor[4][2] = RGB(211, 131, 18);
	tRobeColor[4][3] = RGB(26, 42, 54);
	tRobeColor[4][4] = RGB(72, 85, 79);
	tRobeColor[4][5] = RGB(58, 66, 61);
	tRobeColor[4][6] = RGB(163, 101, 76);
	tRobeColor[4][7] = RGB(121, 81, 69);
	tRobeColor[4][8] = RGB(105, 71, 62);
	tRobeColor[4][9] = RGB(80, 46, 36);
	tRobeColor[4][10] = RGB(255, 219, 167);
	tRobeColor[4][11] = RGB(242, 206, 157);
	tRobeColor[4][12] = RGB(122, 118, 113);
	tRobeColor[4][13] = RGB(96, 170, 116);
	tRobeColor[4][14] = RGB(81, 154, 99);
	tRobeColor[4][15] = RGB(68, 137, 82);
	tRobeColor[4][16] = RGB(68, 128, 79);
	tRobeColor[4][17] = RGB(64, 117, 70);
	tRobeColor[4][18] = RGB(64, 108, 67);
	tRobeColor[4][19] = RGB(54, 85, 54);
	tRobeColor[4][20] = RGB(35, 47, 39);

	// 5. 보라 스피릿(정신)
	tRobeColor[5][0] = RGB(217, 255, 254);
	tRobeColor[5][1] = RGB(229, 210, 98);
	tRobeColor[5][2] = RGB(212, 190, 90);
	tRobeColor[5][3] = RGB(1, 1, 1);
	tRobeColor[5][4] = RGB(68, 48, 79);
	tRobeColor[5][5] = RGB(48, 38, 59);
	tRobeColor[5][6] = RGB(157, 100, 31);
	tRobeColor[5][7] = RGB(112, 73, 32);
	tRobeColor[5][8] = RGB(96, 63, 29);
	tRobeColor[5][9] = RGB(70, 41, 11);
	tRobeColor[5][10] = RGB(202, 174, 134);
	tRobeColor[5][11] = RGB(192, 165, 127);
	tRobeColor[5][12] = RGB(114, 112, 109);
	tRobeColor[5][13] = RGB(167, 106, 161);
	tRobeColor[5][14] = RGB(155, 92, 149);
	tRobeColor[5][15] = RGB(140, 71, 135);
	tRobeColor[5][16] = RGB(131, 70, 128);
	tRobeColor[5][17] = RGB(118, 63, 116);
	tRobeColor[5][18] = RGB(112, 66, 109);
	tRobeColor[5][19] = RGB(86, 52, 83);
	tRobeColor[5][20] = RGB(64, 25, 52);

	//  6번 하양 탐욕
	tRobeColor[6][0] = RGB(255, 255, 255);
	tRobeColor[6][1] = RGB(230, 220, 171);
	tRobeColor[6][2] = RGB(196, 183, 145);
	tRobeColor[6][3] = RGB(2, 2, 2);
	tRobeColor[6][4] = RGB(74, 64, 63);
	tRobeColor[6][5] = RGB(58, 50, 50);
	tRobeColor[6][6] = RGB(119, 131, 118);
	tRobeColor[6][7] = RGB(83, 102, 105);
	tRobeColor[6][8] = RGB(69, 89, 95);
	tRobeColor[6][9] = RGB(77, 49, 15);
	tRobeColor[6][10] = RGB(237, 206, 163);
	tRobeColor[6][11] = RGB(222, 192, 153);
	tRobeColor[6][12] = RGB(117, 114, 110);
	tRobeColor[6][13] = RGB(187, 182, 178);
	tRobeColor[6][14] = RGB(173, 168, 162);
	tRobeColor[6][15] = RGB(148, 141, 135);
	tRobeColor[6][16] = RGB(112, 106, 101);
	tRobeColor[6][17] = RGB(122, 117, 113);
	tRobeColor[6][18] = RGB(127, 123, 120);
	tRobeColor[6][19] = RGB(87, 83, 83);
	tRobeColor[6][20] = RGB(93, 61, 106);

	// 7번 검정 Venture(모험)
	tRobeColor[7][0] = RGB(224, 55, 31);
	tRobeColor[7][1] = RGB(248, 202, 94);
	tRobeColor[7][2] = RGB(225, 183, 86);
	tRobeColor[7][3] = RGB(2, 2, 2);
	tRobeColor[7][4] = RGB(50, 69, 76);
	tRobeColor[7][5] = RGB(36, 50, 54);
	tRobeColor[7][6] = RGB(130, 97, 72);
	tRobeColor[7][7] = RGB(104, 80, 62);
	tRobeColor[7][8] = RGB(86, 68, 56);
	tRobeColor[7][9] = RGB(29, 36, 59);
	tRobeColor[7][10] = RGB(140, 153, 151);
	tRobeColor[7][11] = RGB(133, 146, 143);
	tRobeColor[7][12] = RGB(94, 110, 109);
	tRobeColor[7][13] = RGB(70, 88, 98);
	tRobeColor[7][14] = RGB(55, 74, 83);
	tRobeColor[7][15] = RGB(39, 59, 70);
	tRobeColor[7][16] = RGB(34, 51, 59);
	tRobeColor[7][17] = RGB(26, 40, 46);
	tRobeColor[7][18] = RGB(24, 32, 37);
	tRobeColor[7][19] = RGB(6, 9, 11);
	tRobeColor[7][20] = RGB(17, 13, 15);

	// 8번 회청색 긍지(pride)
	tRobeColor[8][0] = RGB(255, 255, 255);
	tRobeColor[8][1] = RGB(156, 175, 190);
	tRobeColor[8][2] = RGB(142, 160, 172);
	tRobeColor[8][3] = RGB(2, 2, 2);
	tRobeColor[8][4] = RGB(40, 68, 95);
	tRobeColor[8][5] = RGB(31, 63, 75);
	tRobeColor[8][6] = RGB(123, 86, 37);
	tRobeColor[8][7] = RGB(75, 61, 61);
	tRobeColor[8][8] = RGB(58, 48, 49);
	tRobeColor[8][9] = RGB(32, 22, 22);
	tRobeColor[8][10] = RGB(13, 14, 18);
	tRobeColor[8][11] = RGB(12, 14, 16);
	tRobeColor[8][12] = RGB(98, 98, 98);
	tRobeColor[8][13] = RGB(104, 128, 143);
	tRobeColor[8][14] = RGB(89, 114, 130);
	tRobeColor[8][15] = RGB(68, 99, 117); 
	tRobeColor[8][16] = RGB(65, 92, 106); 
	tRobeColor[8][17] = RGB(58, 81, 93);
	tRobeColor[8][18] = RGB(58, 76, 85);
	tRobeColor[8][19] = RGB(41, 51, 57);
	tRobeColor[8][20] = RGB(20, 27, 24);
}
