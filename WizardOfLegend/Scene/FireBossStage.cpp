#include "../stdafx.h"
#include "FireBossStage.h"
#include "../MyBitmap/BmpMgr.h"
#include "../Obj/ObjMgr.h"
#include "../Obj/Player.h"
#include "../Manager/TileMgr.h"
#include "../Manager/SceneMgr.h"
#include "../Obj/FButton.h"
#include "../Obj/IcicleEffect.h"
#include "../Obj/Prison.h"
#include "../Obj/TeleCircle.h"
#include "../Obj/Inventory.h"
#include "../Obj/PlayerHPBar.h"
#include "../Obj/UIGold.h"
#include "../Obj/FireBoss.h"

CFireBossStage::CFireBossStage()
	: m_bTeleCircleInserted(false)
{
}


CFireBossStage::~CFireBossStage()
{
	Release();
}

bool CFireBossStage::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Tile/fireMapTile.bmp", "FireTile"))
		return false;
	if (!CTileMgr::Get_Instance()->Initialize("FireBossTile"))
		return false;
	CTileMgr::Get_Instance()->Load_Tile("FireBossTile");

	auto& rPlayer = CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).front();
	rPlayer->Set_Pos(1601.f, 3344.f);
	// 플레이어 HPBAR, SKILLBAR, 돈 UI 삽입
	CObjMgr::Get_Instance()->Add_Object(OBJID::STAGE_UI,
		CAbstractFactory<CPlayerHPBar>::Create(rPlayer));
	CObjMgr::Get_Instance()->Add_Object(OBJID::STAGE_UI,
		CAbstractFactory<CUISkillSet>::Create());
	CObjMgr::Get_Instance()->Add_Object(OBJID::STAGE_UI,
		CAbstractFactory<CUIGold>::Create(rPlayer));

	// 파이어 보스 삽입, HPBAR도 삽입해야한다. 그리고 이따가 이거 update쪽으로 바꾸자.
	CObjMgr::Get_Instance()->Add_Object(OBJID::BOSS,
		CAbstractFactory<CFireBoss>::Create(1601.f, 988.f));

	return true;
}

int CFireBossStage::Update(float _fdTime)
{
	CObjMgr::Get_Instance()->Update(_fdTime);
	if (CSceneMgr::SCENE_FIREBOSS == CSceneMgr::Get_Instance()->Get_Scene_ID())
	{
		CTileMgr::Get_Instance()->Update(_fdTime);
		// auto& rPlayer = CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).front();
	}

	return OBJ_NOEVENT;
}

void CFireBossStage::Late_Update(float _fdTime)
{
	if (CSceneMgr::SCENE_FIREBOSS == CSceneMgr::Get_Instance()->Get_Scene_ID())
	{
		CTileMgr::Get_Instance()->Late_Update(_fdTime);
		CObjMgr::Get_Instance()->Late_Update(_fdTime);
	}
}

void CFireBossStage::Collision(float _fdTime)
{
	CScene::Collision(_fdTime);
}

void CFireBossStage::Render(HDC _DC, float _fdTime)
{
	CTileMgr::Get_Instance()->Render(_DC, _fdTime);
	CObjMgr::Get_Instance()->Render(_DC, _fdTime);
}

void CFireBossStage::Release()
{
	for (int i = (int)OBJID::UNDERDECO; i <= (int)OBJID::EFFECT; ++i) {
		if ((OBJID::ID)i == OBJID::PLAYER)
			continue;
		CObjMgr::Get_Instance()->Delete_ID((OBJID::ID)i);
	}

	CObjMgr::Get_Instance()->Delete_ID(OBJID::STAGE_UI);
	CBmpMgr::Get_Instance()->Delete_Bmp("FireTile");
}

bool CFireBossStage::Key_Check()
{
	return true;
}
