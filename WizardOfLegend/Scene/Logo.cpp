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

void CLogo::Key_Check()
{
	if (KEY_DOWN(VK_RETURN) || KEY_DOWN(VK_LBUTTON)) {
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_MENU);
	}
}

bool CLogo::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Logo/Logo.bmp", "Logo"))
		return false;
	else
		return true;
}

int CLogo::Update(float _fdTime)
{
	return OBJ_NOEVENT;
}

void CLogo::Late_Update(float _fdTime)
{
	Key_Check();
}

void CLogo::Collision(float _fdTime)
{
}

void CLogo::Render(HDC _DC, float _fdTime)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image("Logo");
	BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
}

void CLogo::Release()
{
}
