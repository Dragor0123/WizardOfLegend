#include "../stdafx.h"
#include "TileEdit.h"
#include "../Manager/TileMgr.h"
#include "../MyBitmap/BmpMgr.h"
#include "../Manager/KeyMgr.h"
#include "../Manager/ScrollMgr.h"
#include "../Obj/FireTile.h"
#include "../Obj/ObjMgr.h"
#include "../Obj/Tile.h"
#include "../Manager/ScrollMgr.h"

CTileEdit::CTileEdit()
	: m_iRow(0), m_iCol(0), m_pTileForPick(nullptr), m_eTileOpt(TILEENUM::OPT_MOVE)
{
}


CTileEdit::~CTileEdit()
{
	Release();
}

bool CTileEdit::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Tile/fireMapTile.bmp", "FireTile"))
		return false;
	CTileMgr::Get_Instance()->Initialize();
	m_pTileForPick = CAbstractFactory<CFireTile>::Create(float(1120 - (TILECX >> 1)), float(80 - (TILECY >> 1)));
	CObjMgr::Get_Instance()->Initialize();
	return true;
}

int CTileEdit::Update(float _fdTime)
{
	Key_Check();
	dynamic_cast<CTile*>(m_pTileForPick)->Set_DrawID_Col(m_iCol);
	dynamic_cast<CTile*>(m_pTileForPick)->Set_DrawID_Row(m_iRow);
	dynamic_cast<CTile*>(m_pTileForPick)->Set_Option(m_eTileOpt);
	CTileMgr::Get_Instance()->Update(_fdTime);
	CObjMgr::Get_Instance()->Update(_fdTime);
	return 0;
}

void CTileEdit::Late_Update(float _fdTime)
{
	CTileMgr::Get_Instance()->Late_Update(_fdTime);
	CObjMgr::Get_Instance()->Late_Update(_fdTime);
}

void CTileEdit::Collision(float _fdTime)
{
	//
}

void CTileEdit::Render(HDC _DC, float _fdTime)
{
	TCHAR szText[32] = L"";
	HDC hOneTileDC = GetDC(g_hWnd);
	CTileMgr::Get_Instance()->Render(_DC, _fdTime);

	m_pTileForPick->Render(hOneTileDC, _fdTime, 0, 0);
	swprintf_s(szText, L"타일옵션: %d", (int)(m_eTileOpt));
	TextOut(hOneTileDC, 1060, 130, szText, lstrlen(szText));
	CObjMgr::Get_Instance()->Render(_DC, _fdTime);
	ReleaseDC(g_hWnd, hOneTileDC);
}

void CTileEdit::Release()
{
	CTileMgr::Destroy_Instance();
	CObjMgr::Destroy_Instance();
	SAFE_DELETE(m_pTileForPick);
}

void CTileEdit::Key_Check()
{
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		CScrollMgr::Get_Instance()->Set_ScrollX(50.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		CScrollMgr::Get_Instance()->Set_ScrollX(-50.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
		CScrollMgr::Get_Instance()->Set_ScrollY(50.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
		CScrollMgr::Get_Instance()->Set_ScrollY(-50.f);

	if (KEY_DOWN('A')) {
		--m_iCol;
		if (m_iCol < 0)
			m_iCol = CTileMgr::Get_Instance()->Get_Tile_Col_Max("FireTile") - 1;
	}
	if (KEY_DOWN('D')) {
		++m_iCol;
		if (m_iCol >= CTileMgr::Get_Instance()->Get_Tile_Col_Max("FireTile"))
			m_iCol = 0;
	}
	if (KEY_DOWN('W')) {
		--m_iRow;
		if (m_iRow < 0)
			m_iRow = CTileMgr::Get_Instance()->Get_Tile_Row_Max("FireTile") - 1;
	}
	if (KEY_DOWN('S')) {
		++m_iRow;
		if (m_iRow >= CTileMgr::Get_Instance()->Get_Tile_Row_Max("FireTile"))
			m_iRow = 0;
	}

	if (KEY_DOWN('R')) {
		if (m_eTileOpt < TILEENUM::OPT_END - 1){
			m_eTileOpt = (TILEENUM::OPTION)(m_eTileOpt + 1);
		}
	}

	if (KEY_DOWN('T')) {
		if (m_eTileOpt > TILEENUM::OPT_MOVE) {
			m_eTileOpt = (TILEENUM::OPTION)(m_eTileOpt - 1);
		}
	}

	if (KEY_DOWN('O')) {
		CTileMgr::Get_Instance()->Save_Tile();
		MessageBox(NULL, L"타일 저장!", L"SAVE", MB_OK);
	}
	if (KEY_DOWN('L')) {
		CTileMgr::Get_Instance()->Load_Tile();
		MessageBox(NULL, L"타일 불러오기!", L"LOAD", MB_OK);
	}

	if (KEY_DOWN(VK_LBUTTON))
	{
		POINT pt = {};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= CScrollMgr::Get_Instance()->Get_ScrollX();
		pt.y -= CScrollMgr::Get_Instance()->Get_ScrollY();

		try {
			CTileMgr::Get_Instance()->Picking_Tile(pt, m_iRow, m_iCol, m_eTileOpt);
		}
		catch (const std::out_of_range&)
		{
			MessageBox(NULL, L"CTileMgr: m_vecTile 범위 초과 ", L"out_of_range", MB_OK);
		}
	}

	if (KEY_DOWN(VK_RBUTTON))
	{
		POINT pt = {};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= CScrollMgr::Get_Instance()->Get_ScrollX();
		pt.y -= CScrollMgr::Get_Instance()->Get_ScrollY();
		try {
			CTileMgr::Get_Instance()->Catching_Tile(pt, &m_iRow, &m_iCol, &m_eTileOpt);
		}
		catch (const std::out_of_range&)
		{
			MessageBox(NULL, L"CTileMgr: m_vecTile 범위 초과 ", L"out_of_range", MB_OK);
		}
	}
}
