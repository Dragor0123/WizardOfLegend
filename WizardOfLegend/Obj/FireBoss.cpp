#include "../stdafx.h"
#include "FireBoss.h"
#include "../MyBitmap/BmpMgr.h"
#include "ObjMgr.h"
#include "FireBall.h"
#include "../Manager/CollisionMgr.h"
#include "Meteor.h"
#include "ScrewBullet.h"
#include "FireKick.h"

namespace FIREBOSS_Space
{
	const DWORD HIT_FRAME_SPEED = 80;
	const DWORD IDLE_TIME = 600;
	const DWORD DEAD_FRAME_SPEED = 1300;
	const DWORD DANCE_FRAME_SPEED = 160;
	const DWORD DRAGON_FRAME_SPEED = 100;
	const DWORD MOVE_FRAME_SPEED = 900;
	const DWORD FIREBALL_FRAME_SPEED = 120;
	const DWORD KICK_FRAME_SPEED = 120;
	const DWORD JUMP_FRAME_SPEED = 80;			// JUMP -> METEOR 소환

	const float METEO_TIMELIMIT = 3.6f;
	const float DRAGON_TIMELIMIT = 2.0f;
}

using namespace FIREBOSS_Space;
using Meteor_Space::METEOR_POSY_START;

CFireBoss::CFireBoss()
	: m_fPosinRange(110.f), m_fPosinAng(0.f)
{
	ZeroMemory(&m_tPosin, sizeof(m_tPosin));
	ZeroMemory(m_fBulletTick, sizeof(m_fBulletTick));
	ZeroMemory(m_fBulletTickLimit, sizeof(m_fBulletTickLimit));
}

CFireBoss::~CFireBoss()
{
	Release();
}

bool CFireBoss::Initialize()
{
	CBoss::Initialize();
	m_eFIREPreState = FIREB_STATE::FIRE_S_END;
	m_eFIRECurState = FIREB_STATE::FIRE_IDLE;

	m_tInfo.iCX = 160;
	m_tInfo.iCY = 200;

	Equalize_HitPosInfoPos();
	// IDLE일 때
	m_tHitInfo.iCX = 68;
	m_tHitInfo.iCY = 152;
	m_iHitStateCount = 0;
	m_ePattern = CFireBoss::P_METEOR;
	m_eDir = BOSSDIR::BOTTOM;

	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/FireBoss/FireBoss.bmp", "FireBoss"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/FireBoss/stretch160_200.bmp", "FireBoss_stretch"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/FireBoss/Fireball_Boss.bmp", "FireBoss_Fireball"))
		return false;

	m_strFrameKey = "FireBoss";

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = IDLE_TIME;
	m_tFrame.dwFrameTime = GetTickCount();

	// EARTH보스의 경우,,,,  스킬 당 FRAME구간 반복 횟수...로 썼는데;;;
	// (한 스킬프레임구간 당 한 발씩 발사했으니까)
	// 파이어보스는 그냥 총알 개수로 보자.

	m_iAttackCntLimit[P_FIREBALL] = 6;
	m_iAttackCntLimit[P_DRAGON] = 1;
	m_iAttackCntLimit[P_KICK] = 3;
	m_iAttackCntLimit[P_METEOR] = 1;

	for (int i = 0; i < CFireBoss::P_END; ++i)
		m_iPatternArr[i] = i;

	for (int i = 0; i < 30; ++i)
	{
		int  _left = rand() % CFireBoss::P_END;
		int  _right = rand() % CFireBoss::P_END;
		Swap(&m_iPatternArr[_left], &m_iPatternArr[_right]);
	}

	// 점프 관련 초기화
	m_fJumpPower = 12.f;
	m_bJump = false;
	m_fJumpTime = 0.f;
	m_fJumpDirAngle = 90.f;
	m_fJumpLandY = m_tInfo.fY + (float)(m_tInfo.iCY >> 1);

	m_fMeteoTime = 0.f;
	m_fDragonTime = 0.f;
	m_bMeteoFire = false;
	m_bDragonFire = false;

	m_bMoveStateFlag = false;
	m_bKickFire = false;
	m_bKick_bulletCreate = false;

	m_fBulletTickLimit[P_FIREBALL] = 0.15f;
	m_fBulletTickLimit[P_DRAGON] = 0.275f;
	m_fBulletTickLimit[P_KICK] = 0.125f;
	m_fBulletTickLimit[P_METEOR] = 0.35f;

	return true;
}

int CFireBoss::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;
	
	float fDX = 0.f, fDY = 0.f, fDis = 0.f, fRadian = 0.f;
	float fPlayerX = 0.f, fPlayerY = 0.f;

	if (CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).empty())
	{
		m_pTarget = nullptr;
	}
	else if (!m_pTarget)
	{
		CObj* pPlayer = CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).front();
		fPlayerX = pPlayer->Get_PosX();
		fPlayerY = pPlayer->Get_PosY();

		fDX = fPlayerX - m_tInfo.fX;
		fDY = fPlayerY - m_tInfo.fY;
		fDis = sqrtf(fDX * fDX + fDY * fDY);

		if (fDis < m_fDetectRange)
			m_pTarget = pPlayer;
	}
	else if (m_eFIRECurState != CFireBoss::FIRE_DEAD)
	{
		fPlayerX = m_pTarget->Get_PosX();
		fPlayerY = m_pTarget->Get_PosY();

		fDX = fPlayerX - m_tInfo.fX;
		fDY = fPlayerY - m_tInfo.fY;
		fDis = sqrtf(fDX * fDX + fDY * fDY);

		float fRealDX = abs(fDX) - (float)(m_tHitInfo.iCX >> 1) - (float)(m_pTarget->Get_HitInfo().iCX >> 1);
		float fRealDY = abs(fDY) - (float)(m_tHitInfo.iCY >> 1) - (float)(m_pTarget->Get_HitInfo().iCY >> 1);

		if (fRealDX > 0.f || fRealDY > 0.f) // 플레이어랑 이격되어있음
		{
			if (fRealDX >= fRealDY) { // 가로로 더 멀다. -> 좌우
				if (m_tInfo.fX < fPlayerX - (float)(m_tHitInfo.iCX >> 1))
				{// 보스가 더 왼쪽
					m_eDir = BOSSDIR::RIGHT;
				}
				else if (m_tInfo.fX > fPlayerX + (float)(m_tHitInfo.iCX >> 1))
				{ // 보스가 더 오른쪽
					m_eDir = BOSSDIR::LEFT;
				}
			}
			else //세로로 더 멀다.
			{
				if (m_tInfo.fY < fPlayerY - (float)(m_pTarget->Get_HitInfo().iCY >> 1))
				{
					// 보스가 더 위에 있음.
					m_eDir = BOSSDIR::BOTTOM;
				}
				else if (m_tInfo.fY > fPlayerY + (float)(m_pTarget->Get_HitInfo().iCY >> 1))
				{
					// 보스가 더 아래에
					m_eDir = BOSSDIR::TOP;
				}
			}
		}

		fRadian = acosf(fDX / fDis);

		if (m_tInfo.fY < m_pTarget->Get_Info().fY)
			fRadian = 2 * PI - fRadian;

		m_fAngle = fRadian * 180.f / PI;
		//m_fPosinAng += 5.f;
		// posin...

		m_ePattern = (CFireBoss::PATTERN)m_iPatternArr[m_iPatternCnt];
		Attack_Pattern(_fdTime, fDis);

		if ((m_eFIREPreState >= ATT_MOVE_RIGHT && m_eFIREPreState <= ATT_KICK) 
			&& m_ePattern == P_KICK)
		{
			if (!(m_eFIREPreState == ATT_KICK && m_tFrame.iFrameStart > 4))
			{
				if (m_eDir == CBoss::LEFT || m_eDir == CBoss::RIGHT) {
					if (floor(fDis) > 116.f) MoveAngle(_fdTime);
				}
				else if (floor(fDis) > 144.f) MoveAngle(_fdTime);
			}
		}
	}

	if (m_bMeteoFire == true)
	{
		m_fMeteoTime += _fdTime;
		m_fBulletTick[CFireBoss::P_METEOR] += _fdTime;
	}

	if (m_bDragonFire == true)
	{
		m_fDragonTime += _fdTime;
		m_fBulletTick[CFireBoss::P_DRAGON] += _fdTime;
	}

	if (m_bKickFire == true)
	{
		m_fBulletTick[CFireBoss::P_KICK] += _fdTime;
	}

	Jumping(_fdTime, m_fAngle);
	Equalize_HitPosInfoPos();

	///////////
	Move_Frame();
	Scene_Change();
	Change_HitRect();
	Update_Rect();
	Update_HitRect();
	return OBJ_NOEVENT;
}

int CFireBoss::Get_Boss_State() const
{
	return (int)m_eFIRECurState;
}

void CFireBoss::Set_Boss_State(int _eNewState)
{
	if (m_eFIRECurState != CFireBoss::FIRE_DEAD)
		m_eFIRECurState = (CFireBoss::FIREB_STATE)(_eNewState);
}

void CFireBoss::Late_Update(float _fdTime)
{
	if (0 >= m_iHp)
	{
		m_eFIRECurState = CFireBoss::FIRE_DEAD;
		m_tHitInfo.iCX = 0;
		m_tHitInfo.iCY = 0;
	}
	
	if (m_iHitDigitCnt > HIT_DIGIT_CNT_MAX || m_eFIREPreState != CFireBoss::FIRE_HIT)
		Reset_HitDigitCnt();

	Update_Rect();
	Update_HitRect();

	if (m_bAttackCool &&
		m_tFrame.iFrameStart == 0 &&
		((m_eFIREPreState >= ATT_DRAGON_RIGHT && m_eFIREPreState <= ATT_FIREBALL_UP)
			|| (m_eFIREPreState > FIRE_DEAD && m_eFIREPreState <= ATT_JUMP))
		)
	{
		m_bAttackCool = false;
	}

	if (m_fMeteoTime > METEO_TIMELIMIT && m_bMeteoFire)
	{
		m_fMeteoTime = 0.f;
		m_bMeteoFire = false;
	}

	if (m_fDragonTime > DRAGON_TIMELIMIT && m_bDragonFire)
	{
		m_fDragonTime = 0.f;
		m_bDragonFire = false;
	}
}

void CFireBoss::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);
	HDC hStrDC = CBmpMgr::Get_Instance()->Find_Image("FireBoss_stretch");

	if (m_eFIREPreState != IDLE && m_eFIREPreState!= FIRE_HIT && m_eFIREPreState != FIRE_DEAD &&
		(m_fAngle > 90.f && m_fAngle <= 270.f))
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


	//TCHAR szText[64] = L"";
	//HDC hOneTileDC = GetDC(g_hWnd);
	//Rectangle(hOneTileDC, 1030, 110, 1600, 200);
	//swprintf_s(szText, L"PatternCnt : %d", m_iPatternCnt);
	//TextOut(hOneTileDC, 1060, 120, szText, lstrlen(szText));
	//swprintf_s(szText, L"이전상태: %d, 현재상태: %d", (int)m_eFIREPreState, (int)m_eFIRECurState);
	//TextOut(hOneTileDC, 1060, 150, szText, lstrlen(szText));
	//swprintf_s(szText, L"프레임start: %d, 프레임End: %d", m_tFrame.iFrameStart, m_tFrame.iFrameEnd);
	//TextOut(hOneTileDC, 1060, 180, szText, lstrlen(szText));

	//ReleaseDC(g_hWnd, hOneTileDC);
}

void CFireBoss::Release()
{
	CBmpMgr::Get_Instance()->Delete_Bmp("FireBoss");
	CBmpMgr::Get_Instance()->Delete_Bmp("FireBoss_stretch");
	CBmpMgr::Get_Instance()->Delete_Bmp("Meteor");
	// FireBall은 플레이어도 쓸 가능성이 농후함.
	// FIREKICK.... 
}

void CFireBoss::Scene_Change()
{
	if (m_eFIREPreState != m_eFIRECurState)
	{
		switch (m_eFIRECurState)
		{
		case CFireBoss::FIRE_IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = IDLE_TIME;
			m_tFrame.dwFrameTime = GetTickCount();
			m_bHittable = false;
			break;
		case CFireBoss::FIRE_DANCE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameSpeed = DANCE_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			m_bHittable = true;
			break;
		case CFireBoss::ATT_DRAGON_RIGHT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 5;
			m_tFrame.dwFrameSpeed = FIREBALL_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			m_bHittable = false;
			break;
		case CFireBoss::ATT_DRAGON_DOWN:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 7;
			m_tFrame.dwFrameSpeed = FIREBALL_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			m_bHittable = false;
			break;
		case CFireBoss::ATT_DRAGON_UP:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 6;
			m_tFrame.dwFrameSpeed = FIREBALL_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			m_bHittable = false;
			break;
		case CFireBoss::ATT_FIREBALL_RIGHT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 5;
			m_tFrame.dwFrameSpeed = FIREBALL_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			m_bHittable = false;
			break;
		case CFireBoss::ATT_FIREBALL_UP:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 6;
			m_tFrame.dwFrameSpeed = FIREBALL_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			m_bHittable = false;
			break;
		case CFireBoss::FIRE_HIT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 10;
			m_tFrame.dwFrameSpeed = HIT_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			m_bHittable = true;
			break;
		case CFireBoss::FIRE_DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 11;
			m_tFrame.dwFrameSpeed = DEAD_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			m_bHittable = false;
			break;
		case CFireBoss::ATT_FIREBALL_DOWN:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 7;
			m_tFrame.dwFrameSpeed = FIREBALL_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			m_bHittable = false;
			break;
		case CFireBoss::ATT_MOVE_RIGHT:
			m_tFrame.iFrameStart = 0; // 0.
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 2;
			m_tFrame.dwFrameSpeed = MOVE_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CFireBoss::ATT_MOVE_DOWN:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 3;
			m_tFrame.dwFrameSpeed = MOVE_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CFireBoss::ATT_MOVE_UP:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 4;
			m_tFrame.dwFrameSpeed = MOVE_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CFireBoss::ATT_KICK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 10;
			m_tFrame.iFrameScene = 8;
			m_tFrame.dwFrameSpeed = KICK_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			m_bHittable = false;
			break;
		case CFireBoss::ATT_JUMP:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iFrameScene = 9;
			m_tFrame.dwFrameSpeed = JUMP_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			m_bHittable = false;
			break;
		default:
			break;
		}
		m_eFIREPreState = m_eFIRECurState;
	}
}

void CFireBoss::Move_Frame()
{
	static int iDanceCount = 0;

	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();

		if (m_eFIREPreState == FIRE_DEAD)
		{
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_bDead = true;
		}
		else if (m_eFIREPreState == FIRE_IDLE)
		{
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				if (m_iPatternCnt >= CFireBoss::P_END)
				{
					m_eFIRECurState = CFireBoss::FIRE_DANCE;
					m_iPatternCnt = 0;
					for (int i = 0; i < 30; ++i)
					{
						int  _left = rand() % CFireBoss::P_END;
						int  _right = rand() % CFireBoss::P_END;
						Swap(&m_iPatternArr[_left], &m_iPatternArr[_right]);
					}
				}
				else
				{
					if (!m_pTarget)
						m_tFrame.iFrameStart = 0;
					else
						m_eFIRECurState = CFireBoss::ATT_FIREBALL_RIGHT;
				}
			}
		}
		else if (m_eFIREPreState == FIRE_HIT)
		{
			if (m_tFrame.iFrameStart == 0)
				m_tFrame.dwFrameSpeed = HIT_FRAME_SPEED;
			else
				m_tFrame.dwFrameSpeed = (DWORD)(HIT_FRAME_SPEED * 1.5f);
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				++m_iHitStateCount;
				if (m_iHitStateCount > B_HIT_FRAME_COUNTMAX)
				{
					m_eFIRECurState = FIRE_IDLE;
					m_iHitStateCount = 0;
				}
				else
				{
					m_tFrame.iFrameStart = 0;
				}
			}
		}
		else if (m_eFIREPreState == FIRE_DANCE)
		{
			m_tFrame.dwFrameSpeed = DANCE_FRAME_SPEED;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd) {
				++iDanceCount;
				if (iDanceCount > B_DANCE_COUNTMAX) {
					m_eFIRECurState = FIRE_IDLE;
					iDanceCount = 0;
				}
				else {
					m_tFrame.iFrameStart = 0;
				}
			}
		}
		else if ((m_eFIREPreState >= ATT_DRAGON_RIGHT && m_eFIREPreState <= ATT_FIREBALL_UP) ||
			(m_eFIREPreState >= ATT_FIREBALL_DOWN && m_eFIREPreState <= ATT_JUMP))
		{
			if (ATT_FIREBALL_DOWN == m_eFIREPreState || ATT_FIREBALL_UP == m_eFIREPreState ||
				ATT_FIREBALL_RIGHT == m_eFIREPreState)
			{
				m_tFrame.dwFrameSpeed = FIREBALL_FRAME_SPEED;
				if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				{
					m_tFrame.iFrameStart = 0;
				}
			}
			else if (ATT_DRAGON_UP == m_eFIREPreState || ATT_DRAGON_DOWN == m_eFIREPreState ||
				ATT_DRAGON_RIGHT == m_eFIREPreState)
			{
				if (m_tFrame.iFrameStart == 1)
				{
					if (!m_bDragonFire)
					{
						m_bDragonFire = true;
						++m_tFrame.iFrameStart;
					}
				}

				if (m_bDragonFire && m_tFrame.iFrameStart == 2)
					--m_tFrame.iFrameStart;
				if (!m_bDragonFire && m_tFrame.iFrameStart >= 2)
				{
					m_eFIRECurState = FIRE_IDLE;
					m_fDragonTime = 0.f;
					m_tFrame.iFrameStart = 0;
				}

			}
			else if (ATT_MOVE_RIGHT <= m_eFIREPreState && m_eFIREPreState <= ATT_KICK)
			{
				if (ATT_MOVE_RIGHT == m_eFIREPreState || ATT_MOVE_UP == m_eFIREPreState || ATT_MOVE_DOWN == m_eFIREPreState)
				{
					if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd) {
						m_eFIRECurState = ATT_KICK;
						m_bMoveStateFlag = true;
					}
				}
				else if (ATT_KICK == m_eFIREPreState)
				{
					if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
					{
						m_tFrame.dwFrameSpeed = KICK_FRAME_SPEED * 5;
					}
					else
						m_tFrame.dwFrameSpeed = KICK_FRAME_SPEED;

					if (m_tFrame.iFrameStart == 1)
					{
						if (!m_bKickFire)
						{
							m_bKickFire = true;
						}
					}
					if (m_bKick_bulletCreate)
						m_bKick_bulletCreate = false;

					if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
					{
						m_eFIRECurState = FIRE_IDLE;
						m_fDragonTime = 0.f;
						m_tFrame.iFrameStart = 0;
					}
				}
			}
			else if (ATT_JUMP == m_eFIREPreState)
			{
				if (m_tFrame.iFrameStart == 1)
				{
					m_tFrame.dwFrameSpeed = DWORD(JUMP_FRAME_SPEED);
					if (!m_bJump)
					{
						m_bJump = true;
						m_fJumpLandY = m_tInfo.fY + (float)(m_tInfo.iCY >> 1);
					};
				}
				if (m_bJump && m_tFrame.iFrameStart == 2)
					--m_tFrame.iFrameStart;
				if (!m_bJump && m_tFrame.iFrameStart == 2)
				{
					if (!m_bMeteoFire)
					{
						m_bMeteoFire = true;
						m_tFrame.iFrameStart = 3;
					}
				}
				if (!m_bJump && m_bMeteoFire && m_tFrame.iFrameStart == 3)
					--m_tFrame.iFrameStart;
				if (!m_bJump && !m_bMeteoFire && m_tFrame.iFrameStart >= 3)
				{
					m_eFIRECurState = FIRE_IDLE;
					m_tFrame.iFrameStart = 0;
					m_fMeteoTime = 0.f;
				}
			}

			if (m_iPatternCnt >= CFireBoss::P_END)
			{
				m_eFIRECurState = FIRE_DANCE;
				m_iPatternCnt = 0;
				m_fPosinAng = 0.f;
				for (int i = 0; i < 30; ++i)
				{
					int  _left = rand() % CFireBoss::P_END;
					int  _right = rand() % CFireBoss::P_END;
					Swap(&m_iPatternArr[_left], &m_iPatternArr[_right]);
				}
			}

		}
	}
}

CObj * CFireBoss::Create_Bullet(const string & _frameKey)
{
	return nullptr;
}

void CFireBoss::Jumping(float _fdTime, float _fAngle)
{
	// 105.f 왼쪽, 		75.f; 

	if (_fAngle > 80.f && _fAngle <= 100.f)
		m_fJumpDirAngle = 90.f;
	else if (_fAngle > 100.f && _fAngle <= 260.f)
		m_fJumpDirAngle = 105.f;
	else if (_fAngle > 260.f && _fAngle <= 280.f)
		m_fJumpDirAngle = 90.f;
	else if ((_fAngle > 280.f && _fAngle < 360.f) || (_fAngle >= 0.f && _fAngle <= 80.f))
		m_fJumpDirAngle = 75.f;


	if (m_bJump && m_eFIREPreState != ATT_JUMP)
	{
		m_bJump = false;
		m_fJumpTime = 0.f;
		return;
	}

	if (m_bJump)
	{
		float fRad = Degree_To_Radian(m_fJumpDirAngle);
		float beforeY = m_tInfo.fY;
		float fDeltaY = 0.f;

		m_tInfo.fX += m_fJumpPower * (3.f * m_fJumpTime) * cosf(fRad);
		m_tInfo.fY -= m_fJumpPower * (3.f * m_fJumpTime) * sinf(fRad) - 0.5f * GRAVITY * (3.f * m_fJumpTime) * (3.f * m_fJumpTime);
		fDeltaY = m_tInfo.fY - beforeY;

		m_fJumpTime += _fdTime; // 이게 안되면은 _fdTime 받아와서 여기에 넣어줘야함....
		
		Equalize_HitPosInfoPos();
		Update_Rect();
		Update_HitRect();

		// 착륙조건... + 타일 충돌 계산.
		bool bCollision;
		float fCollisionX = 0.f, fCollisionY = 0.f;

		bCollision = CCollisionMgr::Collision_Obj_Tile(this, &fCollisionX, &fCollisionY);

		if (bCollision)
		{
			if (fCollisionY > fCollisionX && fDeltaY > 0.f)
			{
				m_bJump = false;
				m_fJumpTime = 0.f;
			}
		}
		else if (m_fJumpLandY < m_tInfo.fY + (m_tInfo.iCY >> 1))
		{
			m_bJump = false;
			m_fJumpTime = 0.f;
			m_tInfo.fY = m_fJumpLandY - (float)(m_tInfo.iCY >> 1);
			Equalize_HitPosInfoPos();
		}
	}
}

void CFireBoss::Change_HitRect()
{
	if (m_eFIREPreState == FIRE_DANCE)
	{
		switch (m_tFrame.iFrameStart)
		{
		case 0:
		case 5:
			m_tHitInfo.iCX = 100;
			m_tHitInfo.iCY = 100;
			break;
		case 1:
		case 3:
		case 6:
		case 8:
			m_tHitInfo.iCX = 108;
			m_tHitInfo.iCY = 96;
			break;
		case 2:
		case 4:
		case 7:
		case 9:
			m_tHitInfo.iCX = 116;
			m_tHitInfo.iCY = 92;
			break;
		}
	}
	else
	{
		m_tHitInfo.iCX = 68;
		m_tHitInfo.iCY = 152;
	}
}

void CFireBoss::Attack_Pattern(float _fdTime, float _fDis)
{
	//&& m_eFIREPreState != FIRE_IDLE
	if (m_eFIREPreState != DEAD && m_eFIREPreState != FIRE_DANCE  && m_eFIREPreState != HIT && m_eFIREPreState != FIRE_IDLE)
	{
		if (P_FIREBALL == m_ePattern)
		{
			switch (m_eDir)
			{
			case CBoss::LEFT:
				m_eFIRECurState = ATT_FIREBALL_RIGHT;
				m_tFrame.iFrameEnd = 1;
				break;
			case CBoss::TOP:
				m_eFIRECurState = ATT_FIREBALL_UP;
				m_tFrame.iFrameEnd = 1;
				break;
			case CBoss::RIGHT:
				m_eFIRECurState = ATT_FIREBALL_RIGHT;
				m_tFrame.iFrameEnd = 1;
				break;
			case CBoss::BOTTOM:
				m_eFIRECurState = ATT_FIREBALL_DOWN;
				m_tFrame.iFrameEnd = 1;
				break;
			}

			if (!m_bAttackCool && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			{
				++m_iAttackCnt;
				if (m_iAttackCnt <= m_iAttackCntLimit[m_ePattern]) {
					float fX = m_tInfo.fX + cosf(Degree_To_Radian(60.f * m_iAttackCnt)) * 200.f;
					float fY = m_tInfo.fY - sinf(Degree_To_Radian(60.f * m_iAttackCnt)) * 200.f;
					CObjMgr::Get_Instance()->Add_Object(OBJID::M_RECTBULLET,
						Create_Bullet<CFireBall>(fX, fY, "FireBall", m_fAngle));
					m_bAttackCool = true;
				}
				else {
					++m_iPatternCnt;
					m_iAttackCnt = 0;
					m_eFIRECurState = FIRE_IDLE;
					m_tFrame.iFrameStart = 0;
				}
			}
		}

		if (P_METEOR == m_ePattern)
		{
			m_eFIRECurState = ATT_JUMP;
			m_tFrame.iFrameEnd = 2;
			std::random_device rd;
			std::default_random_engine generator(rd());
			std::uniform_real_distribution<float> distribution(-330.f, 330.f);

			if (!m_bAttackCool && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			{
				if (m_bMeteoFire) // m_bMeteo
				{
					if (m_fBulletTick[P_METEOR] > m_fBulletTickLimit[P_METEOR])
					{
						m_fBulletTick[P_METEOR] = 0.f;
						float fOffsetX = distribution(generator);
						float fOffsetY = distribution(generator) - METEOR_POSY_START;
						CObj* meteor = Create_Bullet<CMeteor>(m_pTarget->Get_PosX() + fOffsetX,
							m_pTarget->Get_PosY() + fOffsetY, "Meteor");
						CObjMgr::Get_Instance()->Add_Object(OBJID::M_CIRBULLET, meteor);
					}
				}
				else if (!m_bMeteoFire && m_tFrame.iFrameStart >= 2)
				{
					++m_iPatternCnt;
					m_iAttackCnt = 0;
					m_bAttackCool = true;
				}
			}
		}
		if (P_DRAGON == m_ePattern)
		{
			float fPosX = 0.f, fPosY = 0.f;
			float fShotAngle = 0.f;
			switch (m_eDir)
			{
			case CBoss::LEFT:
				m_eFIRECurState = ATT_DRAGON_RIGHT;
				m_tFrame.iFrameEnd = 1;
				fPosX = m_tRect.left - 16.f;
				fPosY = m_tInfo.fY;
				fShotAngle = 180.f;
				break;
			case CBoss::TOP:
				m_eFIRECurState = ATT_DRAGON_UP;
				m_tFrame.iFrameEnd = 1;
				fPosX = m_tInfo.fX;
				fPosY = m_tRect.top - 12.f;
				fShotAngle = 90.f;
				break;
			case CBoss::RIGHT:
				m_eFIRECurState = ATT_DRAGON_RIGHT;
				m_tFrame.iFrameEnd = 1;
				fPosX = m_tRect.right + 16.f;
				fPosY = m_tInfo.fY;
				fShotAngle = 0.f;
				break;
			case CBoss::BOTTOM:
				m_eFIRECurState = ATT_DRAGON_DOWN;
				m_tFrame.iFrameEnd = 1;
				fPosX = m_tInfo.fX;
				fPosY = m_tRect.bottom + 12.f;
				fShotAngle = 270.f;
				break;
			}

			if (!m_bAttackCool && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			{
				if (m_bDragonFire)
				{
					if (m_fBulletTick[P_DRAGON] > m_fBulletTickLimit[P_DRAGON])
					{
						m_fBulletTick[P_DRAGON] = 0.f;

						if (fShotAngle == 90.f || fShotAngle == 270.f)
						{
							for (int i = -1; i < 1; ++i)
							{
								CObj* pDragonBullet = Create_Bullet<CScrewBullet>(fPosX + i * 80, fPosY, "DragonArc",
									m_fAngle);
								static_cast<CBullet*>(pDragonBullet)->Set_bMonsters(true);
								CObjMgr::Get_Instance()->Add_Object(OBJID::M_RECTBULLET, pDragonBullet);

								pDragonBullet = Create_Bullet<CScrewBullet>(fPosX + i * 80, fPosY, "DragonArc",
									m_fAngle);
								static_cast<CBullet*>(pDragonBullet)->Set_bMonsters(true);
								static_cast<CScrewBullet*>(pDragonBullet)->Set_Rotation_Dir(true);
								CObjMgr::Get_Instance()->Add_Object(OBJID::M_RECTBULLET, pDragonBullet);
							}
						}
						else
						{
							for (int i = -1; i < 1; ++i)
							{
								CObj* pDragonBullet = Create_Bullet<CScrewBullet>(fPosX, fPosY + i * 80, "DragonArc",
									m_fAngle);
								static_cast<CBullet*>(pDragonBullet)->Set_bMonsters(true);
								CObjMgr::Get_Instance()->Add_Object(OBJID::M_RECTBULLET, pDragonBullet);
								pDragonBullet = Create_Bullet<CScrewBullet>(fPosX, fPosY + i * 80, "DragonArc",
									m_fAngle);
								static_cast<CBullet*>(pDragonBullet)->Set_bMonsters(true);
								static_cast<CScrewBullet*>(pDragonBullet)->Set_Rotation_Dir(true);
								CObjMgr::Get_Instance()->Add_Object(OBJID::M_RECTBULLET, pDragonBullet);
							}
						}
					}
				}
				else
				{
					m_iAttackCnt = 0;
					m_bAttackCool = true;
					++m_iPatternCnt;
				}
			}
		}

		if (P_KICK == m_ePattern)
		{
			if (m_eDir == CBoss::LEFT || m_eDir == CBoss::RIGHT)
			{
				if (floor(_fDis) > 116.f && !m_bMoveStateFlag)
				{
					m_eFIRECurState = ATT_MOVE_RIGHT;
					m_tFrame.iFrameEnd = 0;
				}
			}
			else if (m_eDir == CBoss::TOP)
			{
				if (floor(_fDis) > 144.f && !m_bMoveStateFlag)
				{
					m_eFIRECurState = ATT_MOVE_UP;
					m_tFrame.iFrameEnd = 0;
				}
			}
			else if (m_eDir == CBoss::BOTTOM)
			{
				if (floor(_fDis) > 144.f && !m_bMoveStateFlag)
				{
					m_eFIRECurState = ATT_MOVE_DOWN;
					m_tFrame.iFrameEnd = 0;
				}
			}

			if (m_bMoveStateFlag && m_eFIREPreState != ATT_KICK)
				m_eFIRECurState = ATT_KICK;

			if (m_eFIREPreState == ATT_KICK)
			{
				float fPosX = 0.f, fPosY = 0.f;
				switch (m_eDir)
				{
				case CBoss::LEFT:
					fPosX = m_tRect.left - 45.f;
					fPosY = m_tInfo.fY;
					break;
				case CBoss::TOP:
					fPosX = m_tInfo.fX;
					fPosY = m_tRect.top - 30.f;
					break;
				case CBoss::RIGHT:
					fPosX = m_tRect.right + 45.f;
					fPosY = m_tInfo.fY;
					break;
				case CBoss::BOTTOM:
					fPosX = m_tInfo.fX;
					fPosY = m_tRect.bottom + 30.f;
					break;
				}

				if (!m_bAttackCool)
				{
					if (m_bKickFire &&
						(m_tFrame.iFrameStart == 1) || (m_tFrame.iFrameStart == 4) ||
						(m_tFrame.iFrameStart == 6)) // m_bMeteo
					{
						if (!m_bKick_bulletCreate)
						{
							CObj* pFireKick = Create_Bullet<CFireKick>(fPosX, fPosY, "FireKick", this);
							CObjMgr::Get_Instance()->Add_Object(OBJID::M_CIRBULLET, pFireKick);
							m_bKick_bulletCreate = true;
						}
					}
					else if (m_bKickFire && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
					{
						m_bKickFire = false;
						m_bMoveStateFlag = false;
						++m_iPatternCnt;
						m_iAttackCnt = 0;
						m_fBulletTick[P_KICK] = 0.f;
						m_bAttackCool = true;
					}
				}
			}
			
		}
	}
}

//if (m_iPatternCnt >= tempnum)
//{
//	m_eFIRECurState = CFireBoss::FIRE_DANCE;
//	m_iPatternCnt = 0;
//	for (int i = 0; i < 30; ++i)
//	{
//		int  _left = rand() % CFireBoss::P_END;
//		int  _right = rand() % CFireBoss::P_END;
//		Swap(&m_iPatternArr[_left], &m_iPatternArr[_right]);
//	}
//}
//else if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd) {
//	if (!m_pTarget)
//		m_tFrame.iFrameStart = 0;
//	else
//		m_eFIRECurState = CFireBoss::ATT_FIREBALL_RIGHT;
//}