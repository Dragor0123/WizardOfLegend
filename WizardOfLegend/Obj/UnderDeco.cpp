#include "../stdafx.h"
#include "UnderDeco.h"
#include "../MyBitmap/BmpMgr.h"


namespace UnderDeco_Space
{
	const DWORD CREVIS_FRAMESPEED = 120;
}

using UnderDeco_Space::CREVIS_FRAMESPEED;

CUnderDeco::CUnderDeco()
{
	ZeroMemory(&m_tRotPoint, sizeof(m_tRotPoint));
}


CUnderDeco::~CUnderDeco()
{
	Release();
}

bool CUnderDeco::Initialize()
{
	if (m_strFrameKey == "")
		return false;

	if (m_strFrameKey == "Crevis")
	{
		m_tInfo.iCX = 100;
		m_tInfo.iCY = 80;
		Equalize_HitPosInfoPos();
		m_tHitInfo.iCX = 0;
		m_tHitInfo.iCY = 0;

		//
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 4;
		m_tFrame.iFrameScene = 0;
		m_tFrame.dwFrameSpeed = CREVIS_FRAMESPEED;
	}
	else if (m_strFrameKey == "otherDeco")
	{
		//...
	}

	m_tFrame.dwFrameTime = GetTickCount();

	return true;
}

void CUnderDeco::Key_Check(float _fdTime)
{
}

int CUnderDeco::Update(float _fdTime)
{
	// crevis ������� alpha blending �Ἥ ���̵�ƿ�??
	if (m_bDead)
		return OBJ_DEAD;

	Move_Frame();
	Scene_Change();
	Update_Rect();
	return OBJ_NOEVENT;
}

void CUnderDeco::Late_Update(float _fdTime)
{
	Update_Rect();

	// ���� ȸ�����Ѿ��ϸ� m_tRotPoint �����ϰ�...
}

void CUnderDeco::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);

	if (m_strFrameKey == "Crevis")
	{
		GdiTransparentBlt(_DC,
			(int)(m_tRect.left + _fScrollX),
			(int)(m_tRect.top + _fScrollY),
			m_tInfo.iCX, m_tInfo.iCY,
			hMemDC,
			m_tFrame.iFrameStart * m_tInfo.iCX,
			0,
			m_tInfo.iCX, m_tInfo.iCY,
			MAGENTA_COLOR);
	}
	//else
	{

	}
}

void CUnderDeco::Release()
{
}

void CUnderDeco::Move_Frame()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();

		if (m_strFrameKey == "Crevis")
		{
			m_tFrame.dwFrameSpeed = CREVIS_FRAMESPEED;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_bDead = true;
		}
	}
}

void CUnderDeco::Scene_Change()
{
	if (m_strFrameKey == "Crevis")
		return;
	//else
}
