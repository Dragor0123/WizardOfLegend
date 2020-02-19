#include "../stdafx.h"
#include "ScrollMgr.h"

CScrollMgr::CScrollMgr()
	: m_fScrollX(0.f), m_fScrollY(0.f)
{
}


CScrollMgr::~CScrollMgr()
{
}

void CScrollMgr::Scroll_Lock()
{
	if (0 < m_fScrollX)
		m_fScrollX = 0.f;
	if (0 < m_fScrollY)
		m_fScrollY = 0.f;

	if (WINCX - TILECX * TILE_NUMX > m_fScrollX)
		m_fScrollX = WINCX - TILECX * TILE_NUMX;
	if (WINCY - TILECY * TILE_NUMY > m_fScrollY)
		m_fScrollY = WINCY - TILECY * TILE_NUMY;
}