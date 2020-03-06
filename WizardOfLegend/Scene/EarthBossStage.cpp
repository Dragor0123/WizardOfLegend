#include "../stdafx.h"
#include "EarthBossStage.h"
#include "../MyBitmap/BmpMgr.h"
#include "../Obj/ObjMgr.h"
#include "../Obj/Player.h"
#include "../Manager/TileMgr.h"
#include "../Manager/SceneMgr.h"
#include "../Obj/FButton.h"
#include "../Obj/Prison.h"
#include "../Obj/TeleCircle.h"
#include "../Obj/Inventory.h"
#include "../Obj/PlayerHPBar.h"
#include "../Obj/UIGold.h"
#include "../Obj/EarthLoad.h"
#include "../Obj/BossHPBar.h"

CEarthBossStage::CEarthBossStage()
	: m_bTeleCircleInserted(false), m_bBossHPBarInserted(false)
{
}


CEarthBossStage::~CEarthBossStage()
{
	Release();
}

bool CEarthBossStage::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Tile/plazaTile.bmp", "PlazaTile"))
		return false;

	if (!CTileMgr::Get_Instance()->Initialize("EarthBossTile"))
		return false;
	CTileMgr::Get_Instance()->Load_Tile("EarthBossTile");

	auto& rPlayer = CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).front();
	rPlayer->Set_Pos(1601.f, 3134.f);
	// ÇÃ·¹ÀÌ¾î HPBAR, SKILLBAR, µ· UI »ðÀÔ
	CObjMgr::Get_Instance()->Add_Object(OBJID::EARTHBSTAGE_UI,
		CAbstractFactory<CPlayerHPBar>::Create(rPlayer));
	CObjMgr::Get_Instance()->Add_Object(OBJID::EARTHBSTAGE_UI,
		CAbstractFactory<CUISkillSet>::Create());
	CObjMgr::Get_Instance()->Add_Object(OBJID::EARTHBSTAGE_UI,
		CAbstractFactory<CUIGold>::Create(rPlayer));

	// ¶¥ º¸½º »ðÀÔ.
	CObjMgr::Get_Instance()->Add_Object(OBJID::BOSS,
		CAbstractFactory<CEarthLoad>::Create(1601.f, 968.f));

	return true;
}

int CEarthBossStage::Update(float _fdTime)
{
	CTileMgr::Get_Instance()->Update(_fdTime);

	auto& rPlayer = CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).front();

	if (rPlayer->Get_HitRect().bottom < 1540.f && !m_bBossHPBarInserted)
	{
		auto& rEarthBoss = CObjMgr::Get_Instance()->Get_listObj(OBJID::BOSS).front();
		CObjMgr::Get_Instance()->Add_Object(OBJID::EARTHBSTAGE_UI,
			CAbstractFactory<CBossHPBar>::Create(rEarthBoss));
		m_bBossHPBarInserted = true;
	}

	CObjMgr::Get_Instance()->Update(_fdTime);
	return OBJ_NOEVENT;
}

void CEarthBossStage::Late_Update(float _fdTime)
{
	CTileMgr::Get_Instance()->Late_Update(_fdTime);
	CObjMgr::Get_Instance()->Late_Update(_fdTime);
}

void CEarthBossStage::Collision(float _fdTime)
{
	CScene::Collision(_fdTime);
}

void CEarthBossStage::Render(HDC _DC, float _fdTime)
{
	CTileMgr::Get_Instance()->Render(_DC, _fdTime);
	CObjMgr::Get_Instance()->Render(_DC, _fdTime);
}

void CEarthBossStage::Release()
{
	for (int i = (int)OBJID::UNDERDECO; i <= (int)OBJID::EFFECT; ++i) {
		if ((OBJID::ID)i == OBJID::PLAYER)
			continue;
		CObjMgr::Get_Instance()->Delete_ID((OBJID::ID)i);
	}

	CObjMgr::Get_Instance()->Delete_ID(OBJID::DIGIT_UI);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::EARTHBSTAGE_UI);
	CBmpMgr::Get_Instance()->Delete_Bmp("PlazaTile");
}

bool CEarthBossStage::Key_Check()
{
	return true;
}
