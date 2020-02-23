#include "stdafx.h"
#include "MainGame.h"
#include "Manager\KeyMgr.h"
#include "Manager\Timer.h"
#include "Manager\ScrollMgr.h"
#include "Obj\ObjMgr.h"
#include "MyBitmap\BmpMgr.h"
#include "MyBitmap\MyBitmap.h"
#include "Manager\SceneMgr.h"
#include "Manager\TileMgr.h"

CMainGame::CMainGame()
	: m_hInst(NULL), m_hDC(NULL)
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//#ifdef _DEBUG
//	// 콘솔창을 생성시켜주는 함수이다.
//	AllocConsole();
//#endif //DEBUG
}


CMainGame::~CMainGame()
{
	Release();
//#ifdef _DEBUG
//	FreeConsole();
//#endif //DEBUG
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
	srand(unsigned(time(NULL)));

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

	if (!CTileMgr::Get_Instance()->Initialize("PlazaTile"))
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
	CScrollMgr::Get_Instance()->Scroll_Lock();
}

void CMainGame::Collision(float _fdTime)
{
	CSceneMgr::Get_Instance()->Collision(_fdTime);
}

void CMainGame::Render(float _fdTime)
{
	HDC hBackBuffer = CBmpMgr::Get_Instance()->Find_Image("BackBuffer");
	Rectangle(hBackBuffer, 0, 0, WINCX, WINCY);

	CSceneMgr::Get_Instance()->Render(hBackBuffer, _fdTime);

	BitBlt(m_hDC, 0, 0, WINCX, WINCY, hBackBuffer, 0, 0, SRCCOPY);

	if (!g_FPS_ON)
		Render_Mouse_Pt();
	else
		Render_FPS();
}

void CMainGame::Release()
{
	CKeyMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();
	CBmpMgr::Destroy_Instance();
	CSceneMgr::Destroy_Instance();
	CTileMgr::Destroy_Instance();
	CObjMgr::Destroy_Instance();
	CTimer::Destroy_Instance();
	ReleaseDC(g_hWnd, m_hDC);
}

void CMainGame::Render_Mouse_Pt()
{
	POINT	pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	pt.x -= (LONG)CScrollMgr::Get_Instance()->Get_ScrollX();
	pt.y -= (LONG)CScrollMgr::Get_Instance()->Get_ScrollY();
	// 마우스를 활성화 비활성화 시켜서 Render를 안할수 있겠지...
	TCHAR szBuff[32] = L"";
	swprintf_s(szBuff, L"마우스 좌표(%d, %d)", (int)(pt.x), (int)(pt.y));
	SetWindowText(g_hWnd, szBuff);
}

void CMainGame::Render_FPS()
{
	TCHAR szFPS[32] = L"";
	swprintf_s(szFPS, 32, L"FPS : %.f", CTimer::Get_Instance()->Get_FPS());
	SetWindowText(g_hWnd, szFPS);
}
