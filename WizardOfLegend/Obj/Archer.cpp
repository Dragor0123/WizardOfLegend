#include "../stdafx.h"
#include "Archer.h"
#include "../Obj/Arrow.h"
#include "../MyBitmap/BmpMgr.h"
#include "../Obj/ObjMgr.h"
#include "../Manager/SoundMgr.h"
using namespace Monster_space;

CArcher::CArcher()
{
	ZeroMemory(m_tRotPoint, sizeof(m_tRotPoint));
}

CArcher::~CArcher()
{
	Release();
}

bool CArcher::Initialize()
{
	// 몬스터 스피드 세팅
	// m_ePreState, m_eCurState 세팅, 크기 세팅
	CMonster::Initialize();
	m_tInfo.iCX = 160;
	m_tInfo.iCY = 160;
	Equalize_HitPosInfoPos();
	m_tHitInfo.iCX = 68;
	m_tHitInfo.iCY = 130;
	m_fSpeed = 330.f;
	
	// 체력 및 기본 전투 스탯 초기화
	m_iMaxHp = 300;
	m_iHp = m_iMaxHp;

	// 공격 주기 구하기

	////

	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/Archer/Archer_Right.bmp", "Archer"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/Archer/stretch160.bmp", "Archer_stretch"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/Archer/Bow.bmp", "Bow"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/Archer/rot150.bmp", "Bow_rot"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/Archer/empty150.bmp", "Bow_empty"))
		return false;

	// 기본 프레임 상태 설정
	m_strFrameKey = "Archer";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();

	return true;
}

int CArcher::Update(float _fdTime)
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

		if (fDis > 720.f)
		{
			MoveAngle(_fdTime);
			PLAY_SOUND(L"SWORDMAN_RUN.wav", CSoundMgr::MONSTER);
			m_eCurState = WALK;
			m_tFrame.iFrameEnd = 4;
		}
		else
		{
			if (m_eCurState != HIT && m_eCurState != FALL)
			{
				m_eCurState = ATTACK;
				m_tFrame.iFrameEnd = 3;
				// 어택하면 됨
				if (!m_bAttackCool && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
				{
					STOP_SOUND(CSoundMgr::MONSTER_EFFECT);
					PLAY_SOUND(L"Mon_ArcherAttackRelease.wav", CSoundMgr::MONSTER_EFFECT);
					CObjMgr::Get_Instance()->Add_Object(OBJID::M_RECTBULLET, Create_Bullet("Arrow"));
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

void CArcher::Late_Update(float _fdTime)
{
	CMonster::Late_Update(_fdTime);

	if ( m_bAttackCool &&
		 m_ePreState == CMonster::ATTACK && m_tFrame.iFrameStart == 0)
	{
		m_bAttackCool = false;
	}

	float	fDis = sqrtf(float((BOWSIZE >> 1) * (BOWSIZE >> 1) + (BOWSIZE >> 1) * (BOWSIZE >> 1)));
	m_tRotPoint[0].x = LONG((BOWSIZE >> 1) + (cosf((m_fAngle + 135.f) * PI / 180.f) * fDis));
	m_tRotPoint[0].y = LONG((BOWSIZE >> 1) - (sinf((m_fAngle + 135.f) * PI / 180.f) * fDis));
	m_tRotPoint[1].x = LONG((BOWSIZE >> 1) + (cosf((m_fAngle + 45.f) * PI / 180.f) * fDis));
	m_tRotPoint[1].y = LONG((BOWSIZE >> 1) - (sinf((m_fAngle + 45.f) * PI / 180.f) * fDis));
	m_tRotPoint[2].x = LONG((BOWSIZE >> 1) + (cosf((m_fAngle + 225.f) * PI / 180.f) * fDis));
	m_tRotPoint[2].y = LONG((BOWSIZE >> 1) - (sinf((m_fAngle + 225.f) * PI / 180.f) * fDis));
}

void CArcher::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);
	HDC hStrDC = CBmpMgr::Get_Instance()->Find_Image("Archer_stretch");
	HDC hBowMemDC = CBmpMgr::Get_Instance()->Find_Image("Bow");
	HDC hBowRotDC = CBmpMgr::Get_Instance()->Find_Image("Bow_rot");
	HDC hBowEmptyDC = CBmpMgr::Get_Instance()->Find_Image("Bow_empty");

	if (m_fAngle > 90.f && m_fAngle <= 270.f)
	{
		StretchBlt(hStrDC,
			0, 0, m_tInfo.iCX, m_tInfo.iCY, hMemDC,
			m_tFrame.iFrameStart * m_tInfo.iCX +m_tInfo.iCX,
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

	if (m_ePreState == CMonster::ATTACK)
	{
		int offSetX = 0, offSetY = 0;

		PlgBlt(hBowRotDC, m_tRotPoint,
			hBowMemDC,
			m_tFrame.iFrameStart * BOWSIZE,
			0,
			BOWSIZE, BOWSIZE,
			NULL, NULL, NULL);

		if ((0.f <= m_fAngle && m_fAngle < 40.f) ||
			(320.f <= m_fAngle && m_fAngle < 360.f))
			offSetX = 12, offSetY = 10;
		else if (40.f <= m_fAngle && m_fAngle < 60.f)
			offSetX = 12, offSetY = 5;
		else if (60.f <= m_fAngle && m_fAngle < 80.f)
			offSetX = 12, offSetY = 0;
		else if (80.f <= m_fAngle && m_fAngle < 90.f)
			offSetX = 12, offSetY = -5;
		else if (90.f <= m_fAngle && m_fAngle < 100.f)
			offSetX = -12, offSetY = -5;
		else if (100.f <= m_fAngle && m_fAngle < 120.f)
			offSetX = -12, offSetY = 0;
		else if (120.f <= m_fAngle && m_fAngle < 140.f)
			offSetX = -12, offSetY = 5;
		else if (140.f <= m_fAngle && m_fAngle < 220.f)
			offSetX = -12, offSetY = 10;
		else if (220.f <= m_fAngle && m_fAngle < 240.f)
			offSetX = -12, offSetY = 16;
		else if (240.f <= m_fAngle && m_fAngle < 260.f)
			offSetX = -8, offSetY = 22;
		else if (260.f <= m_fAngle && m_fAngle < 270.f)
			offSetX = -6, offSetY = 25;
		else if (270.f <= m_fAngle && m_fAngle < 280.f)
			offSetX = 12, offSetY = 25;
		else if (280.f <= m_fAngle && m_fAngle < 300.f)
			offSetX = 12, offSetY = 22;
		else if (300.f <= m_fAngle && m_fAngle < 320.f)
			offSetX = 12, offSetY = 16;

		GdiTransparentBlt(_DC,
			(int)(m_tRect.left + offSetX + _fScrollX),
			(int)(m_tRect.top + offSetY + _fScrollY),
			BOWSIZE, BOWSIZE,
			hBowRotDC,
			0,
			0,
			BOWSIZE, BOWSIZE,
			MAGENTA_COLOR);

		BitBlt(hBowRotDC, 0, 0, BOWSIZE, BOWSIZE, hBowEmptyDC, 0, 0, SRCCOPY);
	}

	Draw_HitBox(_DC, _fScrollX, _fScrollY);
	Draw_DetectCircle(_DC, _fScrollX, _fScrollY);
}

CObj* CArcher::Create_Bullet(const string & _frameKey)
{
	CObj* pObj = CAbstractFactory<CArrow>::Create(m_tHitInfo.fX, m_tHitInfo.fY,
		m_fAngle, _frameKey);
	return pObj;
}

void CArcher::Change_HitRect()
{
	switch (m_eCurState)
	{
	case CMonster::IDLE:
		m_tHitInfo.iCX = 68;
		m_tHitInfo.iCY = 130;
		break;
	case CMonster::WALK:
		m_tHitInfo.iCX = 52;
		m_tHitInfo.iCY = 130;
		break;
	case CMonster::ATTACK:
		m_tHitInfo.iCX = 75;
		m_tHitInfo.iCY = 110;
		break;
	case CMonster::HIT:
		m_tHitInfo.iCX = 75;
		m_tHitInfo.iCY = 124;
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

void CArcher::Scene_Change()
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
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameSpeed = WALK_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CMonster::ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 2;
			m_tFrame.dwFrameSpeed = 400;
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

void CArcher::Move_Frame()
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
			if (m_tFrame.iFrameStart == 1)
			{
				STOP_SOUND(CSoundMgr::MONSTER_EFFECT);
				PLAY_SOUND(L"Mon_ArcherAttackWindup.wav", CSoundMgr::MONSTER_EFFECT);
			}
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_tFrame.iFrameStart = 0;
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
				m_tFrame.iFrameStart = 0;
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
