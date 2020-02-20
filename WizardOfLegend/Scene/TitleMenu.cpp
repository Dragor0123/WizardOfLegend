#include "../stdafx.h"
#include "TitleMenu.h"
#include "../Manager/KeyMgr.h"
#include "../Manager/SceneMgr.h"
#include "../MyBitmap/BmpMgr.h"
#include "../Obj/ObjMgr.h"
#include "../Obj/MyButton.h"

const int CTitleMenu::iLogoX = 141;
int CTitleMenu::iLogoY = 340;

CTitleMenu::CTitleMenu()
	: m_bTitleLogoUp(false)
{
}


CTitleMenu::~CTitleMenu()
{
	Release();
}

bool CTitleMenu::Initialize()
{
	iLogoY = 340;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Menu/TitleScreen.bmp", "TitleScreen"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Menu/TitleLogo_ori.bmp", "TitleLogo"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Menu/PressAnyKey.bmp", "TitlePress"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Menu/start_button.bmp", "Start"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Menu/edit_button.bmp", "Edit"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Menu/devel_button.bmp", "Developer"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Menu/exit_button.bmp", "Exit"))
		return false;

	CObj*	pObj = CAbstractFactory<CMyButton>::Create(492.f, 360.f);
	dynamic_cast<CMyButton*>(pObj)->Set_FrameKey("Start");
	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, pObj);

	pObj = CAbstractFactory<CMyButton>::Create(492.f, 420.f);
	dynamic_cast<CMyButton*>(pObj)->Set_FrameKey("Edit");
	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, pObj);

	pObj = CAbstractFactory<CMyButton>::Create(492.f, 480.f);
	dynamic_cast<CMyButton*>(pObj)->Set_FrameKey("Developer");
	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, pObj);

	pObj = CAbstractFactory<CMyButton>::Create(492.f, 540.f);
	dynamic_cast<CMyButton*>(pObj)->Set_FrameKey("Exit");
	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, pObj);

	return true;
}

int CTitleMenu::Update(float _fdTime)
{
	Key_Check();

	if (m_bTitleLogoUp) {
		iLogoY -= (int)(400.f * _fdTime);
		if (iLogoY < 172) {
			iLogoY = 172;
		}
	}

	CObjMgr::Get_Instance()->Update(_fdTime);

	return OBJ_NOEVENT;
}

void CTitleMenu::Late_Update(float _fdTime)
{
	CObjMgr::Get_Instance()->Late_Update(_fdTime);
}

void CTitleMenu::Collision(float _fdTime)
{
	return;
}

// 아무버튼 : x = 408

void CTitleMenu::Render(HDC _DC, float _fdTime)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image("TitleScreen");
	BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
	hMemDC = CBmpMgr::Get_Instance()->Find_Image("TitleLogo");

	GdiTransparentBlt(_DC, iLogoX, iLogoY
		,718, 144
		,hMemDC
		,0, 0
		,718, 144
		,RGB(255, 0, 255));

	if (!m_bTitleLogoUp){
	hMemDC = CBmpMgr::Get_Instance()->Find_Image("TitlePress");
	GdiTransparentBlt(_DC, 408, 575,
		184, 24
		, hMemDC
		, 0, 0
		, 184, 24
		, RGB(255, 0, 255));
	}

	if (m_bTitleLogoUp){
		CObjMgr::Get_Instance()->Render(_DC, _fdTime);
	}
}

void CTitleMenu::Release()
{
	CObjMgr::Get_Instance()->Delete_ID(OBJID::UI);
}

void CTitleMenu::Key_Check()
{
	if (!m_bTitleLogoUp) {
		for (int i = 0; i < 0xff; ++i)
		{
			if (i == VK_CONTROL || i == VK_LSHIFT || i == VK_RSHIFT || i == VK_SHIFT ||
				i == VK_CAPITAL || i == VK_ESCAPE || i == VK_SCROLL || i == VK_F1 || i == VK_F2)
				continue;
			if (KEY_DOWN(i))
			{
				m_bTitleLogoUp = true;
			}
		}
	}
}
