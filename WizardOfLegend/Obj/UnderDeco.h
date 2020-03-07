#pragma once

#ifndef __UNDERDECO_H__
#define __UNDERDECO_H__

#include "StopObj.h"

// 잘못 설계한 클래스
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

public:
	virtual int	Get_Collision_Code() const { return CC_ONLYTILE_COLLISION; }

public:
	int		Get_DrawID() const { return m_iDrawID; }
	void	Set_DrawID(int i) { m_iDrawID = i; }

protected:
	POINT		m_tRotPoint[3];
	int			m_iDrawID;
	// state가 필요하면 필요한대로....
};

#endif
