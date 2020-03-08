#include "../stdafx.h"
#include "TeleCircle.h"
#include "../MyBitmap/BmpMgr.h"
#include "../Manager/SceneMgr.h"
#include "TeleEffect.h"
#include "ObjMgr.h"
#include "Player.h"
#include "../Manager/SoundMgr.h"

CTeleCircle::CTeleCircle()
	: m_bEffectEnd(false)
{
}


CTeleCircle::~CTeleCircle()
{
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

	if (m_bEffectEnd)
		Change_Scene();
}

void CTeleCircle::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
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

}

void CTeleCircle::Do_FButton_Action(float _fdTime)
{
	CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT,
		CAbstractFactory<CTeleEffect>::Create(this));
	STOP_SOUND(CSoundMgr::EFFECT);
	PLAY_SOUND(L"ExitPortal.wav", CSoundMgr::EFFECT);

	if (!CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).empty())
	{
		CObj* pPlayer = CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).front();
		static_cast<CPlayer*>(pPlayer)->Set_DontDraw(true);
	}
}

void CTeleCircle::Change_Scene()
{
	if (!CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).empty())
	{
		CObj* pPlayer = CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).front();
		static_cast<CPlayer*>(pPlayer)->Set_DontDraw(false);
	}

	if (CSceneMgr::Get_Instance()->Get_Scene_ID() == CSceneMgr::SCENE_PLAZA) {
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_STAGE1);
		return;
	}

	if (CSceneMgr::Get_Instance()->Get_Scene_ID() == CSceneMgr::SCENE_STAGE1) {
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_EARTHBOSS);
		return;
	}

	if (CSceneMgr::Get_Instance()->Get_Scene_ID() == CSceneMgr::SCENE_EARTHBOSS) {
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_FIREBOSS);
		return;
	}
}
