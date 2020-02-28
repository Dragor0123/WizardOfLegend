#include "../stdafx.h"
#include "PlazaTile.h"
#include "../MyBitmap/BmpMgr.h"

CPlazaTile::CPlazaTile()
{
}


CPlazaTile::~CPlazaTile()
{
	Release();
}


bool CPlazaTile::Initialize()
{
	CTile::Initialize();
	m_drawID_row = 3;
	m_drawID_col = 17;
	m_eOption = TILEENUM::OPT_STOP;
	return true;
}

void CPlazaTile::Key_Check(float _fdTime)
{
}

int CPlazaTile::Update(float _fdTime)
{
	return 0;
}

void CPlazaTile::Late_Update(float _fdTime)
{
	CStopObj::Late_Update(_fdTime);
}

void CPlazaTile::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	Update_Rect();
	Update_HitRect();
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image("PlazaTile");
	BitBlt(_DC, (int)(m_tRect.left + _fScrollX), (int)(m_tRect.top + _fScrollY)
		, TILECX, TILECY
		, hMemDC, m_drawID_col * TILECX, m_drawID_row * TILECY, SRCCOPY);
}

void CPlazaTile::Release()
{
}
