#include "../stdafx.h"
#include "EarthPillarMaker.h"
#include "NoMoveBullet.h"
#include "ObjMgr.h"
#include "../MyBitmap/BmpMgr.h"

namespace PillarMaker_space
{
	const float PILLAR_INTERVAL_LIMIT = 0.04f;
}

using PillarMaker_space::PILLAR_INTERVAL_LIMIT;

CEarthPillarMaker::CEarthPillarMaker()
	:	m_fInterval(-0.03f)
{
}


CEarthPillarMaker::~CEarthPillarMaker()
{
	Release();
}

bool CEarthPillarMaker::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/EarthBoss/EarthBoss_Pillar.bmp", "EarthBoss_Pillar"))
		return false;

	CBullet::Initialize();
	m_tInfo.iCX = 32;
	m_tInfo.iCY = 32;
	Equalize_HitPosInfoPos();
	Equalize_HitSzInfoSz();

	m_fSpeed = 800.f;
	m_fShotRange = 1000.f;

	return true;
}

int CEarthPillarMaker::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;
	
	m_tInfo.fX += cosf(Degree_To_Radian(m_fAngle)) * m_fSpeed * _fdTime;
	m_tInfo.fY -= sinf(Degree_To_Radian(m_fAngle)) * m_fSpeed * _fdTime;

	m_fInterval += _fdTime;

	if (m_fInterval > PILLAR_INTERVAL_LIMIT)
	{
		CObj* pPillar = CAbstractFactory<CNoMoveBullet>::Create(m_tInfo.fX, m_tInfo.fY, "EarthBoss_Pillar");
		CObjMgr::Get_Instance()->Add_Object(OBJID::M_RECTBULLET, pPillar);
		m_fInterval = 0.f;
	}

	Equalize_HitPosInfoPos();

	Update_Rect();
	Update_HitRect();
	return OBJ_NOEVENT;
}

void CEarthPillarMaker::Late_Update(float _fdTime)
{
	//EarthPillarMaker 자체는 플레이어와 충돌 X, 오직 벽이랑만 충돌
	if (m_bCollision)
		m_bDead = true;

	float fdX = 0.f, fdY = 0.f;
	fdX = m_tInfo.fX - m_tStartPt.fX;
	fdY = m_tInfo.fY - m_tStartPt.fY;

	float fdist = sqrtf(fdX * fdX + fdY * fdY);

	if (fdist > m_fShotRange) {
		m_bDead = true;
	}
}

void CEarthPillarMaker::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	// 라인 정도를 그려주면 될 지도??
	//Ellipse(_DC,
	//	(int)(m_tHitRect.left + _fScrollX),
	//	(int)(m_tHitRect.top + _fScrollY),
	//	(int)(m_tHitRect.right + _fScrollX),
	//	(int)(m_tHitRect.bottom + _fScrollY));

	//Draw_HitBox(_DC, _fScrollX, _fScrollY);
}

void CEarthPillarMaker::Release()
{
}

void CEarthPillarMaker::Move_Frame()
{
}

void CEarthPillarMaker::Scene_Change()
{
}

void CEarthPillarMaker::Change_HitRect()
{
}
