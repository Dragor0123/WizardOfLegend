#pragma once

#include "Card.h"

#define GAIA_SHIELD			00
#define DRAGON_ARC			01

class CArcRel;

class CArcanaCard : public CCard
{
public:
	CArcanaCard();
	virtual ~CArcanaCard();

public:
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();

	virtual CArcRel* Create_ArcanaRelic(int _bCondition);

public:
	// Inherited via CFAble
	virtual void Do_FButton_Action(float _fdTime);
	virtual int Get_Collision_Code() const { return CC_FABLE_ARCANA; }

private:
	void MoveY(float yourSpeed, float fDeltaTime, MOVEDIR::FB eDir);

private:
	int		m_iDir;
	float	m_fmoveTime;
	float   m_fSpeed;
};

