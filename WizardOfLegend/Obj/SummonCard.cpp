#include "../stdafx.h"
#include "SummonCard.h"
#include "../MyBitmap/BmpMgr.h"
#include "Archer.h"
#include "SwordMan.h"
#include "Summoner.h"
#include "SummonerBall.h"
#include "ObjMgr.h"
#include "../Manager/SoundMgr.h"

CSummonCard::CSummonCard()
{
}

CSummonCard::~CSummonCard()
{
}

bool CSummonCard::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/SummonCard/SUMMON_CARD_ARCHER.bmp", "ARCHER_SCARD"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/SummonCard/SUMMON_CARD_SWORDMAN.bmp", "SWORDMAN_SCARD"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/SummonCard/SUMMON_CARD_WIZARD.bmp", "SUMMONER_SCARD"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/SummonCard/SUMMON_CARD_WIZARDBALL.bmp", "SUMMONERBALL_SCARD"))
		return false;

	m_tInfo.iCX = 114;
	m_tInfo.iCY = 175;

	// 27번에 몬스터 소환
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 31;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 60;
	m_tFrame.dwFrameTime = GetTickCount();
	return true;
}

int CSummonCard::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;
	
	Move_Frame();
	Scene_Change();
	Update_Rect();
	return OBJ_NOEVENT;
}

void CSummonCard::Late_Update(float _fdTime)
{
}

void CSummonCard::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);

	GdiTransparentBlt(_DC,
		(int)(m_tRect.left + _fScrollX),
		(int)(m_tRect.top + _fScrollY),
		m_tInfo.iCX, m_tInfo.iCY,
		hMemDC,
		m_tFrame.iFrameStart * m_tInfo.iCX,
		0,
		m_tInfo.iCX, m_tInfo.iCY,
		MAGENTA_COLOR);
}

void CSummonCard::Release()
{
}

void CSummonCard::Move_Frame()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();

		if (m_tFrame.iFrameStart == 1)
		{
			STOP_SOUND(CSoundMgr::MONSTER_EFFECT);
			PLAY_SOUND(L"CardSpawn.wav", CSoundMgr::MONSTER_EFFECT);
		}

		if (m_tFrame.iFrameStart == 27) {
			CObj* pObj = Create_Monster();
			if (pObj)
				CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, pObj);
		}
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_bDead = true;
	}
}

void CSummonCard::Scene_Change()
{
}

CObj* CSummonCard::Create_Monster()
{
	if (m_strFrameKey == "ARCHER_SCARD")
	{
		return CAbstractFactory<CArcher>::Create(m_tInfo.fX, m_tInfo.fY + 2.f);
	}
	else if (m_strFrameKey == "SWORDMAN_SCARD")
	{
		return CAbstractFactory<CSwordman>::Create(m_tInfo.fX, m_tInfo.fY + 2.f);
	}
	else if (m_strFrameKey == "SUMMONER_SCARD")
	{
		return CAbstractFactory<CSummoner>::Create(m_tInfo.fX, m_tInfo.fY + 2.f);
	}
	else if (m_strFrameKey == "SUMMONERBALL_SCARD")
	{
		return CAbstractFactory<CSummonerBall>::Create(m_tInfo.fX, m_tInfo.fY + 2.f);
	}
	return nullptr;
}

void CSummonCard::Key_Check(float _fdTime)
{
}
