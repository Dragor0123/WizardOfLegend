#pragma once

#ifndef __WARDINVEN_H__
#define __WARDINVEN_H__

#include "StopObj.h"
class CWardInven : public CStopObj
{
public:
	CWardInven();
	virtual ~CWardInven();

public:
	// Inherited via CStopObj
	virtual bool Initialize();
	virtual void Key_Check(float _fdTime);
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();

private:
	void Update_Robe_List(float _fdTime);
	void Late_Update_Robe_List(float _fdTime);
	void Insert_Robe_List();
	void Init_RobeColor();

private:
	static const float		m_fCircleY;
	static const float		m_fRADIUS;
	static const float		m_fOFFSET_X;
	static const float		m_fOFFSET_Y;

	list<CObj*>				m_listRobe;
	static COLORREF			tRobeColor[ROBE::END][21];
};

#endif