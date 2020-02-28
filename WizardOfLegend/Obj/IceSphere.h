#pragma once
#include "LineBullet.h"

class CIceSphere : public CLineBullet
{
public:
	CIceSphere();
	virtual ~CIceSphere();

public:
	// Inherited via CMoveObj
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();

	virtual void Move_Frame();
	virtual void Scene_Change();

public:
	virtual int	Get_Collision_Code() const;
	bool Get_bFireStart() const { return m_bFireStart; }
	bool Get_bMonsterWall() const { return  m_bMonsterAtWall; }

	void Set_bMonsterWall(bool _b) { m_bMonsterAtWall = _b; }
	bool Set_Offset_Arr(float *_arr, int _aSize);
	void Set_FireStartOn() { m_bFireStart = true; }
protected:
	virtual void  Change_HitRect();

private:
	bool		m_bFireStart;
	bool		m_bMonsterAtWall;
	float		m_fOffsetArr[8];
};

