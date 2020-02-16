#include "../stdafx.h"
#include "Logo.h"
#include "../Manager/KeyMgr.h"
#include "../Manager/SceneMgr.h"
#include "../MyBitmap/BmpMgr.h"

CLogo::CLogo()
{
}


CLogo::~CLogo()
{
}

bool CLogo::Initialize()
{
	if (CBmpMgr::Get_Instance()->Load_Bitmap("Logo", L"Logo\\Logo.bmp", BITMAP_PATH))
		return true;
	else
		return false;
}

void CLogo::Input(float _fdTime)
{
	if (KEY_DOWN(VK_RETURN))
	{
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_MENU);
	}
}

int CLogo::Update(float _fdTime)
{
	return 0;
}

void CLogo::Late_Update(float _fdTime)
{
}

void CLogo::Render(HDC _DC, float _fdTime)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Get_Bitmap_DC("Logo");
	if (hMemDC != NULL)
		BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
}

void CLogo::Release()
{
}
