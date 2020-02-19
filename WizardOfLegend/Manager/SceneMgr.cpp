#include "../stdafx.h"
#include "SceneMgr.h"
#include "../Scene/Scene.h"
#include "../Scene/Logo.h"
#include "../Scene/TitleMenu.h"
#include "../Scene/Stage1.h"
#include "../Scene/TileEdit.h"
#include "../Scene/Plaza.h"

CSceneMgr::CSceneMgr()
	: m_pScene(nullptr), m_eCurScene(SCENE_END), m_ePreScene(SCENE_END)
{
}


CSceneMgr::~CSceneMgr()
{
	Release();
}

bool CSceneMgr::Initialize()
{
	Scene_Change(CSceneMgr::SCENE_LOGO);
	return true;
}

void CSceneMgr::Update(float _fdTime)
{
	m_pScene->Update(_fdTime);
}

void CSceneMgr::Late_Update(float _fdTime)
{
	m_pScene->Late_Update(_fdTime);
}

void CSceneMgr::Collision(float _fdTime)
{
	m_pScene->Collision(_fdTime);
}

void CSceneMgr::Render(HDC _DC, float _fdTime)
{
	if (m_pScene)
		m_pScene->Render(_DC, _fdTime);
}

void CSceneMgr::Release()
{
	SAFE_DELETE(m_pScene);
}

// �ڵ� ���� �ʿ�: Scene ��ȯ�� ���� ���� �ǰ� �ؾ���.
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
		case SCENE_HOUSE:
			break;
		case SCENE_PLAZA:
			m_pScene = new CPlaza;
			break;
		case SCENE_STAGE1:
			m_pScene = new CStage1;
			break;
		case SCENE_EDIT:
			m_pScene = new CTileEdit;
			break;
		}

		// �������� ������ ��
		assert(m_pScene->Initialize());
		m_ePreScene = m_eCurScene;
	}
}
