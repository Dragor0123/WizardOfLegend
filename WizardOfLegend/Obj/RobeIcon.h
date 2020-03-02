#pragma once

#ifndef __ROBEICON_H__
#define __ROBEICON_H__

#include "StopObj.h"
class CRobeIcon : public CStopObj
{
public:
	CRobeIcon();
	virtual ~CRobeIcon();

public:
	virtual bool Initialize();
	virtual void Key_Check(float _fdTime);
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();

public:
	void Set_DrawID(int i) { m_iDrawID = i; }
	
	int Get_DrawID() const { return m_iDrawID; }
private:
	int			m_iDrawID;
};

#endif