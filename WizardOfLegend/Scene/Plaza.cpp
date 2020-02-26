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
#include "../Obj/ArcanaCard.h"
#include "../Manager/SceneMgr.h"
// ���߿� �����ٰ�. �ö��ڿ� ���� ����.
#include "../Obj/Sandbag.h"
#include "../Obj/FireBoss.h"
#include "../Obj/Archer.h"
#include "../Obj/Swordman.h"

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

	// �÷��̾� ����
	CObj*	pObj = CAbstractFactory<CPlayer>::Create(2080.f, 2356.f);
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAYER, pObj);

	// �κ� ����
	CObjMgr::Get_Instance()->Add_Object(OBJID::INVENTORY,
		CAbstractFactory<CInventory>::Create());
	
	// �÷��̾� HPBAR, SKILLBAR ����
	CObjMgr::Get_Instance()->Add_Object(OBJID::STAGE_UI,
		CAbstractFactory<CPlayerHPBar>::Create());

	// �ڷ���Ŭ ����
	CObj* pTeleCircle = CAbstractFactory<CTeleCircle>::Create(2050.f, 1050.f);
	CObjMgr::Get_Instance()->Add_Object(OBJID::TELECIR, pTeleCircle);
	CObj* pTeleFButton = CAbstractFactory<CFButton>::Create(2050.f, 1035.f, OBJID::STAGE_UI);
	CObjMgr::Get_Instance()->Add_Object(OBJID::STAGE_UI, pTeleFButton);
	static_cast<CFAble*>(pTeleCircle)->Set_fButton(pTeleFButton);

	// ��ֹ�, ���ڷ��̼� ����

	// NPC ����


	// �Ƹ�ī�� ī�� ����
	////////// �̵��� ������ ��.
	CObj* pGaiaCard = CAbstractFactory<CArcanaCard>::Create(2528.f, 2040.f, "GaiaShieldCard");
	CObjMgr::Get_Instance()->Add_Object(OBJID::CARD, pGaiaCard);

	CObj* pGaiaFButton = CAbstractFactory<CFButton>::Create(2528.f, 2060.f, OBJID::STAGE_UI);
	CObjMgr::Get_Instance()->Add_Object(OBJID::STAGE_UI, pGaiaFButton);
	dynamic_cast<CFAble*>(pGaiaCard)->Set_fButton(pGaiaFButton);

	///////////////////////// ���߿� �����
	CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER,
		CAbstractFactory<CArcher>::Create(3181.f, 823.f));
	 CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER,
		CAbstractFactory<CSwordman>::Create(750.f, 662.f));

	// CObjMgr::Get_Instance()->Add_Object(OBJID::BOSS,
	//	CAbstractFactory<CFireBoss>::Create(2050.f, 892.f));

	return true;
}

//int CPlaza::Update(float _fdTime)
//{
//	
//
//	CTileMgr::Get_Instance()->Update(_fdTime);
//	CObjMgr::Get_Instance()->Update(_fdTime);
//	return 0;
//}

int CPlaza::Update(float _fdTime)
{
	CObjMgr::Get_Instance()->Update(_fdTime);
	if (CSceneMgr::SCENE_PLAZA == CSceneMgr::Get_Instance()->Get_Current_SceneID())
	{
		CTileMgr::Get_Instance()->Update(_fdTime);
	}
	return 0;
}

void CPlaza::Late_Update(float _fdTime)
{
	if (CSceneMgr::SCENE_PLAZA == CSceneMgr::Get_Instance()->Get_Current_SceneID())
	{
		CTileMgr::Get_Instance()->Late_Update(_fdTime);
		CObjMgr::Get_Instance()->Late_Update(_fdTime);
	}
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
	CObjMgr::Get_Instance()->Delete_ID(OBJID::MONSTER);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::P_CIRBULLET);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::P_RECTBULLET);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::M_CIRBULLET);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::M_RECTBULLET);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::BOSS);
	// F_ABLE����
	CObjMgr::Get_Instance()->Delete_ID(OBJID::NPC);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::TELECIR);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::CARD);
	// UI ����
	CObjMgr::Get_Instance()->Delete_ID(OBJID::STAGE_UI);

	// ��ֹ��� ����
	CBmpMgr::Get_Instance()->Delete_Bmp("PlazaTile");
}

void CPlaza::Key_Check()
{
}
