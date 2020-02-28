#pragma once

#ifndef __MEELEBULLET_H__
#define __MEELEBULLET_H__

#include "Bullet.h"

// SWORDMAN ATTACK ���� �׸� �� ��������Ʈ�� �����¿� 4���⸸ �����ϴ� ��
// �������� CMeeleBullet ��ӹ޾Ƽ� �������̵� �ؾ���... (Ư���� ���� ���ݵ�?)
class CMeeleBullet : public CBullet
{
public:
	enum ATTACKDIR { LEFT, TOP, RIGHT, BOTTOM, END};

public:
	CMeeleBullet();
	virtual ~CMeeleBullet();

	// Inherited via CMoveObj
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();

	// �ִϸ��̼� 
	virtual void Move_Frame();
	virtual void Scene_Change();
public:
	CMeeleBullet::ATTACKDIR Get_AttackDir() const { return m_eCurDir; }
	void			Set_AttackDir(CMeeleBullet::ATTACKDIR _eNewDir) { m_eCurDir = _eNewDir; Scene_Change(); }

protected:
	virtual void	Change_HitRect();
	virtual int		Get_Collision_Code() const;
protected:
	ATTACKDIR	m_ePreDir;
	ATTACKDIR	m_eCurDir;
};

#endif