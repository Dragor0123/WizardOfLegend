#pragma once

#ifndef __SCENEMGR_H__
#define __SCENEMGR_H__

#include "Singleton.h"

class CScene;

class CSceneMgr : public Singleton<CSceneMgr>
{
	friend class Singleton<CSceneMgr>;

public:
	enum SCENEID { SCENE_LOGO, SCENE_MENU, SCENE_PLAZA, SCENE_STAGE1, SCENE_EDIT, SCENE_END };

public:
	bool Initialize();
	void Update(float _fdTime);
	void Late_Update(float _fdTime);
	void Collision(float _fdTime);
	void Render(HDC _DC, float _fdTime);
	void Release();

	int Get_Current_SceneID();

public:
	void Scene_Change(SCENEID _eScene);

private:
	CSceneMgr();
	~CSceneMgr();
	CSceneMgr(const CSceneMgr&) = delete;
	CSceneMgr& operator=(const CSceneMgr&) = delete;

private:
	CScene*				m_pScene;	// 현재 Scene의 주소를 가지고 있는 포인터
	SCENEID				m_ePreScene;
	SCENEID				m_eCurScene;
};

#endif
