#include "../stdafx.h"
#include "TileMgr.h"
#include "../Manager/ScrollMgr.h"
#include "../MyBitmap/BmpMgr.h"
#include "../MyBitmap/MyBitmap.h"
#include "../Obj/Tile.h"
#include "../Obj/FireTile.h"

CTileMgr::CTileMgr()
{
}

CTileMgr::~CTileMgr()
{
	Release();
}


bool CTileMgr::Initialize()
{
	m_vecTile.reserve(TILE_NUMX * TILE_NUMY);

	for (int i = 0; i < TILE_NUMY; ++i)
	{
		for (int j = 0; j < TILE_NUMX; ++j)
		{
			float fX = (float)(TILECX * j) + (TILECX >> 1);
			float fY = (float)(TILECY * i) + (TILECY >> 1);
			CObj* pObj = nullptr;
			pObj = CAbstractFactory<CFireTile>::Create(fX, fY);
			m_vecTile.push_back(pObj);
		}
	}
	return true;
}

void CTileMgr::Update(float _fdTime)
{
}

void CTileMgr::Late_Update(float _fdTime)
{
}

void CTileMgr::Render(HDC _DC, float _fdTime)
{
	for (auto& Tile : m_vecTile)
	{
		Tile->Render(_DC, _fdTime, 
			CScrollMgr::Get_Instance()->Get_ScrollX(),
			CScrollMgr::Get_Instance()->Get_ScrollY());
	}
}

void CTileMgr::Release()
{
	Safe_Delete_VecList(m_vecTile);
}

int CTileMgr::Get_Tile_Col_Max(const string& _strKey) const
{
	BITMAP bitmap;
	CMyBitmap* myBitmap = CBmpMgr::Get_Instance()->Find_MyBitmap(_strKey);
	assert(myBitmap);
	HBITMAP hBitmap = myBitmap->Get_Bitmap();
	if (!hBitmap)
		return -1;
	//ZeroMemory(&bitmap, sizeof(BITMAP));
	GetObject(hBitmap, sizeof(BITMAP), (LPVOID)&bitmap);

	int iColMax = int((float)bitmap.bmWidth / TILECX);
	return iColMax;
}

int CTileMgr::Get_Tile_Row_Max(const string& _strKey) const
{
	BITMAP bitmap;
	CMyBitmap* myBitmap = CBmpMgr::Get_Instance()->Find_MyBitmap(_strKey);
	assert(myBitmap);
	HBITMAP hBitmap = myBitmap->Get_Bitmap();
	if (!hBitmap)
		return -1;
	//ZeroMemory(&bitmap, sizeof(BITMAP));
	GetObject(hBitmap, sizeof(BITMAP), (LPVOID)&bitmap);

	int iRowMax = int((float)bitmap.bmHeight / TILECY);
	return iRowMax;
}

void CTileMgr::Picking_Tile(POINT& _pt,
	int _iDrawRow, int _iDrawCol, const TILEENUM::OPTION& _eOption)
{
	int x = _pt.x / TILECX;
	int y = _pt.y / TILECY;

	int iIdx = TILE_NUMX * y + x;

	if (0 > iIdx || m_vecTile.size() <= (size_t)iIdx)
		throw std::out_of_range("Picking_Tile : vector범위 초과!");

	static_cast<CTile*>(m_vecTile[iIdx])->Set_DrawID_Row(_iDrawRow);
	static_cast<CTile*>(m_vecTile[iIdx])->Set_DrawID_Col(_iDrawCol);
	static_cast<CTile*>(m_vecTile[iIdx])->Set_Option(_eOption);
}

void CTileMgr::Catching_Tile(POINT & _pt, int * _iDrawRow, int * _iDrawCol, TILEENUM::OPTION * _eOption)
{
	int x = _pt.x / TILECX;
	int y = _pt.y / TILECY;

	int iIdx = TILE_NUMX * y + x;

	if (0 > iIdx || m_vecTile.size() <= (size_t)iIdx)
		throw std::out_of_range("Picking_Tile : vector범위 초과!");

	*_iDrawRow = static_cast<CTile*>(m_vecTile[iIdx])->Get_DrawID_Row();
	*_iDrawCol = static_cast<CTile*>(m_vecTile[iIdx])->Get_DrawID_Col();
	*_eOption = static_cast<CTile*>(m_vecTile[iIdx])->Get_Option();
	return;
}

// 지금 Save Load가 fire tile일 경우에만 적용됨.
void CTileMgr::Save_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/FireTile.dat"
		, GENERIC_WRITE, NULL, NULL
		, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == NULL) {
		MessageBox(NULL, L"파일 열기 실패", L"CTileMgr::Save_Tile", MB_OK);
		return;
	}

	DWORD	dwByte = 0;
	for (auto& Tile : m_vecTile)
	{
		int iDrawRow = static_cast<CTile*>(Tile)->Get_DrawID_Row();
		int iDrawCol = static_cast<CTile*>(Tile)->Get_DrawID_Col();
		TILEENUM::OPTION eOption = dynamic_cast<CTile*>(Tile)->Get_Option();

		WriteFile(hFile, &Tile->Get_Info(), sizeof(INFO), &dwByte, NULL);
		WriteFile(hFile, &iDrawRow, sizeof(int), &dwByte, NULL);
		WriteFile(hFile, &iDrawCol, sizeof(int), &dwByte, NULL);
		WriteFile(hFile, &eOption, sizeof(TILEENUM::OPTION), &dwByte, NULL);
	}
	CloseHandle(hFile);
}

void CTileMgr::Load_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/FireTile.dat"
		, GENERIC_READ, NULL, NULL
		, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == NULL) {
		MessageBox(NULL, L"파일 열기 실패", L"CTileMgr::Load_Tile", MB_OK);
		return;
	}

	Release();

	INFO	tInfo = {};
	int		iDrawRow = 0, iDrawCol = 0;
	TILEENUM::OPTION eOption = TILEENUM::OPT_END;
	DWORD	dwByte = 0;

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, NULL);
		ReadFile(hFile, &iDrawRow, sizeof(int), &dwByte, NULL);
		ReadFile(hFile, &iDrawCol, sizeof(int), &dwByte, NULL);
		ReadFile(hFile, &eOption, sizeof(TILEENUM::OPTION), &dwByte, NULL);

		if (0 == dwByte)
			break;

		CObj*	pObj = CAbstractFactory<CFireTile>::Create(tInfo.fX, tInfo.fY);
		dynamic_cast<CTile*>(pObj)->Set_DrawID_Row(iDrawRow);
		dynamic_cast<CTile*>(pObj)->Set_DrawID_Col(iDrawCol);
		dynamic_cast<CTile*>(pObj)->Set_Option(eOption);
		m_vecTile.emplace_back(pObj);
	}
	CloseHandle(hFile);
}