#pragma once

#ifndef __FIREBOSSSTAGE_H__
#define __FIREBOSSSTAGE_H__

#include "Scene.h"
class CFireBossStage : public CScene
{
public:
	CFireBossStage();
	virtual ~CFireBossStage();

	// Inherited via CScene
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Collision(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime);
	virtual void Release();
	virtual bool Key_Check();

private:
	bool			m_bTeleCircleInserted;
};

#endif