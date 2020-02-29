#include "../stdafx.h"
#include "TeleCircle.h"
#include "../MyBitmap/BmpMgr.h"
#include "../Manager/SceneMgr.h"

CTeleCircle::CTeleCircle()
{
}


CTeleCircle::~CTeleCircle()
{
	Release();
}

bool CTeleCircle::Initialize()
{
	if (!CFAble::Initialize())
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Fable/TeleCircle.bmp", "TeleCircle"))
		return false;

	m_tInfo.iCX = 256;
	m_tInfo.iCY = 256;
	Equalize_HitSzInfoSz();

	m_eRenderGroupID = GROUPID::BACKGROUND;
	return true;
}

int CTeleCircle::Update(float _fdTime)
{
	if (OBJ_DEAD == CFAble::Update(_fdTime))
		return OBJ_DEAD;

	Update_Rect();
	Update_HitRect();

	return OBJ_NOEVENT;
}

void CTeleCircle::Late_Update(float _fdTime)
{
	CFAble::Key_Check(_fdTime);
}

void CTeleCircle::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	Update_Rect();
	Update_HitRect();
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image("TeleCircle");
	GdiTransparentBlt(_DC
		, (int)(m_tRect.left + _fScrollX)
		, (int)(m_tRect.top + _fScrollY)
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC, 0, 0
		, m_tInfo.iCX, m_tInfo.iCY
		, MAGENTA_COLOR);

	//히트박스 조건 걸자
	Draw_HitBox(_DC, _fScrollX, _fScrollY);
}

void CTeleCircle::Release()
{
	// CFAble::Release();
}

void CTeleCircle::Do_FButton_Action(float _fdTime)
{
	if (CSceneMgr::Get_Instance()->Get_Scene_ID() == CSceneMgr::SCENE_PLAZA) {
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_STAGE1);
		return;
	}

	if (CSceneMgr::Get_Instance()->Get_Scene_ID() == CSceneMgr::SCENE_STAGE1) {
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_FIREBOSS);
		return;
	}
}
