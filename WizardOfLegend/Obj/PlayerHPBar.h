#pragma once
#include "StopObj.h"

class CPlayerHPBar : public CStopObj
{
public:
	CPlayerHPBar();
	virtual ~CPlayerHPBar();

	// Inherited via CStopObj
	virtual bool Initialize() override;
	virtual void Key_Check(float _fdTime) override;
	virtual int Update(float _fdTime) override;
	virtual void Late_Update(float _fdTime) override;
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f) override;
	virtual void Release() override;

private:
	RECT			m_tUserHPRC;
	RECT			m_tUserMPRC;
	const int		m_iManaBarCX;
	const int		m_iManaBarCY;
};

