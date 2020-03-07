#include "../stdafx.h"
#include "Logo.h"
#include "../Manager/KeyMgr.h"
#include "../Manager/SceneMgr.h"
#include "../MyBitmap/BmpMgr.h"

CLogo::CLogo()
	: m_iKeyCnt(0), m_fTime(0.f), m_fTime2(0.f)
{
	ZeroMemory(&m_tBF, sizeof(m_tBF));
}


CLogo::~CLogo()
{
	Release();
}

bool CLogo::Key_Check()
{
	if (KEY_DOWN(VK_RETURN) || KEY_DOWN(VK_LBUTTON)) {
		if (m_iKeyCnt == 0)
			m_iKeyCnt = 1;
	}
	return true;
}

bool CLogo::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Logo/Logo.bmp", "Logo"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Logo/contingent99.bmp", "contingent"))
		return false;

	m_tBF.BlendOp = 0;
	m_tBF.BlendFlags = 0;
	m_tBF.SourceConstantAlpha = 255;
	m_tBF.AlphaFormat = AC_SRC_OVER;

	//
	m_iKeyCnt = 3;
	return true;
}

int CLogo::Update(float _fdTime)
{
	Key_Check();
	if (m_iKeyCnt == 2)
		m_fTime += _fdTime;

	if (m_iKeyCnt == 4)
		m_fTime2 += _fdTime;

	return OBJ_NOEVENT;
}

void CLogo::Late_Update(float _fdTime)
{
	if (m_iKeyCnt == 1)
	{
		m_tBF.SourceConstantAlpha -= 4;
		if (m_tBF.SourceConstantAlpha >= 0 && m_tBF.SourceConstantAlpha < 5)
		{
			m_tBF.SourceConstantAlpha = 0;
			m_iKeyCnt = 2;
		}
	}

	if (m_iKeyCnt == 2 && m_fTime > 1.25f)
	{
		if (m_tBF.SourceConstantAlpha < 254)
			m_tBF.SourceConstantAlpha += 2;
		if (m_tBF.SourceConstantAlpha >= 254 && m_tBF.SourceConstantAlpha < 256)
		{
			m_tBF.SourceConstantAlpha = 255;
			m_iKeyCnt = 3;
		}
	}
	if (m_iKeyCnt == 3)
	{
		m_tBF.SourceConstantAlpha -= 2;
		if (m_tBF.SourceConstantAlpha >= 0 && m_tBF.SourceConstantAlpha < 2)
		{
			m_tBF.SourceConstantAlpha = 0;
			m_iKeyCnt = 4;
		}
	}

	if (m_iKeyCnt == 4 && m_fTime2 > 0.6f)
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_MENU);
}

void CLogo::Collision(float _fdTime)
{
	return;
}

void CLogo::Render(HDC _DC, float _fdTime)
{
	//"BackBuffer"
	// HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image("Logo");
	// BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	HDC hBackDC = CBmpMgr::Get_Instance()->Find_Image("Back_black");
	HDC hMemDC = NULL;
	if (m_iKeyCnt == 0 || m_iKeyCnt == 1)
		hMemDC = CBmpMgr::Get_Instance()->Find_Image("Logo");
	else if (m_iKeyCnt >= 2)
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Image("contingent");
	}

	if (m_iKeyCnt >= 2 && m_fTime > 1.25f)
	{
		BitBlt(_DC,
			0, 0, WINCX, WINCY,
			hBackDC,
			0, 0, SRCCOPY);
	}

	GdiAlphaBlend(
		_DC,
		0, 0, WINCX, WINCY,
		hMemDC,
		0, 0, WINCX, WINCY,
		m_tBF
	);
}

void CLogo::Release()
{
	CBmpMgr::Get_Instance()->Delete_Bmp("contingent");
	CBmpMgr::Get_Instance()->Delete_Bmp("Logo");
}
