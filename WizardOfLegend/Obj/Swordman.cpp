#include "../stdafx.h"
#include "Swordman.h"
#include "../MyBitmap/BmpMgr.h"
#include "ObjMgr.h"

using namespace Monster_space;

CSwordman::CSwordman()
{
}

CSwordman::~CSwordman()
{
	Release();
}

bool CSwordman::Initialize()
{
	CMonster::Initialize();
	m_tInfo.iCX = 200;
	m_tInfo.iCY = 220;
	Equalize_HitPosInfoPos();
	// idle일 때 히트박스
	m_tHitInfo.iCX = 92;
	m_tHitInfo.iCY = 136;
	
	m_iMaxHp = 300;
	m_iHp = m_iMaxHp;
	m_eAttDir = CMeeleBullet::END;
	
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/Swordman/Swordman_Right.bmp", "Swordman"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/Swordman/stretch200_220.bmp", "Swordman_stretch"))
		return false;

	m_strFrameKey = "Swordman";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();

	return true;
}

int CSwordman::Update(float _fdTime)
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

		if (m_ePreState == CMonster::HIT)
			m_pTarget = pPlayer;
	}
	else if (m_eCurState != CMonster::DEAD && m_eCurState != CMonster::HIT)
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
			if (fRealDX < fRealDY || (m_tHitRect.bottom < fPlayerY - (float)(m_pTarget->Get_HitInfo().iCY >> 1))
				|| ((m_tHitRect.top > fPlayerY + (float)(m_pTarget->Get_HitInfo().iCY >> 1)))) 
			{
				if (m_tInfo.fY < fPlayerY - (float)(m_pTarget->Get_HitInfo().iCY >> 1))
				{
					// 보스가 더 위에 있음.
					m_eAttDir = CMeeleBullet::BOTTOM;
				}
				else if (m_tInfo.fY > fPlayerY + (float)(m_pTarget->Get_HitInfo().iCY >> 1))
				{
					// 보스가 더 아래에
					m_eAttDir = CMeeleBullet::TOP;
				}
			}
			else //가로로 더 멀다.
			{
				if (m_tInfo.fX < fPlayerX - (float)(m_tHitInfo.iCX >> 1))
				{
					// 보스가 더 왼쪽
					m_eAttDir = CMeeleBullet::RIGHT;
				}
				else if (m_tInfo.fX > fPlayerX + (float)(m_tHitInfo.iCX >> 1))
				{ // 보스가 더 오른쪽
					m_eAttDir = CMeeleBullet::LEFT;
				}
			}
		}

		fRadian = acosf(fDX / fDis);

		if (m_tInfo.fY < m_pTarget->Get_Info().fY)
			fRadian = 2 * PI - fRadian;

		m_fAngle = fRadian * 180.f / PI;

		// 근접해야할 조건
		if (fRealDX >= fRealDY && floor(fDis) > 128.f)
		{
			MoveAngle(_fdTime);
			m_eCurState = WALK;
			m_tFrame.iFrameEnd = 5;
		}
		else if (fRealDX < fRealDY && floor(fDis) > 128.f)
		{
			MoveAngle(_fdTime);
			m_eCurState = WALK;
			m_tFrame.iFrameEnd = 5;
		}
		else 
		{
			// 공격 조건
			if (m_eCurState != HIT && m_eCurState != FALL)
			{
				m_eCurState = ATTACK;
				m_tFrame.iFrameEnd = 2;
				if (!m_bAttackCool && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
				{
					CObjMgr::Get_Instance()->Add_Object(OBJID::M_RECTBULLET, Create_Bullet("SwordmanAtt"));
					m_bAttackCool = true;
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

void CSwordman::Late_Update(float _fdTime)
{
	CMonster::Late_Update(_fdTime);

	if (m_bAttackCool &&
		m_ePreState == CMonster::ATTACK && m_tFrame.iFrameStart == 0)
	{
		m_bAttackCool = false;
	}
}

void CSwordman::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);
	HDC hStrDC = CBmpMgr::Get_Instance()->Find_Image("Swordman_stretch");
	
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

void CSwordman::Move_Frame()
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
			m_tFrame.dwFrameSpeed = 250;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				m_eCurState = IDLE;
			}
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
			else if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
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
		else if (m_ePreState == CMonster::WALK)
		{
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_tFrame.iFrameStart = 0;
		}
		else if (m_ePreState == CMonster::IDLE)
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

CObj * CSwordman::Create_Bullet(const string & _frameKey)
{
	CObj* pBullet = nullptr;
	float fOffsetX = 0.f, fOffsetY = 0.f;
	switch (m_eAttDir)
	{
	case CMeeleBullet::LEFT:
		fOffsetY = 0.f;
		fOffsetX = -(float)(m_tHitInfo.iCX >> 1) - 20.f;
		break;
	case CMeeleBullet::TOP:
		fOffsetY = -(float)(m_tHitInfo.iCY >> 1);
		fOffsetX = 0.f;
		break;
	case CMeeleBullet::RIGHT:
		fOffsetY = 0.f;
		fOffsetX = (float)(m_tHitInfo.iCX >> 1) + 20.f;
		break;
	case CMeeleBullet::BOTTOM:
		fOffsetY = (float)((m_tHitInfo.iCY >> 1));
		fOffsetX = 0.f;
		break;
	}
	pBullet = CAbstractFactory<CMeeleBullet>::Create(m_tHitInfo.fX + fOffsetX, m_tHitInfo.fY + fOffsetY,
		0.f, _frameKey);
	dynamic_cast<CMeeleBullet*>(pBullet)->Set_AttackDir(m_eAttDir);
	return pBullet;
}

void CSwordman::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CMonster::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CMonster::WALK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameSpeed = WALK_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CMonster::ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iFrameScene = 2;
			m_tFrame.dwFrameSpeed = 250;
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
			m_tFrame.iFrameEnd = 5;
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


void CSwordman::Change_HitRect()
{
	switch (m_eCurState)
	{
	case CMonster::IDLE:
		m_tHitInfo.iCX = 92;
		m_tHitInfo.iCY = 136;
		break;
	case CMonster::WALK:
		m_tHitInfo.iCX = 100;
		m_tHitInfo.iCY = 125;
		break;
	case CMonster::ATTACK:
		m_tHitInfo.iCX = 94;
		m_tHitInfo.iCY = 120;
		break;
	case CMonster::HIT:
		m_tHitInfo.iCX = 102;
		m_tHitInfo.iCY = 132;
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
