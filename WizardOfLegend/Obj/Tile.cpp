#include "../stdafx.h"
#include "Tile.h"


CTile::CTile()
	: m_drawID_row(0), m_drawID_col(0), m_eOption(TILEENUM::OPT_MOVE)
{
}


CTile::~CTile()
{
}

bool CTile::Initialize()
{
	m_tInfo.iCX = TILECX;
	m_tInfo.iCY = TILECY;

	Equalize_HitSzInfoSz();
	return true;
}
