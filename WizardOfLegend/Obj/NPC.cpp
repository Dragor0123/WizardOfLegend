#include "../stdafx.h"
#include "NPC.h"
#include "../MyBitmap/BmpMgr.h"
#include "../MyBitmap/MyBitmap.h"

CNPC::CNPC()
	: m_pMyBitmap(nullptr), m_eEmoState(CEmoji::ES_END)
{
	ZeroMemory(szDialogue, sizeof(szDialogue));
}

CNPC::~CNPC()
{
}

bool CNPC::Initialize()
{
	m_pMyBitmap = CBmpMgr::Get_Instance()->Find_MyBitmap(m_strFrameKey);
	if (!m_pMyBitmap)
		return false;

	BITMAP bitmap;
	HBITMAP hBitmap = m_pMyBitmap->Get_Bitmap();
	if (!hBitmap)
		return false;

	GetObject(hBitmap, sizeof(BITMAP), (LPVOID)&bitmap);

	m_tInfo.iCX = (int)bitmap.bmWidth;
	m_tInfo.iCY = (int)bitmap.bmHeight;
	
	// 수정 필요
	m_tHitInfo.iCX = m_tInfo.iCX;
	m_tHitInfo.iCY = m_tInfo.iCY;

	return true;
}

int CNPC::Update(float _fdTime)
{
	if (OBJ_DEAD == CFAble::Update(_fdTime))
		return OBJ_DEAD;

	Update_Rect();
	Update_HitRect();

	return OBJ_NOEVENT;
}

void CNPC::Late_Update(float _fdTime)
{
	CFAble::Key_Check(_fdTime);
}

void CNPC::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	Update_Rect();
	Update_HitRect();

	if (m_pMyBitmap)
	{
		HDC hMemDC = m_pMyBitmap->Get_MemDC();
		GdiTransparentBlt(_DC
			, (int)(m_tRect.left + _fScrollX)
			, (int)(m_tRect.top + _fScrollY)
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC, 0, 0
			, m_tInfo.iCX, m_tInfo.iCY
			, MAGENTA_COLOR);
	}

	Draw_HitBox(_DC, _fScrollX, _fScrollY);
}

void CNPC::Release()
{
}

void CNPC::Do_FButton_Action(float _fdTime)
{
	// 대사 말하기??
}
