#include "../stdafx.h"
#include "Monster.h"
#include "Gold.h"
#include "ObjMgr.h"
const int CMonster::M_HIT_FRAME_COUNTMAX = 2;
using namespace Monster_space;

CMonster::CMonster()
	: m_bAttackCool(false), m_fDetectRange(DEFAULT_DETECT_RAD), m_iHitCount(0)
{
}

CMonster::~CMonster()
{
	CObjMgr::Get_Instance()->Add_Object(OBJID::GOODS,
		CAbstractFactory<CGold>::Create(m_tInfo.fX, m_tInfo.fY));
	Release();
}

bool CMonster::Initialize()
{
	CMoveObj::Initialize();
	m_fSpeed = 300.f;
	m_ePreState = END;
	m_eCurState = IDLE;

	return true;
}

int CMonster::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
	
	//// 순서
	// 1. 몬스터 움직임 설정하기
	//Equalize_HitPosInfoPos();
	//Move_Frame();
	//Scene_Change();
	//Change_HitRect(); 필수
	//Update_Rect();
	//Update_HitRect();
	//return OBJ_NOEVENT;
}

void CMonster::Late_Update(float _fdTime)
{
	// 떨어지는 것도 고려해야....

	if (0 >= m_iHp)
	{
		m_eCurState = CMonster::DEAD;
		m_eMez = MEZ::MZ_NONE;
		m_tHitInfo.iCX = 0;
		m_tHitInfo.iCY = 0;
	}
	if (m_eMez == MEZ::MZ_FROZEN)
	{
		m_tHitInfo.iCX = 0;
		m_tHitInfo.iCY = 0;
	}

	Update_Rect();
	Update_HitRect();
}

void CMonster::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
}

void CMonster::Release()
{
}

void CMonster::Draw_DetectCircle(HDC _DC, float _fScrollX, float _fScrollY)
{
	HPEN hOldPen, hMyPen;
	HBRUSH hOldBrush;
	hMyPen = CreatePen(PS_SOLID, 2, RGB(250, 230, 56));
	hOldPen = (HPEN)SelectObject(_DC, hMyPen);
	hOldBrush = (HBRUSH)SelectObject(_DC, GetStockObject(NULL_BRUSH));

	if (g_HitBox_On)
	{
		Ellipse(_DC,
			(int)(m_tInfo.fX - m_fDetectRange + _fScrollX),
			(int)(m_tInfo.fY - m_fDetectRange + _fScrollY),
			(int)(m_tInfo.fX + m_fDetectRange + _fScrollX),
			(int)(m_tInfo.fY + m_fDetectRange + _fScrollY));
	}
	SelectObject(_DC, hOldPen);
	SelectObject(_DC, hOldBrush);
	DeleteObject(hMyPen);
}

void CMonster::Move_Frame()
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
			// idle로 바꿔야 하나?
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
					++m_iHitCount;
					if (m_iHitCount > M_HIT_FRAME_COUNTMAX)
					{
						m_eCurState = CMonster::IDLE;
						m_iHitCount = 0;
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

void CMonster::Set_Monster_State(CMonster::MONSTATE _eNewState)
{
	if (m_eCurState != CMonster::DEAD)
		m_eCurState = _eNewState;
}