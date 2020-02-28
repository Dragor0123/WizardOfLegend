#pragma once
#include "Monster.h"

class CSummonerBall : public CMonster
{
public:
	CSummonerBall();
	virtual ~CSummonerBall();

public:
	virtual int	Get_Collision_Code() const { return CC_MONSTER_COLLISION; }

public:
	// Inherited via CMonster
	virtual bool Initialize() ;
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	
	virtual void Move_Frame();
	virtual void Scene_Change();
	virtual void Change_HitRect();

	virtual CObj * Create_Bullet(const string & _frameKey = "");

public:
	int		Get_Att() const { return m_iAtt; }
private:
	POINT					m_tPosin;			// 포신과 같다.
	float					m_fRange;
	POINT					m_tRotPoint[3];
	static const DWORD		ATT_FRAME_SPEED;
	int						m_iAtt;

	float					m_fMoveTime;
	float					m_fEndX;
	float					m_fEndY;
	float					m_fFireDis;
	float					m_fPosinRad;
};

