#include "../stdafx.h"
#include "EarthBossBox.h"
#include "../MyBitmap/BmpMgr.h"
#include "../Manager/SoundMgr.h"

CEarthBossBox::CEarthBossBox()
	: m_fRisingTime(0.f), m_fRisingLimit(0.3f)
{
}


CEarthBossBox::~CEarthBossBox()
{
	Release();
}

bool CEarthBossBox::Initialize()
{
	//if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/EarthBoss/EarthBoss_Box.bmp", "EarthBoss_Box"))
	//	return false;

	CBullet::Initialize();
	m_tInfo.iCX = 100;
	m_tInfo.iCY = 128;
	Equalize_HitPosInfoPos();
	m_tHitInfo.iCX = 70;
	m_tHitInfo.iCY = 98;

	m_fShotRange = 1500.f;
	m_fSpeed = 1100.f;

	if (m_strFrameKey == "")
		m_strFrameKey = string("EarthBoss_Box");

	m_ePreState = END;
	m_eCurState = FIRE;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();
	m_eRenderGroupID = GROUPID::GAMEOBJECT_2;

	m_iAtt = rand() % 8 + 16;

	return true;
}

int CEarthBossBox::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (!m_pTarget)
		return OBJ_DEAD;

	m_fRisingTime += _fdTime;
	if (m_fRisingTime <= m_fRisingLimit)
	{
		float fDX = 0.f, fDY = 0.f, fDis = 0.f, fRadian = 0.f;
		fDX = m_pTarget->Get_PosX() - m_tInfo.fX;
		fDY = m_pTarget->Get_PosY() - m_tInfo.fY;
		fDis = sqrtf(fDX * fDX + fDY * fDY);
		fRadian = acosf(fDX / fDis);

		if (m_tInfo.fY < m_pTarget->Get_Info().fY)
			fRadian = 2 * PI - fRadian;
		m_fAngle = fRadian * 180.f / PI;

		MoveY(200.f, _fdTime, MOVEDIR::MD_BACK);
	}
	else
	{
		m_tInfo.fX += cosf(Degree_To_Radian(m_fAngle)) * m_fSpeed * _fdTime;
		m_tInfo.fY -= sinf(Degree_To_Radian(m_fAngle)) * m_fSpeed * _fdTime;
	}

	Equalize_HitPosInfoPos();

	Move_Frame();
	Scene_Change();
	Update_Rect();
	Update_HitRect();
	return OBJ_NOEVENT;
}

void CEarthBossBox::Late_Update(float _fdTime)
{
	CBullet::Late_Update(_fdTime);
	Update_Rect();
	Update_HitRect();
}

void CEarthBossBox::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);

	GdiTransparentBlt(_DC
		, (int)(m_tRect.left + _fScrollX)
		, (int)(m_tRect.top + _fScrollY)
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, 0
		, 0
		, m_tInfo.iCX, m_tInfo.iCY
		, MAGENTA_COLOR);
}

void CEarthBossBox::Release()
{
}

void CEarthBossBox::Move_Frame()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();

		if (m_ePreState == CBullet::FIRE) {
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_tFrame.iFrameStart = 0;
		}
		else if (m_ePreState == CBullet::COLLISION) {
			m_bDead = true;
		}
	}
}

void CEarthBossBox::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CBullet::FIRE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			STOP_SOUND(CSoundMgr::MONSTER_EFFECT);
			PLAY_SOUND(L"EarthSkillFire.wav", CSoundMgr::MONSTER_EFFECT);
			break;
		case CBullet::COLLISION:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			STOP_SOUND(CSoundMgr::MONSTER_EFFECT);
			PLAY_SOUND(L"EarthSkillEnd.wav", CSoundMgr::MONSTER_EFFECT);
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}
