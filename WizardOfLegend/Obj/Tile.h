#pragma once

#ifndef __TILE_H__
#define __TILE_H__

#include "StopObj.h"

class CTile : public CStopObj
{
public:
	CTile();
	virtual ~CTile();

public:
	// Inherited via CObj
	virtual bool Initialize();
	virtual void Key_Check(float _fdTime) = 0;
	virtual int Update(float _fdTime) = 0;
	virtual void Late_Update(float _fdTime) = 0;
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f) = 0;
	virtual void Release() = 0;

	void Set_DrawID_Row(int _iDrawRow) { m_drawID_row = _iDrawRow; }
	void Set_DrawID_Col(int _iDrawCol) { m_drawID_col = _iDrawCol; }
	void Set_Option(const TILEENUM::OPTION& _eTileOpt) { m_eOption = _eTileOpt; }

	const int& Get_DrawID_Row() const { return m_drawID_row; }
	const int& Get_DrawID_Col() const { return m_drawID_col; }
	const TILEENUM::OPTION& Get_Option() const { return m_eOption; }
protected:
	int						m_drawID_row;
	int						m_drawID_col;
	TILEENUM::OPTION		m_eOption;
};

#endif