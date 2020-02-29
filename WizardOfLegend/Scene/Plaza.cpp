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
#include "../Obj/ArcanaCard.h"
#include "../Manager/SceneMgr.h"
#include "../Obj/Gold.h"
#include "../Obj/OverDeco.h"
#include "../Obj/BossHPBar.h"

// ���߿� �����ٰ�. �ö��ڿ� ���� ����.


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
	// ����ġ (2080.f, 2356.f )
	CObj*	pPlayer = CAbstractFactory<CPlayer>::Create(2080.f, 2356.f);
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAYER, pPlayer);

	// �κ� ����
	CObjMgr::Get_Instance()->Add_Object(OBJID::INVENTORY,
		CAbstractFactory<CInventory>::Create());
	
	// �÷��̾� HPBAR, SKILLBAR ����, �� ui����
	CObjMgr::Get_Instance()->Add_Object(OBJID::STAGE_UI,
		CAbstractFactory<CPlayerHPBar>::Create(pPlayer));

	CObjMgr::Get_Instance()->Add_Object(OBJID::STAGE_UI,
		CAbstractFactory<CUISkillSet>::Create());

	CObjMgr::Get_Instance()->Add_Object(OBJID::STAGE_UI,
		CAbstractFactory<CUIGold>::Create(pPlayer));

	// �ڷ���Ŭ ����
	CObj* pTeleCircle = CAbstractFactory<CTeleCircle>::Create(2050.f, 1050.f);
	CObjMgr::Get_Instance()->Add_Object(OBJID::TELECIR, pTeleCircle);
	CObj* pTeleFButton = CAbstractFactory<CFButton>::Create(2050.f, 1035.f, OBJID::STAGE_UI);
	CObjMgr::Get_Instance()->Add_Object(OBJID::STAGE_UI, pTeleFButton);
	static_cast<CFAble*>(pTeleCircle)->Set_fButton(pTeleFButton);
	// ��ֹ�, ���ڷ��̼� ����

	CObjMgr::Get_Instance()->Add_Object(OBJID::OBSTACLE,
		CAbstractFactory<COverDeco>::Create(2048.f, 1688.f, string("PlazaArch")));
	// NPC ����

	// �Ƹ�ī�� ī�� ���� �̵��� ������ ��. // GaiaShieldCard

	CObj* pGaiaCard = CAbstractFactory<CArcanaCard>::Create(2528.f, 2040.f, "GaiaShieldCard");
	CObjMgr::Get_Instance()->Add_Object(OBJID::CARD, pGaiaCard);
	CObj* pGaiaFButton = CAbstractFactory<CFButton>::Create(2528.f, 2060.f, OBJID::STAGE_UI);
	CObjMgr::Get_Instance()->Add_Object(OBJID::STAGE_UI, pGaiaFButton);
	dynamic_cast<CFAble*>(pGaiaCard)->Set_fButton(pGaiaFButton);

	CObj* pIceSphereCard = CAbstractFactory<CArcanaCard>::Create(2448.f, 2040.f, "IceSphereCard");
	CObjMgr::Get_Instance()->Add_Object(OBJID::CARD, pIceSphereCard);
	CObj* pIceSphereFButton = CAbstractFactory<CFButton>::Create(2448.f, 2060.f, OBJID::STAGE_UI);
	CObjMgr::Get_Instance()->Add_Object(OBJID::STAGE_UI, pIceSphereFButton);
	dynamic_cast<CFAble*>(pIceSphereCard)->Set_fButton(pIceSphereFButton);

	CObj* pFrostFanCard = CAbstractFactory<CArcanaCard>::Create(2368.f, 2040.f, "FrostFanCard");
	CObjMgr::Get_Instance()->Add_Object(OBJID::CARD, pFrostFanCard);
	CObj* pFrostFanFButton = CAbstractFactory<CFButton>::Create(2368.f, 2060.f, OBJID::STAGE_UI);
	CObjMgr::Get_Instance()->Add_Object(OBJID::STAGE_UI, pFrostFanFButton);
	dynamic_cast<CFAble*>(pFrostFanCard)->Set_fButton(pFrostFanFButton);

	return true;
}

int CPlaza::Update(float _fdTime)
{
	CObjMgr::Get_Instance()->Update(_fdTime);
	if (CSceneMgr::SCENE_PLAZA == CSceneMgr::Get_Instance()->Get_Scene_ID())
	{
		CTileMgr::Get_Instance()->Update(_fdTime);
	}
	return 0;
}

void CPlaza::Late_Update(float _fdTime)
{
	if (CSceneMgr::SCENE_PLAZA == CSceneMgr::Get_Instance()->Get_Scene_ID())
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
	CObjMgr::Get_Instance()->Delete_ID(OBJID::P_CIRBULLET);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::P_RECTBULLET);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::P_SHIELD);

	// F_ABLE����
	CObjMgr::Get_Instance()->Delete_ID(OBJID::NPC);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::TELECIR);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::CARD);

	// UI ����
	CObjMgr::Get_Instance()->Delete_ID(OBJID::STAGE_UI);

	// ��ֹ��� ����
	CObjMgr::Get_Instance()->Delete_ID(OBJID::OBSTACLE);
	CBmpMgr::Get_Instance()->Delete_Bmp("PlazaTile");
}

bool CPlaza::Key_Check()
{
	return true;
}
