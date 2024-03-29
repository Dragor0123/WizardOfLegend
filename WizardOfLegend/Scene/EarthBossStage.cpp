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
#include "../Obj/Prison.h"
#include "../Obj/ArcanaCard.h"
#include "../Obj/Potion.h"
#include "../Manager/SoundMgr.h"

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

	if (g_bBGM_On)
		CSoundMgr::Get_Instance()->PlayBGM(L"BossBgm.wav");

	auto& rPlayer = CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).front();
	rPlayer->Set_Pos(1601.f, 3134.f);

	// �÷��̾� HPBAR, SKILLBAR, �� UI ����
	CObjMgr::Get_Instance()->Add_Object(OBJID::EARTHBSTAGE_UI,
		CAbstractFactory<CPlayerHPBar>::Create(rPlayer));
	CObjMgr::Get_Instance()->Add_Object(OBJID::EARTHBSTAGE_UI,
		CAbstractFactory<CUISkillSet>::Create());
	CObjMgr::Get_Instance()->Add_Object(OBJID::EARTHBSTAGE_UI,
		CAbstractFactory<CUIGold>::Create(rPlayer));

	// �� ���� ����.
	CObjMgr::Get_Instance()->Add_Object(OBJID::BOSS,
		CAbstractFactory<CEarthLoad>::Create(1601.f, 968.f));

	return true;
}

int CEarthBossStage::Update(float _fdTime)
{
	CTileMgr::Get_Instance()->Update(_fdTime);

	auto& rPlayer = CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).front();
	CObj* pEarthBoss = nullptr;
	if (!CObjMgr::Get_Instance()->Get_listObj(OBJID::BOSS).empty())
		pEarthBoss = CObjMgr::Get_Instance()->Get_listObj(OBJID::BOSS).front();

	if (rPlayer->Get_HitRect().bottom < 1540.f && !m_bBossHPBarInserted)
	{
		CObjMgr::Get_Instance()->Add_Object(OBJID::EARTHBSTAGE_UI,
			CAbstractFactory<CBossHPBar>::Create(pEarthBoss));

		CObj* pPrison1 = CAbstractFactory<CPrison>::Create(1504.f, 1628.f, 0.f);
		CObj* pPrison2 = CAbstractFactory<CPrison>::Create(1696.f, 1628.f, 0.f);
		CObjMgr::Get_Instance()->Add_Object(OBJID::OBSTACLE, pPrison1);
		CObjMgr::Get_Instance()->Add_Object(OBJID::OBSTACLE, pPrison2);

		m_bBossHPBarInserted = true;
	}

	if (m_bBossHPBarInserted && !pEarthBoss)
	{
		if (!m_bTeleCircleInserted)
		{
			CObj* pTeleCircle = CAbstractFactory<CTeleCircle>::Create(1601.f, 968.f);
			CObjMgr::Get_Instance()->Add_Object(OBJID::TELECIR, pTeleCircle);
			CObj* pTeleFButton = CAbstractFactory<CFButton>::Create(1601.f, 968.f, OBJID::EARTHBSTAGE_UI);
			CObjMgr::Get_Instance()->Add_Object(OBJID::EARTHBSTAGE_UI, pTeleFButton);
			static_cast<CFAble*>(pTeleCircle)->Set_fButton(pTeleFButton);

			CObj* pEarthDrillCard = CAbstractFactory<CArcanaCard>::Create(1555.f, 680.f, "EarthDrillCard");
			CObjMgr::Get_Instance()->Add_Object(OBJID::CARD, pEarthDrillCard);
			CObj* pEarthDrillFButton = CAbstractFactory<CFButton>::Create(1555.f, 606.f, OBJID::EARTHBSTAGE_UI);
			CObjMgr::Get_Instance()->Add_Object(OBJID::EARTHBSTAGE_UI, pEarthDrillFButton);
			dynamic_cast<CFAble*>(pEarthDrillCard)->Set_fButton(pEarthDrillFButton);

			CObj* pPotion = CAbstractFactory<CPotion>::Create(1645.f, 680.f, "HPPotion");
			CObjMgr::Get_Instance()->Add_Object(OBJID::CARD, pPotion);
			CObj* pPotionFButton = CAbstractFactory<CFButton>::Create(1645.f, 620.f, OBJID::EARTHBSTAGE_UI);
			CObjMgr::Get_Instance()->Add_Object(OBJID::EARTHBSTAGE_UI, pPotionFButton);
			dynamic_cast<CFAble*>(pPotion)->Set_fButton(pPotionFButton);

			STOP_SOUND(CSoundMgr::BGM);
			CSoundMgr::Get_Instance()->PlayBGM_NOLOOP(L"WIN_Bgm.wav");
			m_bTeleCircleInserted = true;
		}
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
