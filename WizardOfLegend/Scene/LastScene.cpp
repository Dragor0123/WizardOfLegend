#include "../stdafx.h"
#include "LastScene.h"
#include "../Manager/KeyMgr.h"
#include "../MyBitmap/BmpMgr.h"

CLastScene::CLastScene()
	: m_iKeyCnt(0), m_fTime(0.f)
{
	ZeroMemory(&m_tBF, sizeof(m_tBF));
}


CLastScene::~CLastScene()
{
	Release();
}

bool CLastScene::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Logo/GameOver.bmp", "GameOver"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Logo/Thankyou.bmp", "Thankyou"))
		return false;

	m_tBF.BlendOp = 0;
	m_tBF.BlendFlags = 0;
	m_tBF.SourceConstantAlpha = 0;
	m_tBF.AlphaFormat = AC_SRC_OVER;
	return true;
}

int CLastScene::Update(float _fdTime)
{
	Key_Check();

	if (m_iKeyCnt == 2)
		m_fTime += _fdTime;
	return OBJ_NOEVENT;
}

void CLastScene::Late_Update(float _fdTime)
{
	if (m_iKeyCnt == 1)
	{
		if (m_tBF.SourceConstantAlpha < 254)
			m_tBF.SourceConstantAlpha += 2;
		if (m_tBF.SourceConstantAlpha >= 254 && m_tBF.SourceConstantAlpha < 256)
		{
			m_tBF.SourceConstantAlpha = 255;
			m_iKeyCnt = 2;
		}
	}

	if (m_iKeyCnt == 2 && m_fTime > 1.5f)
	{
		m_tBF.SourceConstantAlpha -= 2;
		if (m_tBF.SourceConstantAlpha >= 0 && m_tBF.SourceConstantAlpha < 2)
		{
			m_tBF.SourceConstantAlpha = 0;
			m_iKeyCnt = 3;
		}
	}

	if (m_iKeyCnt == 3)
	{
		if (m_tBF.SourceConstantAlpha < 254)
			m_tBF.SourceConstantAlpha += 2;
		if (m_tBF.SourceConstantAlpha >= 254 && m_tBF.SourceConstantAlpha < 256)
		{
			m_tBF.SourceConstantAlpha = 255;
			m_iKeyCnt = 4;
		}
	}
}

bool CLastScene::Key_Check()
{
	if (KEY_DOWN(VK_RETURN) || KEY_DOWN(VK_LBUTTON)) {
		if (m_iKeyCnt == 0)
			m_iKeyCnt = 1;
		if (m_iKeyCnt == 4)
		{
			m_iKeyCnt = 5;
			DestroyWindow(g_hWnd);
		}
	}
	return true;
}

void CLastScene::Render(HDC _DC, float _fdTime)
{
	HDC hBackDC = CBmpMgr::Get_Instance()->Find_Image("Back_black");
	HDC hMemDC = NULL;
	

	BitBlt(_DC,
		0, 0, WINCX, WINCY,
		hBackDC,
		0, 0, SRCCOPY);
	if (m_iKeyCnt < 3)
		hMemDC = CBmpMgr::Get_Instance()->Find_Image("GameOver");
	else
		hMemDC = CBmpMgr::Get_Instance()->Find_Image("Thankyou"); 

	GdiAlphaBlend(
		_DC,
		0, 0, WINCX, WINCY,
		hMemDC,
		0, 0, WINCX, WINCY,
		m_tBF
	);
}

void CLastScene::Collision(float _fdTime)
{
	return;
}

void CLastScene::Release()
{
	CBmpMgr::Get_Instance()->Delete_Bmp("GameOver");
	CBmpMgr::Get_Instance()->Delete_Bmp("Back_black");
}
