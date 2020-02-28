#pragma once
#include "Obstacle.h"

class CPrison : public CObstacle
{
public:
	CPrison();
	virtual ~CPrison();

public:
	// Inherited via CObstacle
	virtual bool Initialize() override;
	virtual int Update(float _fdTime) override;
	virtual void Late_Update(float _fdTime) override;
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f) override;
	virtual void Release() override;

public:
	virtual int	Get_Collision_Code() const { return CC_NOHIT_BLOCK_OBS; }

private:
	virtual void Key_Check(float _fdTime) override;

private:

};

