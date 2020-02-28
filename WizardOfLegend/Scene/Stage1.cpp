#include "../stdafx.h"
#include "Stage1.h"
#include "../Manager/ScrollMgr.h"
#include "../MyBitmap/BmpMgr.h"
#include "../Obj/ObjMgr.h"
#include "../Obj/Player.h"
#include "../Manager/TileMgr.h"
#include "../Obj/Sandbag.h"
#include "../Obj/EarthLoad.h"
#include "../Manager/SceneMgr.h"
#include "../Obj/FButton.h"
/////////////
#include "../Obj/Archer.h"
#include "../Obj/Swordman.h"
#include "../Obj/IcicleEffect.h"
#include "../Obj/SummonCard.h"
#include "../Obj/Summoner.h"
#include "../Obj/SummonerBall.h"
#include "../Obj/Prison.h"
#include "../Obj/TeleCircle.h"

CStage1::CStage1()
	: m_bTeleCircleInserted(false)
{
	memset(m_aRoomClear, -1, sizeof(m_aRoomClear));
}


CStage1::~CStage1()
{
	Release();
}

bool CStage1::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Tile/fireMapTile.bmp", "FireTile"))
		return false;

	if (!CTileMgr::Get_Instance()->Initialize("FireTile"))
		return false;
	CTileMgr::Get_Instance()->Load_Tile("FireTile");
	auto& rPlayer = CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).front();
	rPlayer->Set_Pos(2664.f, 1535.f);

	return true;
}

int CStage1::Update(float _fdTime)
{
	CObjMgr::Get_Instance()->Update(_fdTime);
	if (CSceneMgr::SCENE_STAGE1 == CSceneMgr::Get_Instance()->Get_Current_SceneID())
	{
		CTileMgr::Get_Instance()->Update(_fdTime);
		auto& rPlayer = CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).front();
		
		bool b = true;
		for (int i = 0; i < 6; ++i)
		{
			if (m_aRoomClear[i] < 1) {
				b = false;
				break;
			}
		}

		if (m_aRoomClear[0] == -1)
		{
			if (rPlayer->Get_HitRect().top > 151.f &&
				rPlayer->Get_HitRect().left > 131.f &&
				rPlayer->Get_HitRect().right < 867.f &&
				rPlayer->Get_HitRect().bottom < 1028.f)
			{
				CObj* pPrison = CAbstractFactory<CPrison>::Create(922.f, 610.f, 90.f);
				m_stackPrison.push(pPrison);
				CObjMgr::Get_Instance()->Add_Object(OBJID::OBSTACLE, pPrison);

				CObjMgr::Get_Instance()->Add_Object(OBJID::SUMMONCARD,
					CAbstractFactory<CSummonCard>::Create(510.f, 576.f, "ARCHER_SCARD"));
				m_aRoomClear[0] = 0;
			}
		}
		else if (m_aRoomClear[0] == 0)
		{
			if (CObjMgr::Get_Instance()->Get_listObj(OBJID::SUMMONCARD).empty()
				&& CObjMgr::Get_Instance()->Get_listObj(OBJID::MONSTER).empty())
			{
				m_aRoomClear[0] = 1;
			}
		}
		else if (m_aRoomClear[0] == 1)
		{
			if (!m_stackPrison.empty()) {
				m_stackPrison.top()->Set_Dead();
				m_stackPrison.pop();
				m_aRoomClear[0] = 2;
 			}
		}

		if (m_aRoomClear[1] == -1)
		{
			if (rPlayer->Get_HitRect().top > 1049.f &&
				rPlayer->Get_HitRect().left > 128.f &&
				rPlayer->Get_HitRect().bottom < 2242.f &&
				rPlayer->Get_HitRect().right < 1024.f)
			{
				CObj* pPrison = CAbstractFactory<CPrison>::Create(1057.f, 1760.f, 90.f);
				m_stackPrison.push(pPrison);
				CObjMgr::Get_Instance()->Add_Object(OBJID::OBSTACLE, pPrison);

				CObjMgr::Get_Instance()->Add_Object(OBJID::SUMMONCARD,
					CAbstractFactory<CSummonCard>::Create(474.f, 1811.f, "SWORDMAN_SCARD"));
				m_aRoomClear[1] = 0;
			}
		}
		else if (m_aRoomClear[1] == 0)
		{
			if (CObjMgr::Get_Instance()->Get_listObj(OBJID::SUMMONCARD).empty()
				&& CObjMgr::Get_Instance()->Get_listObj(OBJID::MONSTER).empty())
			{
				m_aRoomClear[1] = 1;
			}
		}
		else if (m_aRoomClear[1] == 1)
		{
			if (!m_stackPrison.empty()) {
				m_stackPrison.top()->Set_Dead();
				m_stackPrison.pop();
				m_aRoomClear[1] = 2;
			}
		}

		if (m_aRoomClear[2] == -1)
		{
			if (rPlayer->Get_HitRect().left > 2050.f &&
				rPlayer->Get_HitRect().right < 3024.f &&
				rPlayer->Get_HitRect().bottom < 410.f)
			{
				CObj* pPrison1 = CAbstractFactory<CPrison>::Create(2657.f, 480.f, 0.f);
				CObj* pPrison2 = CAbstractFactory<CPrison>::Create(3232.f, 223.f, 90.f);
				m_stackPrison.push(pPrison1);
				m_stackPrison.push(pPrison2);
				CObjMgr::Get_Instance()->Add_Object(OBJID::OBSTACLE, pPrison1);
				CObjMgr::Get_Instance()->Add_Object(OBJID::OBSTACLE, pPrison2);

				CObjMgr::Get_Instance()->Add_Object(OBJID::SUMMONCARD,
					CAbstractFactory<CSummonCard>::Create(2273.f, 224.f, "SWORDMAN_SCARD"));
				m_aRoomClear[2] = 0;
			}
		}
		else if (m_aRoomClear[2] == 0)
		{
			if (CObjMgr::Get_Instance()->Get_listObj(OBJID::SUMMONCARD).empty()
				&& CObjMgr::Get_Instance()->Get_listObj(OBJID::MONSTER).empty())
			{
				m_aRoomClear[2] = 1;
			}
		}
		else if (m_aRoomClear[2] == 1)
		{
			for (int i = 0; i < 2; ++i) {
				if (!m_stackPrison.empty()) {
					m_stackPrison.top()->Set_Dead();
					m_stackPrison.pop();
					m_aRoomClear[2] = 2;
				}
			}
		}

		if (m_aRoomClear[3] == -1)
		{
			if (rPlayer->Get_HitRect().left > 4927.f && rPlayer->Get_HitRect().right < 5827.f &&
				rPlayer->Get_HitRect().top > 2560.f && rPlayer->Get_HitRect().bottom < 3268.f)
			{
				CObj* pPrison = CAbstractFactory<CPrison>::Create(5278.f, 3291.f, 0.f);
				m_stackPrison.push(pPrison);
				CObjMgr::Get_Instance()->Add_Object(OBJID::OBSTACLE, pPrison);

				CObjMgr::Get_Instance()->Add_Object(OBJID::SUMMONCARD,
					CAbstractFactory<CSummonCard>::Create(5271.f, 2792.f, "SUMMONER_SCARD"));
				m_aRoomClear[3] = 0;
			}
		}
		else if (m_aRoomClear[3] == 0)
		{
			if (CObjMgr::Get_Instance()->Get_listObj(OBJID::SUMMONCARD).empty()
				&& CObjMgr::Get_Instance()->Get_listObj(OBJID::MONSTER).empty())
			{
				m_aRoomClear[3] = 1;
			}
		}
		else if (m_aRoomClear[3] == 1)
		{
			if (!m_stackPrison.empty()) {
				m_stackPrison.top()->Set_Dead();
				m_stackPrison.pop();
				m_aRoomClear[3] = 2;
			}
		}

		if (m_aRoomClear[4] == -1)
		{
			if (rPlayer->Get_HitRect().left > 2110.f && rPlayer->Get_HitRect().right < 3390.f &&
				rPlayer->Get_HitRect().top > 4275.f && rPlayer->Get_HitRect().bottom < 5056.f)
			{
				CObj* pPrison = CAbstractFactory<CPrison>::Create(2783.f, 4203.f, 0.f);
				m_stackPrison.push(pPrison);
				CObjMgr::Get_Instance()->Add_Object(OBJID::OBSTACLE, pPrison);
				CObjMgr::Get_Instance()->Add_Object(OBJID::SUMMONCARD,
					CAbstractFactory<CSummonCard>::Create(2785.f, 4705.f, "SUMMONER_SCARD"));
				m_aRoomClear[4] = 0;
			}
		}
		else if (m_aRoomClear[4] == 0)
		{
			if (CObjMgr::Get_Instance()->Get_listObj(OBJID::SUMMONCARD).empty()
				&& CObjMgr::Get_Instance()->Get_listObj(OBJID::MONSTER).empty())
			{
				m_aRoomClear[4] = 1;
			}
		}
		else if (m_aRoomClear[4] == 1)
		{
			if (!m_stackPrison.empty()) {
				m_stackPrison.top()->Set_Dead();
				m_stackPrison.pop();
				m_aRoomClear[4] = 2;
			}
		}

		if (m_aRoomClear[5] == -1)
		{
			if (rPlayer->Get_HitRect().top > 2782.f &&
				rPlayer->Get_HitRect().left > 511.f &&
				rPlayer->Get_HitRect().right < 1428.f &&
				rPlayer->Get_HitRect().bottom < 3456.f)
			{
				CObj* pPrison1 = CAbstractFactory<CPrison>::Create(1503.f, 2976.f, 90.f);
				CObj* pPrison2 = CAbstractFactory<CPrison>::Create(608.f, 3483.f, 0.f);
				m_stackPrison.push(pPrison1);
				m_stackPrison.push(pPrison2);
				CObjMgr::Get_Instance()->Add_Object(OBJID::OBSTACLE, pPrison1);
				CObjMgr::Get_Instance()->Add_Object(OBJID::OBSTACLE, pPrison2);

				CObjMgr::Get_Instance()->Add_Object(OBJID::SUMMONCARD,
					CAbstractFactory<CSummonCard>::Create(864.f, 3116.f, "ARCHER_SCARD"));
				m_aRoomClear[5] = 0;
			}
		}
		else if (m_aRoomClear[5] == 0)
		{
			if (CObjMgr::Get_Instance()->Get_listObj(OBJID::SUMMONCARD).empty()
				&& CObjMgr::Get_Instance()->Get_listObj(OBJID::MONSTER).empty())
			{
				m_aRoomClear[5] = 1;
			}
		}
		else if (m_aRoomClear[5] == 1)
		{
			for (int i = 0; i < 2; ++i) {
				if (!m_stackPrison.empty()) {
					m_stackPrison.top()->Set_Dead();
					m_stackPrison.pop();
					m_aRoomClear[5] = 2;
				}
			}
		}

		if (b && !m_bTeleCircleInserted && m_stackPrison.empty())
		{
			// 텔레서클 삽입
			CObj* pTeleCircle = CAbstractFactory<CTeleCircle>::Create(2664.f, 1535.f);
			CObjMgr::Get_Instance()->Add_Object(OBJID::TELECIR, pTeleCircle);
			CObj* pTeleFButton = CAbstractFactory<CFButton>::Create(2664.f, 1535.f, OBJID::STAGE_UI);
			CObjMgr::Get_Instance()->Add_Object(OBJID::STAGE_UI, pTeleFButton);
			static_cast<CFAble*>(pTeleCircle)->Set_fButton(pTeleFButton);
			m_bTeleCircleInserted = true;
		}
	}
	return OBJ_NOEVENT;
}

void CStage1::Late_Update(float _fdTime)
{
	if (CSceneMgr::SCENE_STAGE1 == CSceneMgr::Get_Instance()->Get_Current_SceneID())
	{
		CTileMgr::Get_Instance()->Late_Update(_fdTime);
		CObjMgr::Get_Instance()->Late_Update(_fdTime);
	}
}

void CStage1::Collision(float _fdTime)
{
	CScene::Collision(_fdTime);
}

void CStage1::Render(HDC _DC, float _fdTime)
{	
	CTileMgr::Get_Instance()->Render(_DC, _fdTime);
	CObjMgr::Get_Instance()->Render(_DC, _fdTime);

	//TCHAR szText[32] = L"";
	//HDC hOneTileDC = GetDC(g_hWnd);
	//Rectangle(hOneTileDC, 1030, 110, 1300, 190);
	//swprintf_s(szText, L"스택 개수: [%d], 맨 위의 감옥(%d, %d)", m_stackPrison.size());
	//TextOut(hOneTileDC, 1060, 130, szText, lstrlen(szText));
	//ReleaseDC(g_hWnd, hOneTileDC);
}

void CStage1::Release()
{
	CObjMgr::Get_Instance()->Delete_ID(OBJID::MONSTER);
	//CObjMgr::Get_Instance()->Delete_ID(OBJID::TELECIR);
	CBmpMgr::Get_Instance()->Delete_Bmp("FireTile");
}

void CStage1::Key_Check()
{
}
