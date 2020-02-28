#pragma once

#ifndef __UNDERDECO_H__
#define __UNDERDECO_H__

#include "StopObj.h"

class CUnderDeco : public CStopObj
{
public:
	CUnderDeco();
	virtual ~CUnderDeco();

	// Inherited via CStopObj
	virtual bool Initialize();
	virtual void Key_Check(float _fdTime);
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();

	// 애니메이션 
	void Move_Frame();
	void Scene_Change();
	
protected:
	POINT		m_tRotPoint[3];

	// state가 필요하면 필요한대로....
};

#endif
