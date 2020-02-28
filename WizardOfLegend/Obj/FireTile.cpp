#include "../stdafx.h"
#include "FireTile.h"
#include "../MyBitmap/BmpMgr.h"

CFireTile::CFireTile()
{
}

CFireTile::~CFireTile()
{
	Release();
}

bool CFireTile::Initialize()
{
	CTile::Initialize();
	m_drawID_row = 7;
	m_drawID_col = 2;
	m_eOption = TILEENUM::OPT_MOVE;
	return true;
}

void CFireTile::Key_Check(float _fdTime)
{
}

int CFireTile::Update(float _fdTime)
{
	return 0;
}

void CFireTile::Late_Update(float _fdTime)
{
	CStopObj::Late_Update(_fdTime);
}

void CFireTile::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	Update_Rect();
	Update_HitRect();
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image("FireTile");
	BitBlt(_DC, (int)(m_tRect.left + _fScrollX), (int)(m_tRect.top + _fScrollY)
		, TILECX, TILECY
		, hMemDC, m_drawID_col * TILECX, m_drawID_row * TILECY, SRCCOPY);

	// 	Draw_HitBox(_DC, _fScrollX, _fScrollY);
}

void CFireTile::Release()
{
}
