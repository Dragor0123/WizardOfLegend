#pragma once

#ifndef __STAGE1_H__
#define __STAGE1_H__

#include "Scene.h"

// ¾Æ¸¶ Fire Stage
class CObj;
class CStage1 : public CScene
{
public:
	CStage1();
	virtual ~CStage1();

	// Inherited via CScene
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Collision(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime);
	virtual void Release();
	virtual bool Key_Check();

private:
	int				m_aRoomClear[6];
	stack<CObj*>	m_stackPrison;
	bool			m_bTeleCircleInserted;
};

#endif