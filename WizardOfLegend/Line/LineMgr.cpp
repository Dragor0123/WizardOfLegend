#include "../stdafx.h"
#include "LineMgr.h"
#include "Line.h"
#include "../Manager/ScrollMgr.h"

bool CLineMgr::Initialize()
{
	// LINEPOS tLine[9] = {
	// 	{ float(LEFT_CAPTION), 326.f },
	// 	{ float(LEFT_CAPTION + 141), 391.f },
	// 	{ float(LEFT_CAPTION + 204), 368.f },
	// 	{ float(LEFT_CAPTION + 250), 405.f },
	// 	{ float(LEFT_CAPTION + 296), 300.f },
	// 	{ float(WINCX - RIGHT_CAPTION - 250), 405.f },
	// 	{ float(WINCX - RIGHT_CAPTION - 204), 368.f },
	// 	{ float(WINCX - RIGHT_CAPTION - 141), 391.f },
	// 	{ float(WINCX - RIGHT_CAPTION), 326.f }
	// };
	// 
	// for (int i = 0; i < 8; ++i)
	// 	m_listLine.emplace_back(new CLine(tLine[i], tLine[i + 1]));

	Load_Line();
	return true;
}

void CLineMgr::Render(HDC _DC)
{
	for (auto& pLine : m_listLine)
		pLine->Render(_DC, CScrollMgr::Get_Instance()->Get_ScrollX(), CScrollMgr::Get_Instance()->Get_ScrollY());
}

void CLineMgr::Release()
{
	for_each(m_listLine.begin(), m_listLine.end(), Safe_Delete<CLine*>);
	m_listLine.clear();
}

bool CLineMgr::Collision_Line(float _x, float * _y)
{
	if (m_listLine.empty())
		return false;

	CLine* pTargetLine = nullptr;

	for (auto& pLine : m_listLine)
	{
		if (pLine->Get_Info().tLeftPos.fX <= _x
			&& _x <= pLine->Get_Info().tRightPos.fX) {
			pTargetLine = pLine;
			break;
		}
	}

	if (!pTargetLine)
		return false;

	float x1 = pTargetLine->Get_Info().tLeftPos.fX;
	float y1 = pTargetLine->Get_Info().tLeftPos.fY;
	float x2 = pTargetLine->Get_Info().tRightPos.fX;
	float y2 = pTargetLine->Get_Info().tRightPos.fY;

	*_y = ((y2 - y1) / (x2 - x1)) * (_x - x1) + y1;

	return true;
}

void CLineMgr::Load_Line()
{
	HANDLE hFile = CreateFile(L"../LineData/Line.dat",
		GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"파일 불러오기 실패", L"Error", MB_OK);
		DestroyWindow(g_hWnd);
		return;
	}
	
	LINEINFO tInfo = {};
	DWORD dwByteRead = 0;

	for_each(m_listLine.begin(), m_listLine.end(), Safe_Delete<CLine*>);
	m_listLine.clear();
	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(LINEINFO), &dwByteRead, NULL);
		if (0 == dwByteRead)
			break;
		m_listLine.emplace_back(new CLine(tInfo));
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"파일 불러오기 성공", L"Success", MB_OKCANCEL);
}

CLineMgr::CLineMgr()
{
}

CLineMgr::~CLineMgr()
{
	Release();
}
