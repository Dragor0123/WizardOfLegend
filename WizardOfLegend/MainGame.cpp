#include "stdafx.h"
#include "MainGame.h"
#include "Manager\KeyMgr.h"
#include "Manager\Timer.h"
#include "Manager\PathMgr.h"

CMainGame::CMainGame()
	: m_hInst(NULL), m_hDC(NULL)
{
}


CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Logic()
{
	CTimer::Get_Instance()->Update();

	float fDeltaTime = CTimer::Get_Instance()->Get_DeltaTime();

	Input(fDeltaTime);
	Update(fDeltaTime);
	Late_Update(fDeltaTime);
	Collision(fDeltaTime);
	Render(fDeltaTime);
}

bool CMainGame::Initialize(HINSTANCE _hInst)
{
	m_hInst = _hInst;

	m_hDC = GetDC(g_hWnd);
	// 타이머 초기화
	if (!CTimer::Get_Instance()->Initialize())
		return false;

	// 경로 관리자 초기화
	if (!CPathMgr::Get_Instance()->Initialize())
		return false;

	//// 리소스 관리자 초기화
	//if (!CResourcesMgr::Get_Instance()->Initialize(hInst, m_hDC))
	//	return false;

	// 카메라 관리자 초기화

	// 장면 관리자 초기화

	return true;
}

void CMainGame::Input(float _fdTime)
{
}

void CMainGame::Update(float _fdTime)
{
}

void CMainGame::Late_Update(float _fdTime)
{
}

void CMainGame::Collision(float _fdTime)
{
}

void CMainGame::Render(float _fdTime)
{
}

void CMainGame::Release()
{
	ReleaseDC(g_hWnd, m_hDC);
	CKeyMgr::Destroy_Instance();
	// CObjMgr를 제일 나중에했었ㅇ므
}
