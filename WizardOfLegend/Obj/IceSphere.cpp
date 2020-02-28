#include "../stdafx.h"
#include "IceSphere.h"
#include "../MyBitmap/BmpMgr.h"

CIceSphere::CIceSphere()
	:m_bFireStart(false), m_bMonsterAtWall(false)
{
	ZeroMemory(m_fOffsetArr, sizeof(m_fOffsetArr));
}


CIceSphere::~CIceSphere()
{
	Release();
}

bool CIceSphere::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Skill/IceSphere.bmp", "IceSphere"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Skill/rot160.bmp", "Sphere_rot"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Skill/empty160.bmp", "Sphere_empty"))
		return false;

	CBullet::Initialize();
	m_tInfo.iCX = 160;
	m_tInfo.iCY = 160;
	Equalize_HitPosInfoPos();

	m_tHitInfo.iCX = 96;
	m_tHitInfo.iCY = 16;
	m_fShotRange = 5000.f;
	m_fSpeed = 1400.f;
	m_iAtt = 100;

	if (m_strFrameKey == "")
		m_strFrameKey = string("IceSphere");

	m_ePreState = END;
	m_eCurState = FIRE;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();
	return true;
}

int CIceSphere::Update(float _fdTime)
{
	// 수정 요함.
	// 1.마우스 떼면 날아가야되고. 2. 마우스 떼기전까지는 계속 각도 입력받아야함.

	if (m_bDead)
		return OBJ_DEAD;

	if (m_pTarget && !m_bFireStart)
	{
		float fOffSetX = 0.f, fOffsetY = 0.f;
		m_fAngle = m_pTarget->Get_Angle();
		if ((0.f <= m_fAngle && m_fAngle < 45.f) || (315.f <= m_fAngle && m_fAngle < 360.f)) {
			fOffSetX = -36.f;
			fOffsetY = -18.f;
		}
		else if (45.f <= m_fAngle && m_fAngle < 135.f) {
			fOffSetX = 8.f;
			fOffsetY = 20.f;
		}
		else if (135.f <= m_fAngle && m_fAngle < 225.f) {
			fOffSetX = 48.f;
			fOffsetY = -16.f;
		}
		else {
			fOffSetX = -22.f;
			fOffsetY = -52.f;
		}
		m_tInfo.fX = m_tStartPt.fX + fOffSetX;
		m_tInfo.fY = m_tStartPt.fY + fOffsetY;
		Equalize_HitPosInfoPos();
	}
	
	if (m_bFireStart)
	{
		m_tInfo.fX += cosf(Degree_To_Radian(m_fAngle)) * m_fSpeed * _fdTime;
		m_tInfo.fY -= sinf(Degree_To_Radian(m_fAngle)) * m_fSpeed * _fdTime;
	}

	Equalize_HitPosInfoPos();

	Move_Frame();
	Scene_Change();
	Change_HitRect();
	Update_Rect();
	Update_HitRect();
	return OBJ_NOEVENT;
}

void CIceSphere::Late_Update(float _fdTime)
{
	if (m_bCollision && m_bFireStart) {
		m_eCurState = CBullet::COLLISION;
		m_tHitInfo.iCX = 0;
		m_tHitInfo.iCY = 0;
	}

	float fdX = 0.f, fdY = 0.f;
	fdX = m_tInfo.fX - m_tStartPt.fX;
	fdY = m_tInfo.fY - m_tStartPt.fY;

	float fdist = sqrtf(fdX * fdX + fdY * fdY);

	if (fdist > m_fShotRange) {
		m_eCurState = CBullet::COLLISION;
		m_tHitInfo.iCX = 0;
		m_tHitInfo.iCY = 0;
	}

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

void CIceSphere::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	if (m_ePreState != CBullet::COLLISION)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);
		HDC hRotDC = CBmpMgr::Get_Instance()->Find_Image("Sphere_rot");
		HDC hEmpDC = CBmpMgr::Get_Instance()->Find_Image("Sphere_Empty");

		PlgBlt(hRotDC, m_tRotPoint,
			hMemDC, 0, 0, m_tInfo.iCX, m_tInfo.iCY,
			NULL, NULL, NULL);

		GdiTransparentBlt(_DC
			, (int)(m_tRect.left + _fScrollX)
			, (int)(m_tRect.top + _fScrollY)
			, m_tInfo.iCX, m_tInfo.iCY
			, hRotDC
			, 0
			, 0
			, m_tInfo.iCX, m_tInfo.iCY
			, MAGENTA_COLOR);

		BitBlt(hRotDC, 0, 0, m_tInfo.iCX, m_tInfo.iCY, hEmpDC, 0, 0, SRCCOPY);

		Draw_HitBox(_DC, _fScrollX, _fScrollY);
	}
}

void CIceSphere::Release()
{
}

void CIceSphere::Move_Frame()
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
				m_bDead = true;
		}
	}
}

void CIceSphere::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CBullet::FIRE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CBullet::COLLISION:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

int CIceSphere::Get_Collision_Code() const
{
	if (!m_bMonsterAtWall)
		return CC_PBULLET_WALL_PUSH_NDRAG;
	else
		return CC_PBULLET_WALL_NPUSH_NDRAG;
}

bool CIceSphere::Set_Offset_Arr(float * _arr, int _aSize)
{
	if (_aSize != 8)
		return false;

	for (int i = 0; i < 8; ++i)
	{
		m_fOffsetArr[i] = _arr[i];
	}
	return true;
}

void CIceSphere::Change_HitRect()
{
	m_tHitInfo.iCX = (int)(abs(96 * cosf(Degree_To_Radian(m_fAngle))) + abs(16 * cosf(Degree_To_Radian(90.f + m_fAngle))));
	m_tHitInfo.iCY = (int)(abs(96 * sinf(Degree_To_Radian(m_fAngle))) + abs(16 * sinf(Degree_To_Radian(90.f + m_fAngle))));
}
