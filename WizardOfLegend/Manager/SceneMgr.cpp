#include "../stdafx.h"
#include "SceneMgr.h"

CSceneMgr::CSceneMgr()
	: m_pScene(nullptr), m_eCurScene(SCENE_END), m_ePreScene(SCENE_END)
{
}


CSceneMgr::~CSceneMgr()
{
}

bool CSceneMgr::Initialize()
{
	return false;
}

void CSceneMgr::Input(float _fdTime)
{
}

void CSceneMgr::Update(float _fdTime)
{
}

void CSceneMgr::Late_Update(float _fdTime)
{
}

void CSceneMgr::Collision(float _fdTime)
{
}

void CSceneMgr::Render(HDC _DC, float _fdTime)
{
}

void CSceneMgr::Release()
{
}

/*
void CSceneMgr::Scene_Change(SCENEID _eScene)
{
	m_eCurScene = _eScene;

	if (m_ePreScene != m_eCurScene)
	{
		SAFE_DELETE(m_pScene);

		switch (m_eCurScene)
		{
		case SCENE_LOGO:
			m_pScene = new CLogo;
			break;
		case SCENE_MENU:
			m_pScene = new CTitleMenu;
			break;
		case SCENE_STAGE:
			break;
		case SCENE_EDIT:
			break;
		}
		m_pScene->Initialize();
		m_ePreScene = m_eCurScene;
	}
}
*/