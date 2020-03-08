#include "../stdafx.h"
#include "FireKick.h"
#include "../MyBitmap/BmpMgr.h"
#include "FireBoss.h"
#include "../Manager/SoundMgr.h"

CFireKick::CFireKick()
	: m_fDeadTime(0.f)
{
	m_bMonsters = true;
}


CFireKick::~CFireKick()
{
	Release();
}

bool CFireKick::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/FireBoss/FireKick.bmp", "FireKick"))
		return false;
	if (m_pTarget == nullptr)
		return false;
	if (!dynamic_cast<CFireBoss*>(m_pTarget))
		return false;

	CBullet::Initialize();
	m_tInfo.iCX = 200;
	m_tInfo.iCY = 200;
	Equalize_HitPosInfoPos();
	m_tHitInfo.iCX = 188;
	m_tHitInfo.iCX = 188;

	m_fSpeed = 500.f;
	m_fShotRange = 200.f;
	m_iAtt = rand() % 12 + 18;

	if (m_strFrameKey == "")
		m_strFrameKey = string("FireKick");

	m_ePreState = END;
	m_eCurState = FIRE;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();

	const CBoss::BOSSDIR eDir = dynamic_cast<CFireBoss*>(m_pTarget)->Get_Boss_Dir();
	m_iDrawID = (int)eDir;

	m_eRenderGroupID = GROUPID::GAMEOBJECT_2;
	return true;
}

int CFireKick::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	switch (m_iDrawID)
	{
	case 0:
		MoveXFromSpeed(_fdTime, MOVEDIR::MD_BACK);
		break;
	case 1:
		MoveYFromSpeed(_fdTime, MOVEDIR::MD_BACK);
		break;
	case 2:
		MoveXFromSpeed(_fdTime, MOVEDIR::MD_FRONT);
		break;
	case 3:
		MoveYFromSpeed(_fdTime, MOVEDIR::MD_FRONT);
		break;
	}

	if (m_ePreState == CBullet::COLLISION)
	{
		m_fDeadTime += _fdTime;
	}

	// 움직임 구현
	Equalize_HitPosInfoPos();

	//Move_Frame();
	Scene_Change();
	Update_Rect();
	Update_HitRect();
	return OBJ_NOEVENT;
}

void CFireKick::Late_Update(float _fdTime)
{
	CBullet::Late_Update(_fdTime);
	if (m_fDeadTime > 0.4f)
		m_bDead = true;
}

void CFireKick::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);
	GdiTransparentBlt(_DC
		, (int)(m_tRect.left + _fScrollX)
		, (int)(m_tRect.top + _fScrollY)
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_iDrawID * m_tInfo.iCX
		, 0
		, m_tInfo.iCX, m_tInfo.iCY
		, MAGENTA_COLOR);

	Draw_HitCircle(_DC, _fScrollX, _fScrollY);
}

void CFireKick::Release()
{
}

void CFireKick::Move_Frame()
{
}

void CFireKick::Scene_Change()
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
			PLAY_SOUND(L"FireKick.wav", CSoundMgr::MONSTER_EFFECT);
			break;
		case CBullet::COLLISION:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}
