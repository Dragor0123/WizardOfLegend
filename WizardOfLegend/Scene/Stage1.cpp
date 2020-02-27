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
/////////////
#include "../Obj/Archer.h"
#include "../Obj/Swordman.h"
#include "../Obj/IcicleEffect.h"
#include "../Obj/SummonCard.h"
#include "../Obj/Summoner.h"
#include "../Obj/SummonerBall.h"
#include "../Obj/Prison.h"

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
	// 2664, 1535 중앙자리
	rPlayer->Set_Pos(1055.f, 3203.f);

	// 1번방
	CObjMgr::Get_Instance()->Add_Object(OBJID::OBSTACLE,
		CAbstractFactory<CPrison>::Create(922.f, 610.f, 90.f));

	// 2번방 (1번방 바로 아래)
	CObjMgr::Get_Instance()->Add_Object(OBJID::OBSTACLE,
		CAbstractFactory<CPrison>::Create(1057.f, 1760.f, 90.f));

	//3번방 (맨 위 2개)
	CObjMgr::Get_Instance()->Add_Object(OBJID::OBSTACLE,
		CAbstractFactory<CPrison>::Create(2657.f, 480.f, 0.f));
	CObjMgr::Get_Instance()->Add_Object(OBJID::OBSTACLE,
		CAbstractFactory<CPrison>::Create(3232.f, 223.f, 90.f));

	// 4번방 (오른쪽 큰 방)
	CObjMgr::Get_Instance()->Add_Object(OBJID::OBSTACLE,
		CAbstractFactory<CPrison>::Create(5278.f, 3291.f, 0.f));

	// 5번방 (미니보스방으로 쓰려고했던)
	CObjMgr::Get_Instance()->Add_Object(OBJID::OBSTACLE,
		CAbstractFactory<CPrison>::Create(2783.f, 4203.f, 0.f));

	// 마지막 방 (2개)
	CObjMgr::Get_Instance()->Add_Object(OBJID::OBSTACLE,
		CAbstractFactory<CPrison>::Create(1503.f, 2976.f, 90.f));
	CObjMgr::Get_Instance()->Add_Object(OBJID::OBSTACLE,
		CAbstractFactory<CPrison>::Create(608.f, 3483.f, 0.f));

	CObjMgr::Get_Instance()->Add_Object(OBJID::SUMMONCARD,
		CAbstractFactory<CSummonCard>::Create(729.f, 3033.f, "SUMMONERBALL_SCARD"));

	return true;
}

//CObjMgr::Get_Instance()->Add_Object(OBJID::SUMMONCARD,
//	CAbstractFactory<CSummonCard>::Create(2022.f, 2093.f, "SUMMONER_SCARD"));

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
