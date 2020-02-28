#include "../stdafx.h"
#include "FireBoss.h"
#include "../MyBitmap/BmpMgr.h"
#include "ObjMgr.h"
#include "FireBall.h"
#include "../Manager/CollisionMgr.h"

namespace FIREBOSS_Space
{
	const DWORD HIT_FRAME_SPEED = 80;
	const DWORD IDLE_TIME = 300;
	const DWORD DEAD_FRAME_SPEED = 1300;
	const DWORD DANCE_FRAME_SPEED = 160;

	// const DWORD METEOR_FRAME_SPEED = 100; -> 중력과 관련이 크다!

	const DWORD DRAGON_FRAME_SPEED = 100;
	const DWORD FIREBALL_FRAME_SPEED = 100;
	const DWORD KICK_FRAME_SPEED = 100;
	const DWORD JUMP_FRAME_SPEED = 800;			// JUMP -> METEOR 소환
}

using namespace FIREBOSS_Space;

CFireBoss::CFireBoss()
	: m_bMove(false), m_fPosinRange(110.f)
{
	ZeroMemory(&m_tPosin, sizeof(m_tPosin));
	ZeroMemory(&m_tStartPt, sizeof(LINEPOS));
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
	m_iHitCount = 0;
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

	m_iAttackCntLimit[P_FIREBALL] = 5;
	m_iAttackCntLimit[P_DRAGON] = 1;
	m_iAttackCntLimit[P_KICK] = 1;
	m_iAttackCntLimit[P_METEOR] = 1;

	for (int i = 0; i < CFireBoss::P_END; ++i)
		m_iPatternArr[i] = i;

	for (int i = 0; i < 30; ++i)
	{
		int  _left = rand() % CFireBoss::P_END;
		int  _right = rand() % CFireBoss::P_END;
		Swap(&m_iPatternArr[_left], &m_iPatternArr[_right]);
	}

	m_fDetectRange = 200.f;
	m_fJumpPower = 15.f;
	m_bJump = false;
	m_bMeteoFire = false;
	m_fJumpTime = 0.f;
	m_fJumpDirAngle = 90.f;
	m_fJumpLandY = m_tInfo.fY + (float)(m_tInfo.iCY >> 1);

	m_fMoveDist = 0.f;

	m_fBulletTick = 0.f;
	m_fBulletTickLimit[P_FIREBALL] = 0.15f;
	m_fBulletTickLimit[P_DRAGON] = 0.15f;
	m_fBulletTickLimit[P_KICK] = 0.15f;
	m_fBulletTickLimit[P_METEOR] = 0.15f;

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
				{
					// 보스가 더 왼쪽
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

		m_tPosin.x = LONG(m_tHitInfo.fX + cosf(fRadian) * m_fPosinRange);
		m_tPosin.y = LONG(m_tHitInfo.fY - sinf(fRadian) * m_fPosinRange);

		//m_ePattern = (CFireBoss::PATTERN)m_iPatternArr[m_iPatternCnt];
		m_ePattern = P_METEOR;
		Attack_Pattern(_fdTime, fDis);
	}

	Jumping(_fdTime, 90.f);
	Equalize_HitPosInfoPos();

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
	//TCHAR szBuff[32] = L"";
	//swprintf_s(szBuff, L"각도(%.2f), 총알 수 : %d, 방향: %d", m_fAngle, 
	//	CObjMgr::Get_Instance()->Get_listObj(OBJID::M_RECTBULLET).size(), (int)m_eDir);
	//SetWindowText(g_hWnd, szBuff);
	Draw_HitBox(_DC, _fScrollX, _fScrollY);
	Draw_DetectCircle(_DC, _fScrollX, _fScrollY);
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
			m_tFrame.iFrameEnd = 1;				// 3번째 착지하는 사진 지운다!
			m_tFrame.iFrameScene = 2;
			m_tFrame.dwFrameSpeed = DRAGON_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			m_bHittable = false;
			break;
		case CFireBoss::ATT_DRAGON_DOWN:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 3;
			m_tFrame.dwFrameSpeed = 3 * DRAGON_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			m_bHittable = false;
			break;
		case CFireBoss::ATT_DRAGON_UP:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 4;
			m_tFrame.dwFrameSpeed = 3 * DRAGON_FRAME_SPEED;
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
			if (m_iPatternCnt >= 1)
			{
				m_eFIRECurState = FIRE_DANCE;
				m_iPatternCnt = 0;
				for (int i = 0; i < 30; ++i)
				{
					int  _left = rand() % CFireBoss::P_END;
					int  _right = rand() % CFireBoss::P_END;
					Swap(&m_iPatternArr[_left], &m_iPatternArr[_right]);
				}
			}
			else if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd) {
				if (!m_pTarget)
					m_tFrame.iFrameStart = 0;
				else
					m_eFIRECurState = ATT_FIREBALL_RIGHT;
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
				++m_iHitCount;
				if (m_iHitCount > B_HIT_FRAME_COUNTMAX)
				{
					m_eFIRECurState = FIRE_IDLE;
					m_iHitCount = 0;
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
				if (iDanceCount > 1) {
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
			//int iBulletLimit = 0;
			if (ATT_FIREBALL_UP == m_eFIREPreState || 
				ATT_FIREBALL_DOWN == m_eFIREPreState || ATT_FIREBALL_RIGHT == m_eFIREPreState)
			{
				m_tFrame.dwFrameSpeed = FIREBALL_FRAME_SPEED;
				//iBulletLimit = m_iBulletCntLimit[P_BOX];
				if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
					m_tFrame.iFrameStart = 0;
			}
			else if (P_DRAGON == m_ePattern)
			{
				
			}
			else if (P_KICK == m_ePattern)
			{
				
			}
			else if (ATT_JUMP == m_eFIREPreState)
			{

				if (m_bJump && m_tFrame.iFrameStart == 2)
					--m_tFrame.iFrameStart;
				if (m_tFrame.iFrameStart == 2)
					m_tFrame.dwFrameSpeed = JUMP_FRAME_SPEED * 2;

				if (m_tFrame.iFrameStart == 1)
				{
					m_tFrame.dwFrameSpeed = DWORD(JUMP_FRAME_SPEED / 4.f);
					if (!m_bJump)
					{
						m_bJump = true;
						m_fJumpLandY = m_tInfo.fY + (float)(m_tInfo.iCY >> 1);
					};
				}
				if (!m_bJump && m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
					m_eFIRECurState = FIRE_IDLE;
			}

			if (m_iPatternCnt >= 1)
			{
				m_eFIRECurState = FIRE_DANCE;
				m_iPatternCnt = 0;
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

//_fLandY 필요 ㅇ벗음
void CFireBoss::Jumping(float _fdTime, float _fAngle)
{
	// 105.f 왼쪽, 		75.f; 
	//float m_fJumpAngle;
	//if (_fAngle > 80.f && _fAngle <= 100.f)
	//	m_fJumpAngle = 90.f;
	//else if (_fAngle > 100.f && _fAngle <= 260.f)
	//	m_fJumpAngle = 105.f;
	//else if (_fAngle > 260.f && _fAngle <= 280.f)
	//	m_fJumpAngle = 90.f;
	//else if ((_fAngle > 280.f && _fAngle < 360.f) || (_fAngle >= 0.f && _fAngle <= 80.f))
	//	m_fJumpAngle = 75.f;

	float fJumpAngle = 90.f;

	if (m_bJump)
	{
		float fRad = Degree_To_Radian(fJumpAngle);
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
}



//FIRE_IDLE, FIRE_DANCE, ATT_DRAGON_RIGHT, ATT_DRAGON_DOWN, ATT_DRAGON_UP, ATT_FIREBALL_RIGHT, ATT_FIREBALL_UP, FIRE_HIT, FIRE_DEAD,
//ATT_FIREBALL_DOWN, ATT_KICK, ATT_JUMP,

// P_FIREBALL, P_DRAGON, P_KICK, P_METEOR, P_END

void CFireBoss::Attack_Pattern(float _fdTime, float _fDis)
{
	if (m_eFIREPreState != DEAD && m_eFIREPreState != FIRE_DANCE && m_eFIREPreState != FIRE_IDLE && m_eFIREPreState != HIT)
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
					CObjMgr::Get_Instance()->Add_Object(OBJID::M_RECTBULLET,
						Create_Bullet<CFireBall>("FireBoss_Fireball"));
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
			if (!m_bAttackCool && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			{
				if (m_iAttackCnt <= m_iAttackCntLimit[m_ePattern]) {
					m_bAttackCool = true;
				}
				else
				{
					++m_iPatternCnt;
					m_iAttackCnt = 0;
					m_eFIRECurState = FIRE_IDLE;
					m_tFrame.iFrameStart = 0;
				}
			}
		}
	}
}