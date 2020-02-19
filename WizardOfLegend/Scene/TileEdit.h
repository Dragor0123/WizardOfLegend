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

	virtual void Key_Check();

private:
	int							m_iRow;
	int							m_iCol;
	TILEENUM::OPTION			m_eTileOpt;
	CObj*						m_pTileForPick;
};

#endif