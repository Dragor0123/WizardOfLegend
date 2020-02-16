#include "stdafx.h"
#include "MainGame.h"
#include "Manager\KeyMgr.h"
#include "Manager\Timer.h"
#include "Manager\PathMgr.h"
#include "Manager\ScrollMgr.h"
#include "Line\LineMgr.h"
#include "Obj\ObjMgr.h"
#include "MyBitmap\BmpMgr.h"
#include "MyBitmap\MyBitmap.h"

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

	// 오브젝트 매니저 초기화
	if (!CObjMgr::Get_Instance()->Initialize())
		return false;

	// 스크롤 매니저 초기화
	if (!CScrollMgr::Get_Instance()->Initialize())
		return false;

	//// 비트맵 매니저 초기화
	if (!CBmpMgr::Get_Instance()->Initialize(_hInst, m_hDC))
		return false;

	// 라인 매니저 초기화 -> 나중에 라인 매니저 지울거임
	if (!CLineMgr::Get_Instance()->Initialize())
		return false;
	
	// Scene 매니저 초기화

	return true;
}

void CMainGame::Input(float _fdTime)
{
	//CSceneMgr::Get_Instance()->Input(_fdTime);
	CObjMgr::Get_Instance()->Input(_fdTime);
}

void CMainGame::Update(float _fdTime)
{
	// CSceneMgr::Get_Instance()->Update(fDeltaTime);
	CObjMgr::Get_Instance()->Update(_fdTime);
}

void CMainGame::Late_Update(float _fdTime)
{
	CObjMgr::Get_Instance()->Late_Update(_fdTime);
	CKeyMgr::Get_Instance()->Key_Update();
}

void CMainGame::Collision(float _fdTime)
{
	CObjMgr::Get_Instance()->Collision(_fdTime);
}

void CMainGame::Render(float _fdTime)
{
	CMyBitmap* pBackbuffer = CBmpMgr::Get_Instance()->Get_BackBuffer();
	CMyBitmap* pBackGround = CBmpMgr::Get_Instance()->Load_Bitmap("Background", L"Stage1.bmp", BITMAP_PATH);
	HDC hBackBuffer = pBackbuffer->Get_DC();
	Rectangle(hBackBuffer, 0, 0, WINCX, WINCY);
	// BitBlt(hBackBuffer, 0, 0, WINCX, WINCY, pBackGround->Get_DC(), -(int)CScrollMgr::Get_Instance()->Get_ScrollX(), -(int)CScrollMgr::Get_Instance()->Get_ScrollY(), SRCCOPY);

	//CLineMgr::Get_Instance()->Render(hBackBuffer);
	CObjMgr::Get_Instance()->Render(hBackBuffer, _fdTime);

	// 고속 전송
	BitBlt(m_hDC, 0, 0, WINCX, WINCY,
		hBackBuffer, 0, 0, SRCCOPY);

	SAFE_RELEASE(pBackbuffer);
}

void CMainGame::Release()
{
	ReleaseDC(g_hWnd, m_hDC);
	CKeyMgr::Destroy_Instance();

	CLineMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();
	CPathMgr::Destroy_Instance();
	CObjMgr::Destroy_Instance();
	CTimer::Destroy_Instance();
}
