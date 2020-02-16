#include "../stdafx.h"
#include "TitleMenu.h"
#include "../Manager/KeyMgr.h"
#include "../Manager/SceneMgr.h"
#include "../MyBitmap/BmpMgr.h"

CTitleMenu::CTitleMenu()
{
}


CTitleMenu::~CTitleMenu()
{
}

bool CTitleMenu::Initialize()
{
	if (CBmpMgr::Get_Instance()->Load_Bitmap("TitleMenu", L"Menu\\Menu.bmp", BITMAP_PATH))
		return true;
	else
		return false;
}

void CTitleMenu::Input(float _fdTime)
{
}

int CTitleMenu::Update(float _fdTime)
{
	return 0;
}

void CTitleMenu::Late_Update(float _fdTime)
{
}

void CTitleMenu::Render(HDC _DC, float _fdTime)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Get_Bitmap_DC("TitleMenu");
	if (hMemDC != NULL)
		BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
}

void CTitleMenu::Release()
{
}
