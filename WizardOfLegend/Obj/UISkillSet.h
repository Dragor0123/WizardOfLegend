#pragma once
#include "StopObj.h"

class CUISkillSet : public CStopObj
{
public:
	CUISkillSet();
	virtual ~CUISkillSet();

public:
	// Inherited via CStopObj
	virtual bool Initialize() override;
	virtual void Key_Check(float _fdTime) override;
	virtual int Update(float _fdTime) override;
	virtual void Late_Update(float _fdTime) override;
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f) override;
	virtual void Release() override;

private:
	void Render_Skill_Sets(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY);

private:
	array<int, 6>* m_pInvenOuter;
};

