#include "../stdafx.h"
#include "EarthLoad.h"
#include "../MyBitmap/BmpMgr.h"
#include "ObjMgr.h"
#include "EarthBossBox.h"
#include "EarthDrill.h"
#include "EarthPillarMaker.h"
#include "NoMoveBullet.h"
#include "GuideLBullet.h"

// HIT FRAME_SPEED start 0번 : 100, 1번 : 200
namespace EARTHLOAD_space
{
	const DWORD HIT_FRAME_SPEED = 80;
	const DWORD IDLE_TIME = 550;
	const DWORD PILLAR_FRAME_SPEED = 200;
	const DWORD GUIDE_FRAME_SPEED = 50;
	const DWORD DEAD_FRAME_SPEED = 1300;
}

using namespace EARTHLOAD_space;

CEarthLoad::CEarthLoad()
	: m_fPosinRange(110.f)
{
	ZeroMemory(&m_tPosin, sizeof(m_tPosin));
}

CEarthLoad::~CEarthLoad()
{
	Release();
}

bool CEarthLoad::Initialize()
{
	CBoss::Initialize();

	m_tInfo.iCX = 180;
	m_tInfo.iCY = 220;
	Equalize_HitPosInfoPos();

	//IDLE일 때 히트박스
	m_tHitInfo.iCX = 106;
	m_tHitInfo.iCY = 180;
	m_iHitStateCount = 0;
	m_ePattern = CEarthLoad::P_BOX;
	m_eDir = BOSSDIR::BOTTOM;

	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/EarthBoss/EarthBoss.bmp", "EarthBoss"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/EarthBoss/stretch180_220.bmp", "EarthBoss_stretch"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/EarthBoss/EarthBoss_Box.bmp", "EarthBoss_Box"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/EarthBoss/EarthBoss_Pillar.bmp", "EarthBoss_Pillar"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/EarthBoss/GuideSword.bmp", "EarthBoss_Sword"))
		return false;

	m_strFrameKey = "EarthBoss";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = IDLE_TIME;
	m_tFrame.dwFrameTime = GetTickCount();

	for (int i = 0; i < CEarthLoad::P_END; ++i)
		m_iPatternArr[i] = i;

	m_iAttackCntLimit[P_BOX] = 5;
	m_iAttackCntLimit[P_DRILL] = 2;
	m_iAttackCntLimit[P_PILLAR] = 7;
	m_iAttackCntLimit[P_GUIDED] = 10;

	for (int i = 0; i < 30; ++i)
	{
		int  _left = rand() % CEarthLoad::P_END;
		int  _right = rand() % CEarthLoad::P_END;
		Swap(&m_iPatternArr[_left], &m_iPatternArr[_right]);
	}

	return true;
}

int CEarthLoad::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	// UPATE할 거 해준다.
	// 공격패턴 같은거...

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
	else if (m_eCurState != CBoss::DEAD)
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

		// 플레이어가 타겟으로 잡혔다.
		m_ePattern = (CEarthLoad::PATTERN)m_iPatternArr[m_iPatternCnt];
		// m_ePattern = P_DRILL;
		Attack_Pattern(_fdTime, fDis);
	}// m_pTarget != nullptr

	Equalize_HitPosInfoPos();
	//
	Move_Frame();
	Scene_Change();
	Change_HitRect();
	Update_Rect();
	Update_HitRect();
	return OBJ_NOEVENT;
}

// enum BOSSSTATE {IDLE, DANCE, ATT1, ATT2, ATT3_RIGHT, ATT3_BOT, ATT3_UP, HIT, DEAD, S_END};
void CEarthLoad::Late_Update(float _fdTime)
{
	CBoss::Late_Update(_fdTime);

	if (m_bAttackCool &&
		m_tFrame.iFrameStart == 0 &&
		(m_ePreState >= ATT1 && m_ePreState <= ATT3_UP))
	{
		m_bAttackCool = false;
	}
}

void CEarthLoad::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);
	HDC hStrDC = CBmpMgr::Get_Instance()->Find_Image("EarthBoss_stretch");

	if (m_ePreState != IDLE && m_ePreState != DEAD &&
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
}


void CEarthLoad::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CEarthLoad::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = IDLE_TIME;
			m_tFrame.dwFrameTime = GetTickCount();
			m_bHittable = false;
			break;
		case CEarthLoad::DANCE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameSpeed = 120;
			m_tFrame.dwFrameTime = GetTickCount();
			m_bHittable = true;
			break;
		case CEarthLoad::ATT1:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 2;

			if (m_ePattern == CEarthLoad::P_PILLAR)
				m_tFrame.dwFrameSpeed = PILLAR_FRAME_SPEED;
			else
				m_tFrame.dwFrameSpeed = 180;
			m_tFrame.dwFrameTime = GetTickCount();
			m_bHittable = false;
			break;
		case CEarthLoad::ATT2:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iFrameScene = 3;
			m_tFrame.dwFrameSpeed = GUIDE_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			m_bHittable = false;
			break;
		case CEarthLoad::ATT3_RIGHT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iFrameScene = 4;
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			m_bHittable = false;
			break;
		case CEarthLoad::ATT3_DOWN:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iFrameScene = 5;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			m_bHittable = false;
			break;
		case CEarthLoad::ATT3_UP:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iFrameScene = 6;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			m_bHittable = false;
			break;
		case CEarthLoad::HIT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 7;
			m_tFrame.dwFrameSpeed = HIT_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			m_bHittable = true;
			break;
		case CEarthLoad::DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 8;
			m_tFrame.dwFrameSpeed = DEAD_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			m_bHittable = false;
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CEarthLoad::Move_Frame()
{
	static int iDanceCount = 0;

	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();

		if (m_ePreState == CEarthLoad::DEAD)
		{
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_bDead = true;
		}
		else if (m_ePreState == CEarthLoad::IDLE)
		{
			if (m_iPatternCnt >= CEarthLoad::P_END)
			{
				m_eCurState = CEarthLoad::DANCE;
				m_iPatternCnt = 0;
				for (int i = 0; i < 30; ++i)
				{
					int  _left = rand() % CEarthLoad::P_END;
					int  _right = rand() % CEarthLoad::P_END;
					Swap(&m_iPatternArr[_left], &m_iPatternArr[_right]);
				}
			}
			else if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd) {
				if (!m_pTarget)
					m_tFrame.iFrameStart = 0;
				else
					m_eCurState = CEarthLoad::ATT1;
			}
		}
		else if (m_ePreState == CEarthLoad::HIT)
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
					m_eCurState = CEarthLoad::IDLE;
					m_iHitStateCount = 0;
				}
				else
				{
					m_tFrame.iFrameStart = 0;
				}
			}
		}
		else if (m_ePreState == CEarthLoad::DANCE)
		{
			m_tFrame.dwFrameSpeed = 120;
			if (m_tFrame.iFrameStart > 5 && m_tFrame.iFrameStart <= 7)
				m_tFrame.dwFrameSpeed = 200;
			if (m_tFrame.iFrameStart >= 8)
				m_tFrame.dwFrameSpeed = 260;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd) {
				++iDanceCount;
				if (iDanceCount > B_DANCE_COUNTMAX) {
					m_eCurState = CEarthLoad::ATT1;
					iDanceCount = 0;
				}
				else {
					m_tFrame.iFrameStart = 0;
				}
			}
		}
		else if (m_ePreState >= CEarthLoad::ATT1 && m_ePreState <= CEarthLoad::ATT3_UP)
		{
			//int iBulletLimit = 0;
			if (P_BOX == m_ePattern)
			{
				m_tFrame.dwFrameSpeed = 180;
				//iBulletLimit = m_iBulletCntLimit[P_BOX];
			}
			else if (P_DRILL == m_ePattern)
			{
				if (m_ePreState == CEarthLoad::ATT3_DOWN || m_ePreState == CEarthLoad::ATT3_UP)
					m_tFrame.dwFrameSpeed = 200;
				else
					m_tFrame.dwFrameSpeed = 100;
				//iBulletLimit = m_iBulletCntLimit[P_DRILL];
			}
			else if (P_PILLAR == m_ePattern)
			{
				m_tFrame.dwFrameSpeed = PILLAR_FRAME_SPEED;
				//iBulletLimit = m_iBulletCntLimit[P_PILLAR];
			}
			else if (P_GUIDED == m_ePattern)
			{
				m_tFrame.dwFrameSpeed = GUIDE_FRAME_SPEED;
				//iBulletLimit = m_iBulletCntLimit[P_GUIDED];
			}

			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				m_tFrame.iFrameStart = 0;
			}

			if (m_iPatternCnt >= CEarthLoad::P_END)
			{
				m_eCurState = CEarthLoad::DANCE;
				m_iPatternCnt = 0;
				for (int i = 0; i < 30; ++i)
				{
					int  _left = rand() % CEarthLoad::P_END;
					int  _right = rand() % CEarthLoad::P_END;
					Swap(&m_iPatternArr[_left], &m_iPatternArr[_right]);
				}
			}
		}
	}
}

void CEarthLoad::Attack_Pattern(float _fdTime, float _fDis)
{
	if (m_ePreState != DEAD && m_ePreState  != DANCE && m_ePreState != IDLE && m_ePreState != HIT)
	{
		if (P_BOX == m_ePattern)
		{
			// ATT1
			m_eCurState = ATT1;
			m_tFrame.iFrameEnd = 1;
			if (!m_bAttackCool && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			{
				++m_iAttackCnt;
				if (m_iAttackCnt <= m_iAttackCntLimit[m_ePattern]) {
					CObjMgr::Get_Instance()->Add_Object(OBJID::M_RECTBULLET, Create_Bullet<CEarthBossBox>("EarthBoss_Box", 1500.f));
					m_bAttackCool = true;
				}
				else {
					++m_iPatternCnt;
					m_iAttackCnt = 0;
					m_eCurState = IDLE;
					m_tFrame.iFrameStart = 0;
				}
			}
		}
		else if (P_DRILL == m_ePattern)
		{
			switch (m_eDir)
			{
			case CEarthLoad::LEFT:
				m_eCurState = ATT3_RIGHT;
				m_tFrame.iFrameEnd = 5;
				break;
			case CEarthLoad::TOP:
				m_eCurState = ATT3_UP;
				m_tFrame.iFrameEnd = 2;
				break;
			case CEarthLoad::RIGHT:
				m_eCurState = ATT3_RIGHT;
				m_tFrame.iFrameEnd = 5;
				break;
			case CEarthLoad::BOTTOM:
				m_eCurState = ATT3_DOWN;
				m_tFrame.iFrameEnd = 2;
				break;
			}

			if (!m_bAttackCool && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			{
				++m_iAttackCnt;
				if (m_iAttackCnt <= m_iAttackCntLimit[m_ePattern]) {
					CObjMgr::Get_Instance()->Add_Object(OBJID::M_RECTBULLET,
						Create_Bullet<CEarthDrill>((float)m_tPosin.x, (float)m_tPosin.y, "EarthDrill"));
					m_bAttackCool = true;
				}
				else {
					++m_iPatternCnt;
					m_iAttackCnt = 0;
					m_eCurState = IDLE;
					m_tFrame.iFrameStart = 0;
				}
			}
			if (m_eDir == CEarthLoad::LEFT || m_eDir == CEarthLoad::RIGHT) {
				if (floor(_fDis) > 100.f) MoveAngle(_fdTime);
			}
			else if (floor(_fDis) > 128.f) MoveAngle(_fdTime);
		}
		else if (P_PILLAR == m_ePattern)
		{
			m_eCurState = ATT1;
			m_tFrame.iFrameEnd = 1;
			if (!m_bAttackCool
				&& m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			{
				++m_iAttackCnt;
				if (m_iAttackCnt <= m_iAttackCntLimit[m_ePattern]) {
					CObjMgr::Get_Instance()->Add_Object(OBJID::M_RECTBULLET,
						Create_Bullet<CEarthPillarMaker>("", 1000.f,
							m_fAngle));
					m_bAttackCool = true;
				}
				else {
					++m_iPatternCnt;
					m_iAttackCnt = 0;
					m_eCurState = IDLE;
					m_tFrame.iFrameStart = 0;
				}
			}
		}
		else if (P_GUIDED == m_ePattern)
		{
			m_eCurState = ATT2;
			m_tFrame.iFrameEnd = 7;

			if (!m_bAttackCool
				&& m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			{
				++m_iAttackCnt;
				if (m_iAttackCnt <= m_iAttackCntLimit[m_ePattern]) {
					
					float _tempAngle = 36.f * (m_iAttackCnt - 1);
					float _tempX = m_tInfo.fX + cosf(Degree_To_Radian(_tempAngle)) * 120.f;
					float _tempY = m_tInfo.fY - sinf(Degree_To_Radian(_tempAngle)) * 120.f;
					CObjMgr::Get_Instance()->Add_Object(OBJID::M_RECTBULLET,
						Create_Bullet<CGuideLBullet>(_tempX, _tempY, "EarthBoss_Sword"));
					m_bAttackCool = true;
				}
				else {
					++m_iPatternCnt;
					m_iAttackCnt = 0;
					m_eCurState = IDLE;
					m_tFrame.iFrameStart = 0;
				}
			}
		}
	}
}

void CEarthLoad::Release()
{
	CBmpMgr::Get_Instance()->Delete_Bmp("EarthBoss");
	CBmpMgr::Get_Instance()->Delete_Bmp("EarthBoss_stretch");
	CBmpMgr::Get_Instance()->Delete_Bmp("EarthBoss_Box");
	CBmpMgr::Get_Instance()->Delete_Bmp("EarthBoss_Pillar");
	CBmpMgr::Get_Instance()->Delete_Bmp("EarthBoss_Sword");
}

void CEarthLoad::Change_HitRect()
{
	if (m_ePreState == DANCE)
	{
		switch (m_tFrame.iFrameStart)
		{
		case 2:
			m_tHitInfo.iCX = 122;
			m_tHitInfo.iCY = 154;
			break;
		case 3:
			m_tHitInfo.iCX = 119;
			m_tHitInfo.iCY = 154;
			break;
		case 4:
			m_tHitInfo.iCX = 187;
			m_tHitInfo.iCY = 164;
			break;
		case 5:
			m_tHitInfo.iCX = 119;
			m_tHitInfo.iCY = 164;
			break;
		case 6:
			m_tHitInfo.iCX = 103;
			m_tHitInfo.iCY = 164;
			break;
		case 7:
			m_tHitInfo.iCX = 119;
			m_tHitInfo.iCY = 164;
			break;
		case 8:
			m_tHitInfo.iCX = 103;
			m_tHitInfo.iCY = 164;
			break;
		}
	}
	else
	{
		m_tHitInfo.iCX = 106;
		m_tHitInfo.iCY = 180;
	}
}

CObj* CEarthLoad::Create_Bullet(const string & _frameKey)
{
	return nullptr;
}

int CEarthLoad::Get_Boss_State() const
{
	return (int)m_eCurState;
}

void CEarthLoad::Set_Boss_State(int _eNewState)
{
	if (m_eCurState != CBoss::DEAD)
		m_eCurState = (CBoss::EARTHB_STATE)_eNewState;
}
