#pragma once

#ifndef __MOVEOBJ_H__
#define __MOVEOBJ_H__

#include "Obj.h"
class CMoveObj : public CObj
{
public:
	CMoveObj();
	//CMoveObj(const CMoveObj& _other);
	virtual ~CMoveObj();
public:
	void Move(float x, float y);
	void Move(float x, float y, float fDeltaTime);
	void MoveXFromSpeed(float fDeltaTime, MOVEDIR::FB eDir);
	void MoveYFromSpeed(float fDeltaTime, MOVEDIR::FB eDir);
	void MoveX(float x);
	void MoveX(float yourSpeed, float fDeltaTime, MOVEDIR::FB eDir = MOVEDIR::MD_FRONT);
	void MoveY(float y);
	void MoveY(float yourSpeed, float fDeltaTime, MOVEDIR::FB eDir = MOVEDIR::MD_FRONT);
	void MoveLeftTop(float fDeltaTime);
	void MoveLeftBottom(float fDeltaTime);
	void MoveRightTop(float fDeltaTime);
	void MoveRightBottom(float fDeltaTime);
	void MoveLeftTop(float yourspeed, float fDeltaTime);
	void MoveLeftBottom(float yourspeed, float fDeltaTime);
	void MoveRightTop(float yourspeed, float fDeltaTime);
	void MoveRightBottom(float yourspeed, float fDeltaTime);


	// 무브앵글 수정 필요
	void MoveAngle();
	void MoveAngle(float fTime);

public:
	void  Set_Speed(float _fSpeed) { m_fSpeed = _fSpeed; }
	float Get_Speed() const { return m_fSpeed; }
	const MOVEDIR::DIR& Get_Move_Dir() const{ return m_eMoveDir; }
public:
	// Inherited via CObj
	virtual bool Initialize() = 0;
	virtual void Key_Check(float _fdTime);
	virtual int Update(float _fdTime) = 0;
	virtual void Late_Update(float _fdTime) = 0;
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f) = 0;
	virtual void Release() = 0;
	virtual void Move_Frame();

protected:
	float			m_fSpeed;
	MOVEDIR::DIR	m_eMoveDir;
};

#endif