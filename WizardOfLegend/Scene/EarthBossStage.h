#pragma once

#ifndef __EARTHBOSSSTAGE_H__
#define __EARTHBOSSSTAGE_H__

#include "Scene.h"

class CEarthBossStage : public CScene
{
public:
	CEarthBossStage();
	virtual ~CEarthBossStage();

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
	bool			m_bBossHPBarInserted;
};

#endif