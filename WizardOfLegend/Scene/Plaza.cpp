#include "../stdafx.h"
#include "Plaza.h"
#include "../Manager/TileMgr.h"
#include "../MyBitmap/BmpMgr.h"
#include "../Obj/Player.h"
#include "../Obj/ObjMgr.h"
#include "../Obj/TeleCircle.h"
#include "../Obj/FButton.h"
// 나중에 없애줄것. 플라자엔 몬스터 없음.
#include "../Obj/Archer.h"
#include "../Obj/Swordman.h"
#include "../Obj/EarthLoad.h"

CPlaza::CPlaza()
{
}


CPlaza::~CPlaza()
{
	Release();
}

bool CPlaza::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Tile/plazaTile.bmp", "PlazaTile")) {
		return false;
	}

	if (!CTileMgr::Get_Instance()->Initialize("PlazaTile"))
		return false;
	CTileMgr::Get_Instance()->Load_Tile("PlazaTile");

	CObj*	pObj = CAbstractFactory<CPlayer>::Create(2080.f, 2356.f);
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAYER, pObj);

	CObj* pTeleCircle = CAbstractFactory<CTeleCircle>::Create(2050.f, 1050.f);
	CObjMgr::Get_Instance()->Add_Object(OBJID::TELECIR, pTeleCircle);

	CObj* pTeleFButton = CAbstractFactory<CFButton>::Create(2050.f, 1035.f);
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAZA_UI, pTeleFButton);
	dynamic_cast<CFAble*>(pTeleCircle)->Set_fButton(pTeleFButton);

	//CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER,
	//	CAbstractFactory<CArcher>::Create(732.f, 738.f));
	
	CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER,
		CAbstractFactory<CEarthLoad>::Create(2058.f, 641.f));

	return true;
}

int CPlaza::Update(float _fdTime)
{
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
	CObjMgr::Get_Instance()->Delete_ID(OBJID::MONSTER);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::PLAZA_UI);
	// NPC들 제거
	CObjMgr::Get_Instance()->Delete_ID(OBJID::NPC);
	
	CObjMgr::Get_Instance()->Delete_ID(OBJID::TELECIR);
	// 장애물들 제거
	//

	CBmpMgr::Get_Instance()->Delete_Bmp("PlazaTile");
}

void CPlaza::Key_Check()
{
}
