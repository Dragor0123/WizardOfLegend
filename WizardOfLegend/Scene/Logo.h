#pragma once

#ifndef __LOGO_H__
#define __LOGO_H__

#include "Scene.h"

class CLogo : public CScene
{
public:
	CLogo();
	virtual ~CLogo();

	// Inherited via CScene
	virtual bool Key_Check();
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Collision(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime);
	virtual void Release();
private:
	int		m_iKeyCnt;
	float	m_fTime;
	float	m_fTime2;
	BLENDFUNCTION	m_tBF;

};

#endif