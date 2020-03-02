#pragma once
#include "StopObj.h"

class CMyBitmap;

class COverDeco : public CStopObj
{
public:
	COverDeco();
	virtual ~COverDeco();

public:
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();

public:
	virtual int Get_Collision_Code() const { return CC_ONLYTILE_COLLISION; }

protected:
	TCHAR			szDialogue[128];
	CMyBitmap*		m_pMyBitmap;

	// Inherited via CStopObj
	virtual void Key_Check(float _fdTime) override;
};

