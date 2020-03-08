#include "../stdafx.h"
#include "Summoner.h"
#include "ObjMgr.h"
#include "../MyBitmap/BmpMgr.h"
#include "SuommnerBullet.h"
#include "SummonCard.h"

using namespace Monster_space;

namespace Summoner_space
{
	const DWORD SUMMONER_ATT_FRAME_SPEED = 600;
}

using Summoner_space::SUMMONER_ATT_FRAME_SPEED;

CSummoner::CSummoner()
{
}

CSummoner::~CSummoner()
{
	Release();
}

bool CSummoner::Initialize()
{
	CMonster::Initialize();
	m_tInfo.iCX = 128;
	m_tInfo.iCY = 200;
	Equalize_HitPosInfoPos();
	m_tHitInfo.iCX = 100;
	m_tHitInfo.iCY = 128;

	// m_fSpeed
	m_iMaxHp = 300;
	m_iHp = m_iMaxHp;
	m_fBulletTick = 0.f;
	m_iBulletCount = 4;
	m_bBulletTickFlag = false;
	m_iPattern = 0;

	//
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/Summoner/Summoner_Right.bmp", "Summoner"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/Summoner/Stretch_128_200.bmp", "Summoner_stretch"))
		return false;

	m_strFrameKey = "Summoner";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();
	return true;
}

int CSummoner::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	// 몬스터 움직이기

	float fDX = 0.f, fDY = 0.f, fDis = 0.f, fRadian = 0.f;

	if (CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).empty())
	{
		m_pTarget = nullptr;
	}
	else if (!m_pTarget)
	{
		CObj* pPlayer = CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).front();
		float fPlayerX = pPlayer->Get_PosX();
		float fPlayerY = pPlayer->Get_PosY();

		fDX = fPlayerX - m_tInfo.fX;
		fDY = fPlayerY - m_tInfo.fY;
		fDis = sqrtf(fDX * fDX + fDY * fDY);

		if (fDis < m_fDetectRange)
			m_pTarget = pPlayer;

		if (m_ePreState == CMonster::HIT)
			m_pTarget = pPlayer;
	}
	else if (m_eCurState != CMonster::DEAD && m_eCurState != CMonster::HIT)
	{
		fDX = m_pTarget->Get_PosX() - m_tInfo.fX;
		fDY = m_pTarget->Get_PosY() - m_tInfo.fY;
		fDis = sqrtf(fDX * fDX + fDY * fDY);
		fRadian = acosf(fDX / fDis);

		if (m_tInfo.fY < m_pTarget->Get_Info().fY)
			fRadian = 2 * PI - fRadian;

		m_fAngle = fRadian * 180.f / PI;

		if (fDis > 600.f)
		{
			MoveAngle(_fdTime);
			m_eCurState = WALK;
			m_tFrame.iFrameEnd = 4;
		}
		else
		{
			if (m_eCurState != IDLE && m_eCurState != FALL)
			{
				m_eCurState = ATTACK;
				m_tFrame.iFrameEnd = 3;

				if (m_bBulletTickFlag)
					m_fBulletTick += _fdTime;

				if (!m_bAttackCool) {
					if (m_tFrame.iFrameStart == 0)
					{
						if (m_iPattern == 0)
						{
							m_bBulletTickFlag = true;
							if (m_fBulletTick > 0.1125f)
							{
								m_fBulletTick = 0.f;
								CObjMgr::Get_Instance()->Add_Object(OBJID::M_RECTBULLET,
									Create_Bullet("SummonerBullet"));
								CObjMgr::Get_Instance()->Add_Object(OBJID::M_RECTBULLET,
									Create_Bullet(m_fAngle + 32.f, "SummonerBullet"));
								CObjMgr::Get_Instance()->Add_Object(OBJID::M_RECTBULLET,
									Create_Bullet(m_fAngle - 32.f, "SummonerBullet"));

								--m_iBulletCount;
								if (m_iBulletCount <= 0)
								{
									m_bAttackCool = true;
									m_iBulletCount = 4;
								}
							}
						}
						else if (m_iPattern == 1)
						{
							for (int i = 0; i < 6; ++i)
							{
								float fX = m_tInfo.fX + cosf(Degree_To_Radian(60.f * i)) * 270.f;
								float fY = m_tInfo.fY - sinf(Degree_To_Radian(60.f * i)) * 270.f;
								CObjMgr::Get_Instance()->Add_Object(OBJID::SUMMONCARD,
									CAbstractFactory<CSummonCard>::Create(fX, fY, "SUMMONERBALL_SCARD"));
							}	
							m_bAttackCool = true;
						}
					}
				}
			}
		}
	}

	Equalize_HitPosInfoPos();
	Move_Frame();
	Scene_Change();
	Change_HitRect();
	Update_Rect();
	Update_HitRect();
	return OBJ_NOEVENT;
}

void CSummoner::Late_Update(float _fdTime)
{
	CMonster::Late_Update(_fdTime);

	if (m_bAttackCool &&
		m_ePreState == CMonster::ATTACK && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
	{
		m_bAttackCool = false;
		m_iBulletCount = 4;
		m_fBulletTick = 0.f;
		m_bBulletTickFlag = false;
		m_iPattern = (m_iPattern == 0) ? 1 : 0;
	}
}

void CSummoner::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);
	HDC hStrDC = CBmpMgr::Get_Instance()->Find_Image("Summoner_stretch");

	if (m_fAngle > 90.f && m_fAngle <= 270.f)
	{
		StretchBlt(hStrDC,
			0, 0, m_tInfo.iCX, m_tInfo.iCY, hMemDC,
			m_tFrame.iFrameStart * m_tInfo.iCX + m_tInfo.iCX,
			m_tFrame.iFrameScene * m_tInfo.iCY,
			-m_tInfo.iCX, m_tInfo.iCY, SRCCOPY);

		GdiTransparentBlt(_DC,
			(int)(m_tRect.left + _fScrollX),
			(int)(m_tRect.top + _fScrollY),
			m_tInfo.iCX, m_tInfo.iCY,
			hStrDC,
			0,
			0,
			m_tInfo.iCX, m_tInfo.iCY,
			MAGENTA_COLOR);
	}
	else {
		GdiTransparentBlt(_DC,
			(int)(m_tRect.left + _fScrollX),
			(int)(m_tRect.top + _fScrollY),
			m_tInfo.iCX, m_tInfo.iCY,
			hMemDC,
			m_tFrame.iFrameStart * m_tInfo.iCX,
			m_tFrame.iFrameScene * m_tInfo.iCY,
			m_tInfo.iCX, m_tInfo.iCY,
			MAGENTA_COLOR);
	}

	Draw_HitBox(_DC, _fScrollX, _fScrollY);
	Draw_DetectCircle(_DC, _fScrollX, _fScrollY);
}

void CSummoner::Move_Frame()
{

	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();
		if (m_ePreState == CMonster::DEAD)
		{
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_bDead = true;
		}
		else if (m_ePreState == CMonster::ATTACK)
		{
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_eCurState = CMonster::IDLE;
		}
		else if (m_ePreState == CMonster::HIT)
		{
			if (m_tFrame.iFrameStart == 0)
				m_tFrame.dwFrameSpeed = HIT_FRAME_SPEED;
			else
				m_tFrame.dwFrameSpeed = (DWORD)(HIT_FRAME_SPEED * 1.5f);

			if (m_eMez == MEZ::MZ_FROZEN)
			{
				if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				{
					m_tFrame.iFrameStart = m_tFrame.iFrameEnd;
				}
			}
			else
			{
				if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				{
					++m_iHitStateCount;
					if (m_iHitStateCount > M_HIT_FRAME_COUNTMAX)
					{
						m_eCurState = CMonster::IDLE;
						m_iHitStateCount = 0;
					}
					else
					{
						m_tFrame.iFrameStart = 0;
					}
				}
			}
		}
		else if (m_ePreState == CMonster::IDLE)
		{
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_eCurState = CMonster::ATTACK;
		}
		else if (m_ePreState == CMonster::WALK)
		{
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_tFrame.iFrameStart = 0;
		}
		else if (m_ePreState == CMonster::FALL)
		{	// 여기 신경 쓸 것
			--m_tFrame.iFrameStart;
		}
	}
}

void CSummoner::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CMonster::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 2400;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CMonster::WALK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameSpeed = WALK_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CMonster::ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 2;
			m_tFrame.dwFrameSpeed = SUMMONER_ATT_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CMonster::HIT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 3;
			m_tFrame.dwFrameSpeed = HIT_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CMonster::DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 4;
			m_tFrame.dwFrameSpeed = DEAD_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CMonster::FALL:
			m_tFrame.iFrameStart = 1;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 3;
			m_tFrame.dwFrameSpeed = FALL_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CSummoner::Change_HitRect()
{
	switch (m_eCurState)
	{
	case CMonster::IDLE:
		m_tHitInfo.iCX = 100;
		m_tHitInfo.iCY = 128;
		break;
	case CMonster::WALK:
		m_tHitInfo.iCX = 88;
		m_tHitInfo.iCY = 128;
		break;
	case CMonster::ATTACK:
		m_tHitInfo.iCX = 98;
		m_tHitInfo.iCY = 132;
		break;
	case CMonster::HIT:
		m_tHitInfo.iCX = 72;
		m_tHitInfo.iCY = 128;
		break;
	case CMonster::DEAD:
		m_tHitInfo.iCX = 0;
		m_tHitInfo.iCY = 0;
		break;
	case CMonster::FALL:
		m_tHitInfo.iCX = 0;
		m_tHitInfo.iCY = 0;
		break;
	default:
		break;
	}
}

CObj * CSummoner::Create_Bullet(const string & _frameKey)
{
	CObj* pObj = CAbstractFactory<CSummonerBullet>::Create(
		m_tHitInfo.fX, m_tHitInfo.fY, m_fAngle, _frameKey);
	return pObj;
}

CObj * CSummoner::Create_Bullet(float _fAngle, const string & _frameKey)
{
	CObj* pObj = CAbstractFactory<CSummonerBullet>::Create(
		m_tHitInfo.fX, m_tHitInfo.fY, _fAngle, _frameKey);
	return pObj;
}
