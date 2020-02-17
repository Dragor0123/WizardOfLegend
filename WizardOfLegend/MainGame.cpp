#include "stdafx.h"
#include "MainGame.h"
#include "Manager\KeyMgr.h"
#include "Manager\Timer.h"
#include "Manager\ScrollMgr.h"
#include "Line\LineMgr.h"
#include "Obj\ObjMgr.h"
#include "MyBitmap\BmpMgr.h"
#include "MyBitmap\MyBitmap.h"
#include "Manager\SceneMgr.h"

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

	// 오브젝트 매니저 초기화
	if (!CObjMgr::Get_Instance()->Initialize())
		return false;

	// 스크롤 매니저 초기화
	if (!CScrollMgr::Get_Instance()->Initialize())
		return false;

	//// 비트맵 매니저 초기화
	if (!CBmpMgr::Get_Instance()->Initialize())
		return false;

	// Scene 매니저 초기화
	if (!CSceneMgr::Get_Instance()->Initialize())
		return false;

	return true;
}

void CMainGame::Update(float _fdTime)
{
	CSceneMgr::Get_Instance()->Update(_fdTime);
}

void CMainGame::Late_Update(float _fdTime)
{
	CSceneMgr::Get_Instance()->Late_Update(_fdTime);
	CKeyMgr::Get_Instance()->Key_Update();
}

void CMainGame::Collision(float _fdTime)
{
	//CSceneMgr는 Collision 안하나?
	//CSceneMgr::Get_Instance()->Collision(_fdTime)
}

void CMainGame::Render(float _fdTime)
{
	HDC hBackBuffer = CBmpMgr::Get_Instance()->Find_Image("BackBuffer");
	Rectangle(hBackBuffer, 0, 0, WINCX, WINCY);

	CSceneMgr::Get_Instance()->Render(hBackBuffer, _fdTime);

	BitBlt(m_hDC, 0, 0, WINCX, WINCY, hBackBuffer, 0, 0, SRCCOPY);
}

void CMainGame::Release()
{
	ReleaseDC(g_hWnd, m_hDC);
	CKeyMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();
	CBmpMgr::Destroy_Instance();
	CSceneMgr::Destroy_Instance();
	CObjMgr::Destroy_Instance();
	CTimer::Destroy_Instance();
}
