#include "../stdafx.h"
#include "Stage1.h"
#include "../Manager/ScrollMgr.h"
#include "../MyBitmap/BmpMgr.h"
#include "../Obj/ObjMgr.h"
#include "../Obj/Player.h"
#include "../Manager/TileMgr.h"
#include "../Obj/Sandbag.h"
#include "../Obj/EarthLoad.h"
#include "../Manager/SceneMgr.h"
#include "../Obj/FButton.h"

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

	if (!CTileMgr::Get_Instance()->Initialize("FireTile"))
		return false;
	CTileMgr::Get_Instance()->Load_Tile("FireTile");
	auto& rPlayer = CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).front();
	rPlayer->Set_Pos(2664, 1535);

	return true;
}

int CStage1::Update(float _fdTime)
{
	CObjMgr::Get_Instance()->Update(_fdTime);
	if (CSceneMgr::SCENE_STAGE1 == CSceneMgr::Get_Instance()->Get_Current_SceneID())
	{
		CTileMgr::Get_Instance()->Update(_fdTime);
	}
	return 0;
}

void CStage1::Late_Update(float _fdTime)
{
	if (CSceneMgr::SCENE_STAGE1 == CSceneMgr::Get_Instance()->Get_Current_SceneID())
	{
		CTileMgr::Get_Instance()->Late_Update(_fdTime);
		CObjMgr::Get_Instance()->Late_Update(_fdTime);
	}
}

void CStage1::Collision(float _fdTime)
{
	CScene::Collision(_fdTime);
}

void CStage1::Render(HDC _DC, float _fdTime)
{	
	CTileMgr::Get_Instance()->Render(_DC, _fdTime);
	CObjMgr::Get_Instance()->Render(_DC, _fdTime);
}

void CStage1::Release()
{
	CObjMgr::Get_Instance()->Delete_ID(OBJID::MONSTER);
	//CObjMgr::Get_Instance()->Delete_ID(OBJID::TELECIR);
	CBmpMgr::Get_Instance()->Delete_Bmp("FireTile");
}

void CStage1::Key_Check()
{
}
