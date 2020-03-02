#include "../stdafx.h"
#include "SummonerBall.h"
#include "../MyBitmap/BmpMgr.h"
#include "ObjMgr.h"

using Monster_space::HIT_FRAME_SPEED;
const DWORD CSummonerBall::ATT_FRAME_SPEED = 140;

// IDLE -> 속도 200.f, 어택 -> 속도 1000.f

CSummonerBall::CSummonerBall()
	: m_iAtt(8), m_fMoveTime(0.f)
{
	ZeroMemory(m_tRotPoint, sizeof(m_tRotPoint));
	ZeroMemory(&m_tPosin, sizeof(m_tPosin));
}

CSummonerBall::~CSummonerBall()
{
	Release();
}

bool CSummonerBall::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/Summoner/SummonerBall.bmp", "SummonerBall"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/Summoner/rot_80.bmp", "SumBall_rot"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/Summoner/empty_80.bmp", "SumBall_empty"))
		return false;

	CMonster::Initialize();

	m_tInfo.iCX = 80;
	m_tInfo.iCY = 80;
	Equalize_HitPosInfoPos();
	m_tHitInfo.iCX = 40;
	m_tHitInfo.iCY = 40;

	m_iMaxHp = 140;
	m_iHp = m_iMaxHp;

	m_fSpeed = 160.f;
	m_fRange = 580.f;
	
	m_fEndX = 0.f;
	m_fEndY = 0.f;
	m_fFireDis = 0.f;
	m_fPosinRad = 0.f;

	m_fDetectRange = 410.f;
	if (m_strFrameKey == "")
		m_strFrameKey = string("SummonerBall");

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 1000;
	m_tFrame.dwFrameTime = GetTickCount();
	return true;
}

int CSummonerBall::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;

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

		if (m_eCurState == IDLE)
		{
			m_fMoveTime += _fdTime;
			m_tPosin.x = LONG(m_tInfo.fX + cosf(fRadian) * m_fRange);
			m_tPosin.y = LONG(m_tInfo.fY - sinf(fRadian) * m_fRange);
			if (m_fMoveTime <= 3.3f) {
				MoveAngle(_fdTime);
			}
			else
			{
				m_fEndX = (float)m_tPosin.x;
				m_fEndY = (float)m_tPosin.y;
				m_eCurState = ATTACK;
				m_fMoveTime = 0.f;
			}
		}
		else
		{
			if (!m_bAttackCool && m_tFrame.iFrameStart == m_tFrame.iFrameEnd) {
				fDX = (float)(m_fEndX - m_tInfo.fX);
				fDY = (float)(m_fEndY - m_tInfo.fY);
				m_fFireDis = sqrtf(fDX * fDX + fDY * fDY);
				m_fPosinRad = acosf(fDX / m_fFireDis);
				if (0 < fDY)
					m_fPosinRad = 2 * PI - m_fPosinRad;

				if (m_fFireDis > 1.000f)
				{
					m_tInfo.fX += cosf(m_fPosinRad) * (1300.f) * _fdTime;
					m_tInfo.fY -= sinf(m_fPosinRad) * (1300.f) * _fdTime;
				}
				else {
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

void CSummonerBall::Late_Update(float _fdTime)
{
	if (0 >= m_iHp)
	{
		m_eCurState = CMonster::DEAD;
		m_eMez = MEZ::MZ_NONE;
		m_tHitInfo.iCX = 0;
		m_tHitInfo.iCY = 0;
	}

	if (m_iHitDigitCnt > HIT_DIGIT_CNT_MAX || m_ePreState != CMonster::HIT)
		Reset_HitDigitCnt();

	Update_Rect();
	Update_HitRect();

	float	fDis = sqrtf(float((m_tInfo.iCX >> 1) * (m_tInfo.iCX >> 1) + (m_tInfo.iCY >> 1) * (m_tInfo.iCY >> 1)));
	m_tRotPoint[0].x = LONG((m_tInfo.iCX >> 1) + (cosf((m_fAngle + 135.f) * PI / 180.f) * fDis));
	m_tRotPoint[0].y = LONG((m_tInfo.iCY >> 1) - (sinf((m_fAngle + 135.f) * PI / 180.f) * fDis));
	m_tRotPoint[1].x = LONG((m_tInfo.iCX >> 1) + (cosf((m_fAngle + 45.f) * PI / 180.f) * fDis));
	m_tRotPoint[1].y = LONG((m_tInfo.iCY >> 1) - (sinf((m_fAngle + 45.f) * PI / 180.f) * fDis));
	m_tRotPoint[2].x = LONG((m_tInfo.iCX >> 1) + (cosf((m_fAngle + 225.f) * PI / 180.f) * fDis));
	m_tRotPoint[2].y = LONG((m_tInfo.iCY >> 1) - (sinf((m_fAngle + 225.f) * PI / 180.f) * fDis));
}

void CSummonerBall::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);
	HDC hRotDC = CBmpMgr::Get_Instance()->Find_Image("SumBall_rot");
	HDC hEmpDC = CBmpMgr::Get_Instance()->Find_Image("SumBall_empty");

	PlgBlt(hRotDC, m_tRotPoint,
		hMemDC,
		m_tFrame.iFrameStart * m_tInfo.iCX,
		m_tFrame.iFrameScene * m_tInfo.iCY,
		m_tInfo.iCX, m_tInfo.iCY,
		NULL, NULL, NULL);

	GdiTransparentBlt(_DC,
		(int)(m_tRect.left + _fScrollX),
		(int)(m_tRect.top + _fScrollY),
		m_tInfo.iCX, m_tInfo.iCY,
		hRotDC,
		0,
		0,
		m_tInfo.iCX, m_tInfo.iCY,
		MAGENTA_COLOR);

	BitBlt(hRotDC, 0, 0, m_tInfo.iCX, m_tInfo.iCY, hEmpDC, 0, 0, SRCCOPY);

	if (m_tFrame.iFrameScene == 2 && m_tFrame.iFrameStart != m_tFrame.iFrameEnd)
	{
		MoveToEx(_DC, (int)(m_tInfo.fX + _fScrollX), (int)(m_tInfo.fY + _fScrollY), NULL);
		LineTo(_DC, (int)(m_tPosin.x + _fScrollX), (int)(m_tPosin.y + _fScrollY));
	}

	Draw_HitBox(_DC, _fScrollX, _fScrollY);
	Draw_DetectCircle(_DC, _fScrollX, _fScrollY);

	TCHAR szText[64] = L"";
	HDC hOneTileDC = GetDC(g_hWnd);
	Rectangle(hOneTileDC, 1030, 110, 1500, 190);
	swprintf_s(szText, L"m_bAttackCool: %d, 이전상태: %d, 현상태: %d, 거리 %.4f", m_bAttackCool, (int)m_ePreState, (int)m_eCurState, m_fFireDis);
	TextOut(hOneTileDC, 1060, 150, szText, lstrlen(szText));
	ReleaseDC(g_hWnd, hOneTileDC);
}

void CSummonerBall::Move_Frame()
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
		else if (m_ePreState == CMonster::IDLE)
		{
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_tFrame.iFrameStart = 0;
		}
		else if (m_ePreState == CMonster::ATTACK)
		{
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_tFrame.iFrameStart = m_tFrame.iFrameEnd;

			if (m_bAttackCool && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			{
				m_eCurState = CMonster::IDLE;
				m_bAttackCool = false;
			}
		}
		else if (m_ePreState == CMonster::HIT)
		{
			if (m_tFrame.iFrameStart == 0)
				m_tFrame.dwFrameSpeed = HIT_FRAME_SPEED;
			else
				m_tFrame.dwFrameSpeed = (DWORD)(HIT_FRAME_SPEED * 1.5f);

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
}

void CSummonerBall::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CMonster::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CMonster::WALK:
			m_eCurState = IDLE;
			return;
		case CMonster::ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 2;
			m_tFrame.dwFrameSpeed = ATT_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CMonster::HIT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameSpeed = HIT_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CMonster::DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameSpeed = HIT_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CMonster::FALL:
			m_eCurState = IDLE;
			return;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CSummonerBall::Change_HitRect()
{
}

CObj * CSummonerBall::Create_Bullet(const string & _frameKey)
{
	return nullptr;
}
