#pragma once

#include "MoveObj.h"
class CShield : public CMoveObj
{
	// 필요하다면 STATE 넣어서 state를 달리주는 것도 방법
public:
	CShield();
	virtual ~CShield();

	// Inherited via CMoveObj
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f)  = 0;
	virtual void Release();

public:
	virtual void Move_Frame();
	virtual void Scene_Change();
	
public:
	int		Get_Att() const		{ return m_iAtt; }
	int     Get_Collision_Code() const { return  CC_PSHIELD_NOREFLECT; }
	void	Set_Distance(float _fDis) { m_fDis = _fDis; }

protected:
	float			m_fLifeTime;
	float			m_fLifeTLimit;
	float			m_fDis;
	int				m_iAtt;
};

