#include "../stdafx.h"
#include "Plaza.h"
#include "../Manager/TileMgr.h"
#include "../MyBitmap/BmpMgr.h"
#include "../Obj/Player.h"
#include "../Obj/ObjMgr.h"
#include "../Obj/TeleCircle.h"
#include "../Obj/FButton.h"
#include "../Obj/MyButton.h"

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

	CTileMgr::Get_Instance()->Load_Tile("PlazaTile");

	CObj*	pObj = CAbstractFactory<CPlayer>::Create(500.f, 600.f);
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAYER, pObj);

	CObj* pTeleCircle = CAbstractFactory<CTeleCircle>::Create(1800.f, 2380.f);
	CObjMgr::Get_Instance()->Add_Object(OBJID::FABLE, pTeleCircle);

	CObj* pTeleFButton = CAbstractFactory<CFButton>::Create(1800.f, 2380.f);
	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, pTeleFButton);
	dynamic_cast<CFAble*>(pTeleCircle)->Set_fButton(pTeleFButton);
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
	CObjMgr::Get_Instance()->Delete_ID(OBJID::FABLE);
	CTileMgr::Destroy_Instance();
}

void CPlaza::Key_Check()
{
}
