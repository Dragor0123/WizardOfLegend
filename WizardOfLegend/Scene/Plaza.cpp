#include "../stdafx.h"
#include "Plaza.h"
#include "../Manager/TileMgr.h"
#include "../MyBitmap/BmpMgr.h"
#include "../Obj/Player.h"
#include "../Obj/ObjMgr.h"
#include "../Obj/TeleCircle.h"
#include "../Obj/FButton.h"
#include "../Obj/Inventory.h"
#include "../Obj/PlayerHPBar.h"
#include "../Obj/UISkillSet.h"
#include "../Obj/UIGold.h"
#include "../Manager/SceneMgr.h"
#include "../Obj/Gold.h"
#include "../Obj/OverDeco.h"
#include "../Obj/Wardrobe.h"
#include "../Obj/SpellSeller.h"
#include "../Obj/UnderDeco.h"
#include "../Obj/ArcanaCard.h"
// 나중에 없애줄것
#include "../Obj/Archer.h"

CPlaza::CPlaza()
{
}

CPlaza::~CPlaza()
{
	Release();
}

bool CPlaza::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Tile/plazaTile.bmp", "PlazaTile"))
		return false;

	if (!CTileMgr::Get_Instance()->Initialize("PlazaTile"))
		return false;
	CTileMgr::Get_Instance()->Load_Tile("PlazaTile");

	// 플레이어 삽입
	// 원위치 (2080.f, 2356.f )
	CObj*	pPlayer = CAbstractFactory<CPlayer>::Create(2080.f, 2356.f);
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAYER, pPlayer);
	
	// 인벤 삽입
	CObjMgr::Get_Instance()->Add_Object(OBJID::INVENTORY,
		CAbstractFactory<CInventory>::Create());
	
	// 플레이어 HPBAR, SKILLBAR 삽입, 돈 ui삽입
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAZA_UI,
		CAbstractFactory<CPlayerHPBar>::Create(pPlayer));
	
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAZA_UI,
		CAbstractFactory<CUISkillSet>::Create());
	
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAZA_UI,
		CAbstractFactory<CUIGold>::Create(pPlayer));

	// 텔레서클 삽입
	CObj* pTeleCircle = CAbstractFactory<CTeleCircle>::Create(2050.f, 1050.f);
	CObjMgr::Get_Instance()->Add_Object(OBJID::TELECIR, pTeleCircle);
	CObj* pTeleFButton = CAbstractFactory<CFButton>::Create(2050.f, 1035.f, OBJID::PLAZA_UI);
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAZA_UI, pTeleFButton);
	static_cast<CFAble*>(pTeleCircle)->Set_fButton(pTeleFButton);
	
	// 장애물, 데코레이션 삽입
	CObjMgr::Get_Instance()->Add_Object(OBJID::DECO,
		CAbstractFactory<COverDeco>::Create(2048.f, 1688.f, "PlazaArch"));
	
	// NPC 삽입
	CObjMgr::Get_Instance()->Add_Object(OBJID::DECO,
		CAbstractFactory<COverDeco>::Create(2621.f, 1874.f, "PurpleTent"));
	CObjMgr::Get_Instance()->Add_Object(OBJID::NPC,
		CAbstractFactory<CSpellSeller>::Create(2723.f, 1966.f, "SpellSeller"));
	CObjMgr::Get_Instance()->Add_Object(OBJID::DECO,
		CAbstractFactory<CUnderDeco>::Create(2621.f, 2100.f, "Carpet_0"));

	// Plaza에 넣을거 -> NormalAttackCard, DragonArcCard, IceSphereCard
	//// 아르카나 카드 삽입. Fbutton은 카드y의 -74.f
	CObj* pNormalAttCard = CAbstractFactory<CArcanaCard>::Create(2448.f, 2092.f, "NormalAttackCard");
	CObjMgr::Get_Instance()->Add_Object(OBJID::CARD, pNormalAttCard);
	CObj* pNormalAttFButton = CAbstractFactory<CFButton>::Create(2448.f, 2018.f, OBJID::PLAZA_UI);
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAZA_UI, pNormalAttFButton);
	dynamic_cast<CFAble*>(pNormalAttCard)->Set_fButton(pNormalAttFButton);

	CObj* pDragonArcCard = CAbstractFactory<CArcanaCard>::Create(2538.f, 2092.f, "DragonArcCard");
	CObjMgr::Get_Instance()->Add_Object(OBJID::CARD, pDragonArcCard);
	CObj* pDragonArcFButton = CAbstractFactory<CFButton>::Create(2538.f, 2018.f, OBJID::PLAZA_UI);
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAZA_UI, pDragonArcFButton);
	dynamic_cast<CFAble*>(pDragonArcCard)->Set_fButton(pDragonArcFButton);

	CObj* pIceSphereCard = CAbstractFactory<CArcanaCard>::Create(2628.f, 2092.f, "IceSphereCard");
	CObjMgr::Get_Instance()->Add_Object(OBJID::CARD, pIceSphereCard);
	CObj* pIceSphereFButton = CAbstractFactory<CFButton>::Create(2628.f, 2018.f, OBJID::PLAZA_UI);
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAZA_UI, pIceSphereFButton);
	dynamic_cast<CFAble*>(pIceSphereCard)->Set_fButton(pIceSphereFButton);

	//////////////////
	//CObj* pFireBallCard = CAbstractFactory<CArcanaCard>::Create(2718, 2092.f, "FireBallCard");
	//CObjMgr::Get_Instance()->Add_Object(OBJID::CARD, pFireBallCard);
	//CObj* pFireBallFButton = CAbstractFactory<CFButton>::Create(2718, 2018.f, OBJID::PLAZA_UI);
	//CObjMgr::Get_Instance()->Add_Object(OBJID::PLAZA_UI, pFireBallFButton);
	//dynamic_cast<CFAble*>(pFireBallCard)->Set_fButton(pFireBallFButton);

	//CObj* pEarthDrillCard = CAbstractFactory<CArcanaCard>::Create(2358.f, 2092.f, "EarthDrillCard");
	//CObjMgr::Get_Instance()->Add_Object(OBJID::CARD, pEarthDrillCard);
	//CObj* pEarthDrillFButton = CAbstractFactory<CFButton>::Create(2358.f, 2018.f, OBJID::PLAZA_UI);
	//CObjMgr::Get_Instance()->Add_Object(OBJID::PLAZA_UI, pEarthDrillFButton);
	//dynamic_cast<CFAble*>(pEarthDrillCard)->Set_fButton(pEarthDrillFButton);

	//CObj* pFrostFanCard = CAbstractFactory<CArcanaCard>::Create(2268.f, 2092.f, "FrostFanCard");
	//CObjMgr::Get_Instance()->Add_Object(OBJID::CARD, pFrostFanCard);
	//CObj* pFrostFanFButton = CAbstractFactory<CFButton>::Create(2268.f, 2018.f, OBJID::PLAZA_UI);
	//CObjMgr::Get_Instance()->Add_Object(OBJID::PLAZA_UI, pFrostFanFButton);
	//dynamic_cast<CFAble*>(pFrostFanCard)->Set_fButton(pFrostFanFButton);

	//CObj* pWindFalconCard = CAbstractFactory<CArcanaCard>::Create(2178.f, 2092.f, "WindFalconCard");
	//CObjMgr::Get_Instance()->Add_Object(OBJID::CARD, pWindFalconCard);
	//CObj* pFalconFButton = CAbstractFactory<CFButton>::Create(2178.f, 2018.f, OBJID::PLAZA_UI);
	//CObjMgr::Get_Instance()->Add_Object(OBJID::PLAZA_UI, pFalconFButton);
	//dynamic_cast<CFAble*>(pWindFalconCard)->Set_fButton(pFalconFButton);

	//// 옷장 NPC 추가
	CObjMgr::Get_Instance()->Add_Object(OBJID::NPC,
		CAbstractFactory<CWardrobe>::Create(3350.f, 528.f));

	// 몬스터 테스트.
	//ADD_OBJECT(OBJID::MONSTER,
	//	CAbstractFactory<CArcher>::Create(3495.f, 2269.f));
	//ADD_OBJECT(OBJID::MONSTER,
	//	CAbstractFactory<CArcher>::Create(3300.f, 2433.f));
	//ADD_OBJECT(OBJID::MONSTER,
	//	CAbstractFactory<CArcher>::Create(3382.f, 1837.f));
	//ADD_OBJECT(OBJID::MONSTER,
	//	CAbstractFactory<CArcher>::Create(3595.f, 2079.f));
	//ADD_OBJECT(OBJID::MONSTER,
	//	CAbstractFactory<CArcher>::Create(3648.f, 2430.f));

	return true;
}

int CPlaza::Update(float _fdTime)
{
	//
	//if (CSceneMgr::SCENE_PLAZA == CSceneMgr::Get_Instance()->Get_Scene_ID())
	//{
	//	
	//}
	CTileMgr::Get_Instance()->Update(_fdTime);
	CObjMgr::Get_Instance()->Update(_fdTime);
	return 0;
}

void CPlaza::Late_Update(float _fdTime)
{
	CTileMgr::Get_Instance()->Late_Update(_fdTime);
	CObjMgr::Get_Instance()->Late_Update(_fdTime);
}

void CPlaza::Collision(float _fdTime)
{
	CScene::Collision(_fdTime);
}

void CPlaza::Render(HDC _DC, float _fdTime)
{
	CTileMgr::Get_Instance()->Render(_DC, _fdTime);
	CObjMgr::Get_Instance()->Render(_DC, _fdTime);
}

void CPlaza::Release()
{
	for (int i = (int)OBJID::UNDERDECO; i <= (int)OBJID::EFFECT; ++i) {
		if ((OBJID::ID)i == OBJID::PLAYER)
			continue;
		CObjMgr::Get_Instance()->Delete_ID((OBJID::ID)i);
	}

	CObjMgr::Get_Instance()->Delete_ID(OBJID::DIGIT_UI);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::PLAZA_UI);
	
	CBmpMgr::Get_Instance()->Delete_Bmp("PlazaTile");
}

bool CPlaza::Key_Check()
{
	return true;
}


// 아르카나 카드 삽입
