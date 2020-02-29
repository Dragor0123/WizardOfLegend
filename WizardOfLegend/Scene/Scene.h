#pragma once

#ifndef __SCENE_H__
#define __SCENE_H__

class CScene
{
public:
	CScene();
	virtual ~CScene() = 0;

	virtual bool Initialize() = 0;
	virtual int Update(float _fdTime) = 0;
	virtual void Late_Update(float _fdTime) = 0;
	virtual void Collision(float _fdTime) = 0;
	virtual void Render(HDC _DC, float _fdTime) = 0;
	virtual void Release() = 0;

	virtual bool Key_Check() = 0;
};

#endif