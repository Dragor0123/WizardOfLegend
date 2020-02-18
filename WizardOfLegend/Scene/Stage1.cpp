#include "../stdafx.h"
#include "Stage1.h"
#include "../Manager/ScrollMgr.h"
#include "../MyBitmap/BmpMgr.h"
#include "../Obj/ObjMgr.h"
#include "../Obj/Player.h"

CStage1::CStage1()
{
}


CStage1::~CStage1()
{
	Release();
}

bool CStage1::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Stage/Background.bmp", "Background"))
		return false;

	CObj*	pObj = CAbstractFactory<CPlayer>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAYER, pObj);
}

int CStage1::Update(float _fdTime)
{
	CObjMgr::Get_Instance()->Update(_fdTime);
	return 0;
}

void CStage1::Late_Update(float _fdTime)
{
	CObjMgr::Get_Instance()->Late_Update(_fdTime);
}

void CStage1::Collision(float _fdTime)
{
}

void CStage1::Render(HDC _DC, float _fdTime)
{
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image("Background");
	BitBlt(_DC, fScrollX, fScrollY, 1920, 1200, hMemDC, 0, 0, SRCCOPY);

	CObjMgr::Get_Instance()->Render(_DC, _fdTime);
}

void CStage1::Release()
{
	CObjMgr::Get_Instance()->Delete_ID(OBJID::MONSTER);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::BULLET);
}

void CStage1::Key_Check()
{
}
