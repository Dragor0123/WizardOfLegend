#pragma once

#ifndef __TILEEDIT_H__
#define __TILEEDIT_H__

#include "Scene.h"

class CTileEdit : public CScene
{
public:
	CTileEdit();
	virtual ~CTileEdit();

	// Inherited via CScene
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Collision(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime);
	virtual void Release();

	virtual bool Key_Check();

private:
	bool Reload_TileMgr();

private:
	int							m_iRow;
	int							m_iCol;
	CObj*						m_pTileForPick;
	TILEENUM::ID				m_curTileID;
	TILEENUM::OPTION			m_eTileOpt;
	bool						m_bCursorEnable;
};

#endif