#include "../stdafx.h"
#include "ScrewBullet.h"
#include "../MyBitmap/BmpMgr.h"

CScrewBullet::CScrewBullet()
	: m_fRotAngle(0.f), m_fRotDis(0.f), m_fRotSpeed(0.f)
{
	ZeroMemory(&m_tCenter, sizeof(m_tCenter));
}


CScrewBullet::~CScrewBullet()
{
	Release();
}

bool CScrewBullet::Initialize()
{
	// 이 Insert_Bmp부분을 ArcanaMgr에서 다 한꺼번에 삽입 시킨다.
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Skill/dragon_arc116.bmp", "DragonArc"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Skill/rot116.bmp", "DragonArcRot"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Skill/stretch116.bmp", "DArc_Stretch"))
		return false;

	CBullet::Initialize();

	m_tInfo.iCX = 116;
	m_tInfo.iCY = 116;
	Equalize_HitPosInfoPos();
	m_tHitInfo.iCX = 104;
	m_tHitInfo.iCY = 68;
	m_tStartPt.fX = m_tInfo.fX;
	m_tStartPt.fY = m_tInfo.fY;

	m_fSpeed = 900.f;	 //직선으로 날아갈 점의 스피드
	if (!m_bMonsters)
		m_fShotRange = 850.f;
	else
		m_fShotRange = 1600.f;

	m_fRotAngle = 0.f;
	if (!m_bMonsters)
	{
		m_fRotSpeed = 4.f;
		m_fRotDis = 40.f;
	}
	else
	{
		m_fRotSpeed = 12.f;
		m_fRotDis = 80.f;
	}
	m_bStart = true;
	m_bClockWise = false;

	if (m_strFrameKey == "")
		m_strFrameKey = string("DragonArc");
	
	m_ePreState = END;
	m_eCurState = FIRE;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 2;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();

	if (!m_bMonsters)
		m_iAtt = 10;
	else
		m_iAtt = 20;

	return true;
}

int CScrewBullet::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	float fStartAngle = 0.f;
	if (m_bStart)
	{
		// 발사시점에 딱 한 번,
		// m_tCenter.x, m_tCenter.y 를 tPosin.x, tPosin.y 값으로 넣어준다.
		m_tCenter.x = (LONG)m_tHitInfo.fX;
		m_tCenter.y = (LONG)m_tHitInfo.fY;
		if (!m_bClockWise)
			m_fRotAngle = -90.f + m_fAngle;
		else
			m_fRotAngle = 90.f + m_fAngle;
		fStartAngle = m_fRotAngle;
		m_bStart = false;
	}

	m_tCenter.x += (LONG)(cosf(Degree_To_Radian(m_fAngle)) * m_fSpeed * _fdTime);
	m_tCenter.y -= (LONG)(sinf(Degree_To_Radian(m_fAngle)) * m_fSpeed * _fdTime);

	if (!m_bClockWise)
		m_fRotAngle += m_fRotSpeed;
	else
		m_fRotAngle -= m_fRotSpeed;

	m_tInfo.fX = m_tCenter.x + cosf(Degree_To_Radian(m_fRotAngle)) * m_fRotDis;
	m_tInfo.fY = m_tCenter.y - sinf(Degree_To_Radian(m_fRotAngle)) * m_fRotDis;
	Equalize_HitPosInfoPos();
	// 여기까지 움직임

	Move_Frame();
	Scene_Change();
	Change_HitRect();
	Update_Rect();
	Update_HitRect();

	return OBJ_NOEVENT;
}

void CScrewBullet::Late_Update(float _fdTime)
{
	CBullet::Late_Update(_fdTime);
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

void CScrewBullet::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);
	HDC hRotDC = CBmpMgr::Get_Instance()->Find_Image("DragonArcRot");
	HDC hStrDC = CBmpMgr::Get_Instance()->Find_Image("DArc_Stretch");

	if (m_ePreState == CBullet::COLLISION)
	{
		GdiTransparentBlt(_DC
			, (int)(m_tRect.left + _fScrollX)
			, (int)(m_tRect.top + _fScrollY)
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * m_tInfo.iCX
			, m_tFrame.iFrameScene * m_tInfo.iCY
			, m_tInfo.iCX, m_tInfo.iCY
			, MAGENTA_COLOR);
	}
	else
	{
		PlgBlt(hRotDC, m_tRotPoint,
			hMemDC,
			m_tFrame.iFrameStart * m_tInfo.iCX,
			m_tFrame.iFrameScene * m_tInfo.iCY,
			m_tInfo.iCX, m_tInfo.iCY,
			NULL, NULL, NULL);

		if (m_fAngle >= 150.f && m_fAngle < 180.f)
		{
			StretchBlt(hStrDC,
				0, 0, m_tInfo.iCX, m_tInfo.iCY, hRotDC,
				0, m_tInfo.iCY, m_tInfo.iCX, -m_tInfo.iCY, SRCCOPY);
		}
		if (m_fAngle >= 180.f && m_fAngle < 220.f) {
			StretchBlt(hStrDC,
				0, 0, m_tInfo.iCX, m_tInfo.iCY, hRotDC,
				0, m_tInfo.iCY, m_tInfo.iCX, -m_tInfo.iCY, SRCCOPY);
		}
		else if (m_fAngle >= 220.f && m_fAngle < 270.f)
		{
			StretchBlt(hStrDC,
				0, 0, m_tInfo.iCX, m_tInfo.iCY, hRotDC,
				m_tInfo.iCX, 0, -m_tInfo.iCX, m_tInfo.iCY, SRCCOPY);
		}

		GdiTransparentBlt(_DC,
			(int)(m_tRect.left + _fScrollX),
			(int)(m_tRect.top + _fScrollY),
			m_tInfo.iCX, m_tInfo.iCY,
			//hRotDC,
			(m_fAngle >= 150.f && m_fAngle < 270.f) ? hStrDC : hRotDC,
			0, 0,
			m_tInfo.iCX, m_tInfo.iCY,
			MAGENTA_COLOR);
		////
	}

	Draw_HitBox(_DC, _fScrollX, _fScrollY);
}

void CScrewBullet::Release()
{
}

void CScrewBullet::Move_Frame()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();

		if (m_ePreState == CBullet::FIRE) {
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_tFrame.iFrameStart = 0;
		}
		else if (m_ePreState == CBullet::COLLISION) {
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_bDead = true;
		}
	}
}

void CScrewBullet::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CBullet::FIRE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CBullet::COLLISION:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CScrewBullet::Change_HitRect()
{
	m_tHitInfo.iCX = (int)(abs(104 * cosf(Degree_To_Radian(m_fAngle))) + abs(68 * cosf(Degree_To_Radian(90.f + m_fAngle))));
	m_tHitInfo.iCY = (int)(abs(104 * sinf(Degree_To_Radian(m_fAngle))) + abs(68 * sinf(Degree_To_Radian(90.f + m_fAngle))));
}

int CScrewBullet::Get_Collision_Code() const
{
	if (!m_bMonsters)
		return CC_PBULLET_WALL_PUSH_NDRAG;
	else
		return CC_MBULLET_WALL_PUSH;
}
