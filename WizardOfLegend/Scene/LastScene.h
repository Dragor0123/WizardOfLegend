#pragma once

#ifndef __LASTSCENE_H__
#define __LASTSCENE_H__

#include "Scene.h"

class CLastScene : public CScene
{
public:
	CLastScene();
	virtual ~CLastScene();

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
	BLENDFUNCTION	m_tBF;
};

#endif