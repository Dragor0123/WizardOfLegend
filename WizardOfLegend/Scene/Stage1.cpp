#include "../stdafx.h"
#include "Stage1.h"
#include "../Manager/ScrollMgr.h"
#include "../MyBitmap/BmpMgr.h"
#include "../Obj/ObjMgr.h"
#include "../Obj/Player.h"
#include "../Manager/TileMgr.h"
#include "../Obj/Sandbag.h"

CStage1::CStage1()
{
}


CStage1::~CStage1()
{
	Release();
}

bool CStage1::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Tile/fireMapTile.bmp", "FireTile"))
		return false;

	CTileMgr::Get_Instance()->Load_Tile("FireTile");
	CObj*	pObj = CAbstractFactory<CPlayer>::Create(400.f, 400.f);
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAYER, pObj);

	//CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, CAbstractFactory<CSandbag>::Create(800.f, 650.f));

	return true;
}

int CStage1::Update(float _fdTime)
{
	CTileMgr::Get_Instance()->Update(_fdTime);
	CObjMgr::Get_Instance()->Update(_fdTime);
	return 0;
}

void CStage1::Late_Update(float _fdTime)
{
	CTileMgr::Get_Instance()->Late_Update(_fdTime);
	CObjMgr::Get_Instance()->Late_Update(_fdTime);
}

void CStage1::Collision(float _fdTime)
{
	CScene::Collision(_fdTime);
}

void CStage1::Render(HDC _DC, float _fdTime)
{
	//HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image("Background");
	//BitBlt(_DC, fScrollX, fScrollY, 1920, 1200, hMemDC, 0, 0, SRCCOPY);
	
	CTileMgr::Get_Instance()->Render(_DC, _fdTime);
	CObjMgr::Get_Instance()->Render(_DC, _fdTime);
}

void CStage1::Release()
{
	CObjMgr::Get_Instance()->Delete_ID(OBJID::MONSTER);
	//CObjMgr::Get_Instance()->Delete_ID(OBJID::P_BULLET);
	CTileMgr::Destroy_Instance();
}

void CStage1::Key_Check()
{
}
