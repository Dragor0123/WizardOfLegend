#include "../stdafx.h"
#include "FireBossStage.h"
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
#include "../Obj/FireBoss.h"
#include "../Obj/BossHPBar.h"
#include "../Obj/Prison.h"
#include "../Manager/SoundMgr.h"

CFireBossStage::CFireBossStage()
	: m_bTeleCircleInserted(false), m_bBossHPBarInserted(false)
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

	if (g_bBGM_On)
		CSoundMgr::Get_Instance()->PlayBGM(L"BossBgm.wav");

	auto& rPlayer = CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).front();
	rPlayer->Set_Pos(1601.f, 3344.f);
	// ÇÃ·¹ÀÌ¾î HPBAR, SKILLBAR, µ· UI »ðÀÔ
	CObjMgr::Get_Instance()->Add_Object(OBJID::FIREBSTAGE_UI,
		CAbstractFactory<CPlayerHPBar>::Create(rPlayer));
	CObjMgr::Get_Instance()->Add_Object(OBJID::FIREBSTAGE_UI,
		CAbstractFactory<CUISkillSet>::Create());
	CObjMgr::Get_Instance()->Add_Object(OBJID::FIREBSTAGE_UI,
		CAbstractFactory<CUIGold>::Create(rPlayer));

	CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER,
		CAbstractFactory<CFireBoss>::Create(1601.f, 968.f));

	return true;
}

int CFireBossStage::Update(float _fdTime)
{
	CTileMgr::Get_Instance()->Update(_fdTime);

	auto& rPlayer = CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).front();
	CObj* pFireBoss = nullptr;
	if (!CObjMgr::Get_Instance()->Get_listObj(OBJID::MONSTER).empty())
		pFireBoss = CObjMgr::Get_Instance()->Get_listObj(OBJID::MONSTER).front();

	if (rPlayer->Get_HitRect().bottom < 1540.f && !m_bBossHPBarInserted)
	{
		CObjMgr::Get_Instance()->Add_Object(OBJID::FIREBSTAGE_UI,
			CAbstractFactory<CBossHPBar>::Create(pFireBoss));

		CObj* pPrison1 = CAbstractFactory<CPrison>::Create(1504.f, 1628.f, 0.f);
		CObj* pPrison2 = CAbstractFactory<CPrison>::Create(1696.f, 1628.f, 0.f);
		CObjMgr::Get_Instance()->Add_Object(OBJID::OBSTACLE, pPrison1);
		CObjMgr::Get_Instance()->Add_Object(OBJID::OBSTACLE, pPrison2);

		m_bBossHPBarInserted = true;
	}

	if (m_bBossHPBarInserted && !pFireBoss)
	{
		if (!m_bTeleCircleInserted)
		{
			CObj* pTeleCircle = CAbstractFactory<CTeleCircle>::Create(1601.f, 968.f);
			CObjMgr::Get_Instance()->Add_Object(OBJID::TELECIR, pTeleCircle);
			CObj* pTeleFButton = CAbstractFactory<CFButton>::Create(1601.f, 968.f, OBJID::FIREBSTAGE_UI);
			CObjMgr::Get_Instance()->Add_Object(OBJID::FIREBSTAGE_UI, pTeleFButton);
			static_cast<CFAble*>(pTeleCircle)->Set_fButton(pTeleFButton);

			m_bTeleCircleInserted = true;
		}
	}

	CObjMgr::Get_Instance()->Update(_fdTime);
	return OBJ_NOEVENT;
}

void CFireBossStage::Late_Update(float _fdTime)
{
	CTileMgr::Get_Instance()->Late_Update(_fdTime);
	CObjMgr::Get_Instance()->Late_Update(_fdTime);
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

	CObjMgr::Get_Instance()->Delete_ID(OBJID::DIGIT_UI);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::FIREBSTAGE_UI);
	CBmpMgr::Get_Instance()->Delete_Bmp("FireTile");
}

bool CFireBossStage::Key_Check()
{
	return true;
}
