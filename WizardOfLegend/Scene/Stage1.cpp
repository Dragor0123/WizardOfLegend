#include "../stdafx.h"
#include "Stage1.h"
#include "../MyBitmap/BmpMgr.h"
#include "../Obj/ObjMgr.h"
#include "../Obj/Player.h"
#include "../Manager/TileMgr.h"
#include "../Manager/SceneMgr.h"
#include "../Obj/FButton.h"
#include "../Obj/IcicleEffect.h"
#include "../Obj/Prison.h"
#include "../Obj/TeleCircle.h"
#include "../Obj/Inventory.h"
#include "../Obj/PlayerHPBar.h"
#include "../Obj/UIGold.h"
#include "../Obj/SummonCard.h"

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

	// ÇÃ·¹ÀÌ¾î HPBAR, SKILLBAR »ðÀÔ, µ· ui»ðÀÔ
	CObjMgr::Get_Instance()->Add_Object(OBJID::STAGE1_UI,
		CAbstractFactory<CPlayerHPBar>::Create(rPlayer));

	CObjMgr::Get_Instance()->Add_Object(OBJID::STAGE1_UI,
		CAbstractFactory<CUISkillSet>::Create());

	CObjMgr::Get_Instance()->Add_Object(OBJID::STAGE1_UI,
		CAbstractFactory<CUIGold>::Create(rPlayer));

	return true;
}

int CStage1::Update(float _fdTime)
{
	CTileMgr::Get_Instance()->Update(_fdTime);
	auto& rPlayer = CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).front();

	bool b = true;
	for (int i = 0; i < 5; ++i)
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
				CAbstractFactory<CSummonCard>::Create(290.f, 858.f, "SWORDMAN_SCARD"));

			CObjMgr::Get_Instance()->Add_Object(OBJID::SUMMONCARD,
				CAbstractFactory<CSummonCard>::Create(289.f, 417.f, "SWORDMAN_SCARD"));
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
				CAbstractFactory<CSummonCard>::Create(297.f, 1649.f, "ARCHER_SCARD"));
			CObjMgr::Get_Instance()->Add_Object(OBJID::SUMMONCARD,
				CAbstractFactory<CSummonCard>::Create(277.f, 2100.f, "ARCHER_SCARD"));
			CObjMgr::Get_Instance()->Add_Object(OBJID::SUMMONCARD,
				CAbstractFactory<CSummonCard>::Create(827.f, 2077.f, "ARCHER_SCARD"));
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
				CAbstractFactory<CSummonCard>::Create(2293.f, 224.f, "SWORDMAN_SCARD"));
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
				CAbstractFactory<CSummonCard>::Create(5098.f, 2712.f, "ARCHER_SCARD"));
			CObjMgr::Get_Instance()->Add_Object(OBJID::SUMMONCARD,
				CAbstractFactory<CSummonCard>::Create(5608.f, 2718.f, "SUMMONERBALL_SCARD"));
			CObjMgr::Get_Instance()->Add_Object(OBJID::SUMMONCARD,
				CAbstractFactory<CSummonCard>::Create(5670.f, 3038.f, "SUMMONERBALL_SCARD"));
			CObjMgr::Get_Instance()->Add_Object(OBJID::SUMMONCARD,
				CAbstractFactory<CSummonCard>::Create(5357.f, 2667.f, "SUMMONERBALL_SCARD"));
			CObjMgr::Get_Instance()->Add_Object(OBJID::SUMMONCARD,
				CAbstractFactory<CSummonCard>::Create(5029.f, 3104.f, "SUMMONERBALL_SCARD"));
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
				CAbstractFactory<CSummonCard>::Create(2457.f, 4637.f, "SUMMONER_SCARD"));
			CObjMgr::Get_Instance()->Add_Object(OBJID::SUMMONCARD,
				CAbstractFactory<CSummonCard>::Create(3102.f, 4637.f, "SUMMONER_SCARD"));
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
		// ÅÚ·¹¼­Å¬ »ðÀÔ
		CObj* pTeleCircle = CAbstractFactory<CTeleCircle>::Create(2664.f, 1535.f);
		CObjMgr::Get_Instance()->Add_Object(OBJID::TELECIR, pTeleCircle);
		CObj* pTeleFButton = CAbstractFactory<CFButton>::Create(2664.f, 1535.f, OBJID::STAGE1_UI);
		CObjMgr::Get_Instance()->Add_Object(OBJID::STAGE1_UI, pTeleFButton);
		static_cast<CFAble*>(pTeleCircle)->Set_fButton(pTeleFButton);
		m_bTeleCircleInserted = true;
	}

	CObjMgr::Get_Instance()->Update(_fdTime);

	return OBJ_NOEVENT;
}

void CStage1::Late_Update(float _fdTime)
{
	CTileMgr::Get_Instance()->Late_Update(_fdTime);
	CObjMgr::Get_Instance()->Late_Update(_fdTime);
}

void CStage1::Collision(float _fdTime)
{
	CScene::Collision(_fdTime);
}

void CStage1::Render(HDC _DC, float _fdTime)
{	
	CTileMgr::Get_Instance()->Render(_DC, _fdTime);
	CObjMgr::Get_Instance()->Render(_DC, _fdTime);
}

void CStage1::Release()
{
	for (int i = (int)OBJID::UNDERDECO; i <= (int)OBJID::EFFECT; ++i) {
		if ((OBJID::ID)i == OBJID::PLAYER)
			continue;
		CObjMgr::Get_Instance()->Delete_ID((OBJID::ID)i);
	}

	CObjMgr::Get_Instance()->Delete_ID(OBJID::DIGIT_UI);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::STAGE1_UI);

	CBmpMgr::Get_Instance()->Delete_Bmp("FireTile");
}

bool CStage1::Key_Check()
{
	return true;
}

/*
	CObjMgr::Get_Instance()->Update(_fdTime);
	if (CSceneMgr::SCENE_STAGE1 == CSceneMgr::Get_Instance()->Get_Scene_ID())
	{
		CTileMgr::Get_Instance()->Update(_fdTime);
		auto& rPlayer = CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).front();

		bool b = true;
		for (int i = 0; i < 5; ++i)
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
					CAbstractFactory<CSummonCard>::Create(290.f, 858.f, "SWORDMAN_SCARD"));

				CObjMgr::Get_Instance()->Add_Object(OBJID::SUMMONCARD,
					CAbstractFactory<CSummonCard>::Create(289.f, 417.f, "SWORDMAN_SCARD"));
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
					CAbstractFactory<CSummonCard>::Create(297.f, 1649.f, "ARCHER_SCARD"));
				CObjMgr::Get_Instance()->Add_Object(OBJID::SUMMONCARD,
					CAbstractFactory<CSummonCard>::Create(277.f, 2100.f, "ARCHER_SCARD"));
				CObjMgr::Get_Instance()->Add_Object(OBJID::SUMMONCARD,
					CAbstractFactory<CSummonCard>::Create(827.f, 2077.f, "ARCHER_SCARD"));
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
					CAbstractFactory<CSummonCard>::Create(2293.f, 224.f, "SWORDMAN_SCARD"));
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
					CAbstractFactory<CSummonCard>::Create(5098.f, 2712.f, "ARCHER_SCARD"));
				CObjMgr::Get_Instance()->Add_Object(OBJID::SUMMONCARD,
					CAbstractFactory<CSummonCard>::Create(5608.f, 2718.f, "SUMMONERBALL_SCARD"));
				CObjMgr::Get_Instance()->Add_Object(OBJID::SUMMONCARD,
					CAbstractFactory<CSummonCard>::Create(5670.f, 3038.f, "SUMMONERBALL_SCARD"));
				CObjMgr::Get_Instance()->Add_Object(OBJID::SUMMONCARD,
					CAbstractFactory<CSummonCard>::Create(5357.f, 2667.f, "SUMMONERBALL_SCARD"));
				CObjMgr::Get_Instance()->Add_Object(OBJID::SUMMONCARD,
					CAbstractFactory<CSummonCard>::Create(5029.f, 3104.f, "SUMMONERBALL_SCARD"));
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
					CAbstractFactory<CSummonCard>::Create(2457.f, 4637.f, "SUMMONER_SCARD"));
				CObjMgr::Get_Instance()->Add_Object(OBJID::SUMMONCARD,
					CAbstractFactory<CSummonCard>::Create(3102.f, 4637.f, "SUMMONER_SCARD"));
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
			// ÅÚ·¹¼­Å¬ »ðÀÔ
			CObj* pTeleCircle = CAbstractFactory<CTeleCircle>::Create(2664.f, 1535.f);
			CObjMgr::Get_Instance()->Add_Object(OBJID::TELECIR, pTeleCircle);
			CObj* pTeleFButton = CAbstractFactory<CFButton>::Create(2664.f, 1535.f, OBJID::STAGE_UI);
			CObjMgr::Get_Instance()->Add_Object(OBJID::STAGE_UI, pTeleFButton);
			static_cast<CFAble*>(pTeleCircle)->Set_fButton(pTeleFButton);
			m_bTeleCircleInserted = true;
		}
	}
*/