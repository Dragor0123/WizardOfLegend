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
#include "../Obj/PlazaTile.h"
#include "../Manager/SceneMgr.h"

CTileEdit::CTileEdit()
	: m_iRow(0), m_iCol(0), m_pTileForPick(nullptr),
	m_eTileOpt(TILEENUM::OPT_MOVE), m_curTileID(TILEENUM::ID_FIRE), m_bCursorEnable(true)
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
	CTileMgr::Get_Instance()->Initialize("FireTile");
	m_pTileForPick = CAbstractFactory<CFireTile>::Create(float(1120 - (TILECX >> 1)), float(80 - (TILECY >> 1)));
	return true;
}

int CTileEdit::Update(float _fdTime)
{
	Key_Check();
	dynamic_cast<CTile*>(m_pTileForPick)->Set_DrawID_Col(m_iCol);
	dynamic_cast<CTile*>(m_pTileForPick)->Set_DrawID_Row(m_iRow);
	dynamic_cast<CTile*>(m_pTileForPick)->Set_Option(m_eTileOpt);
	CTileMgr::Get_Instance()->Update(_fdTime);
	return 0;
}

void CTileEdit::Late_Update(float _fdTime)
{
	CTileMgr::Get_Instance()->Late_Update(_fdTime);
}

void CTileEdit::Collision(float _fdTime)
{
	return;
}

void CTileEdit::Render(HDC _DC, float _fdTime)
{
	TCHAR szText[32] = L"";
	HDC hOneTileDC = GetDC(g_hWnd);
	CTileMgr::Get_Instance()->Render(_DC, _fdTime);

	m_pTileForPick->Render(hOneTileDC, _fdTime, 0, 0);
	swprintf_s(szText, L"타일옵션: %d", (int)(m_eTileOpt));
	TextOut(hOneTileDC, 1060, 130, szText, lstrlen(szText));
	ReleaseDC(g_hWnd, hOneTileDC);
}

void CTileEdit::Release()
{
	SAFE_DELETE(m_pTileForPick);
	CTileMgr::Destroy_Instance();
}

void CTileEdit::Key_Check()
{
	if (KEY_DOWN(VK_TAB)) {
		m_bCursorEnable = (m_bCursorEnable) ? false : true;
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		CScrollMgr::Get_Instance()->Set_ScrollX(5.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		CScrollMgr::Get_Instance()->Set_ScrollX(-5.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
		CScrollMgr::Get_Instance()->Set_ScrollY(5.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
		CScrollMgr::Get_Instance()->Set_ScrollY(-5.f);

	if (KEY_DOWN('2')) {
		m_curTileID = TILEENUM::ID_FIRE;
		if (!Reload_TileMgr()) {
			MessageBox(NULL, L"TileSet 바꾸기 실패", L"Reload_TileMgr", MB_OK);
		}
	}
	else if (KEY_DOWN('1')) {
		m_curTileID = TILEENUM::ID_PLAZA;
		if (!Reload_TileMgr()) {
			MessageBox(NULL, L"TileSet 바꾸기 실패", L"Reload_TileMgr", MB_OK);
		}
	}

	string strKey;
	switch (m_curTileID)
	{
	case TILEENUM::ID_FIRE:
		strKey = "FireTile";
		break;
	case TILEENUM::ID_HOUSE:
		break;
	case TILEENUM::ID_PLAZA:
		strKey = "PlazaTile";
		break;
	case TILEENUM::ID_ICE:
		break;
	case TILEENUM::ID_END:
		break;
	default:
		break;
	}

	if (KEY_DOWN('A')) {
		--m_iCol;
		if (m_iCol < 0)
			m_iCol = CTileMgr::Get_Instance()->Get_Tile_Col_Max(strKey) - 1;
	}
	if (KEY_DOWN('D')) {
		++m_iCol;
		if (m_iCol >= CTileMgr::Get_Instance()->Get_Tile_Col_Max(strKey))
			m_iCol = 0;
	}
	if (KEY_DOWN('W')) {
		--m_iRow;
		if (m_iRow < 0)
			m_iRow = CTileMgr::Get_Instance()->Get_Tile_Row_Max(strKey) - 1;
	}
	if (KEY_DOWN('S')) {
		++m_iRow;
		if (m_iRow >= CTileMgr::Get_Instance()->Get_Tile_Row_Max(strKey))
			m_iRow = 0;
	}

	if (KEY_DOWN('T')) {
		if (m_eTileOpt < TILEENUM::OPT_END - 1){
			m_eTileOpt = (TILEENUM::OPTION)(m_eTileOpt + 1);
		}
	}

	if (KEY_DOWN('R')) {
		if (m_eTileOpt > TILEENUM::OPT_MOVE) {
			m_eTileOpt = (TILEENUM::OPTION)(m_eTileOpt - 1);
		}
	}

	if (m_bCursorEnable) {
		if (KEY_DOWN('9')) {
			CTileMgr::Get_Instance()->Save_Tile(strKey);
			MessageBox(NULL, L"타일 저장!", L"SAVE", MB_OK);
		}
		if (KEY_DOWN('L')) {
			CTileMgr::Get_Instance()->Load_Tile(strKey);
			MessageBox(NULL, L"타일 불러오기!", L"LOAD", MB_OK);
		}
		if (KEY_DOWN(VK_LBUTTON))
		{
			POINT pt = {};
			GetCursorPos(&pt);
			ScreenToClient(g_hWnd, &pt);

			pt.x -= (LONG)CScrollMgr::Get_Instance()->Get_ScrollX();
			pt.y -= (LONG)CScrollMgr::Get_Instance()->Get_ScrollY();

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

			pt.x -= (LONG)CScrollMgr::Get_Instance()->Get_ScrollX();
			pt.y -= (LONG)CScrollMgr::Get_Instance()->Get_ScrollY();
			try {
				CTileMgr::Get_Instance()->Catching_Tile(pt, &m_iRow, &m_iCol, &m_eTileOpt);
			}
			catch (const std::out_of_range&)
			{
				MessageBox(NULL, L"CTileMgr: m_vecTile 범위 초과 ", L"out_of_range", MB_OK);
			}
		}
	}
}

bool CTileEdit::Reload_TileMgr()
{
	CTileMgr::Get_Instance()->Release();
	SAFE_DELETE(m_pTileForPick);
	float fTileX = float(1120 - (TILECX >> 1));
	float fTileY = float(80 - (TILECY >> 1));

	switch (m_curTileID)
	{
	case TILEENUM::ID_FIRE:
		CTileMgr::Get_Instance()->Initialize("FireTile");
		if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Tile/fireMapTile.bmp", "FireTile"))
			return false;
		m_pTileForPick = CAbstractFactory<CFireTile>::Create(fTileX, fTileY);
		return true;
	case TILEENUM::ID_HOUSE:
		break;
	case TILEENUM::ID_PLAZA:
		CTileMgr::Get_Instance()->Initialize("PlazaTile");
		if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Tile/plazaTile.bmp", "PlazaTile"))
			return false;
		m_pTileForPick = CAbstractFactory<CPlazaTile>::Create(fTileX, fTileY);
		return true;
	case TILEENUM::ID_ICE:
		break;
	default:
		break;
	}
	return false;
}
