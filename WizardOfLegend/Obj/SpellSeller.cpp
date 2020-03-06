#include "../stdafx.h"
#include "SpellSeller.h"
#include "../MyBitmap/BmpMgr.h"
#include "ObjMgr.h"
#include "Emoji.h"

CSpellSeller::CSpellSeller()
{
}

CSpellSeller::~CSpellSeller()
{
	Release();
}

bool CSpellSeller::Initialize()
{
	if (!CFAble::Initialize())
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Fable/SpellSeller.bmp", "SpellSeller"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Fable/stretch_160_180.bmp", "stretch_160_180"))
		return false;

	m_tInfo.iCX = 160;
	m_tInfo.iCY = 180;
	Equalize_HitPosInfoPos();
	m_tHitInfo.iCX = 144;
	m_tHitInfo.iCY = 164;

	m_strFrameKey = "SpellSeller";
	Update_Rect();
	Update_HitRect();
	return true;
}

int CSpellSeller::Update(float _fdTime)
{
	if (OBJ_DEAD == CFAble::Update(_fdTime))
		return OBJ_DEAD;

	if (CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).empty())
	{
		m_pTarget = nullptr;
	}
	else if (!m_pTarget)
	{
		CObj* pPlayer = CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).front();
		m_pTarget = pPlayer;
	}
	else
	{
		float fDX = 0.f, fDY = 0.f, fDis = 0.f, fRadian = 0.f;
		float fPlayerX = 0.f, fPlayerY = 0.f;

		fPlayerX = m_pTarget->Get_PosX();
		fPlayerY = m_pTarget->Get_PosY();

		fDX = fPlayerX - m_tInfo.fX;
		fDY = fPlayerY - m_tInfo.fY;
		fDis = sqrtf(fDX * fDX + fDY * fDY);

		fRadian = acosf(fDX / fDis);

		if (m_tInfo.fY < m_pTarget->Get_Info().fY)
			fRadian = 2 * PI - fRadian;

		m_fAngle = Radian_To_Degree(fRadian);
	}

	Update_Rect();
	Update_HitRect();
	return OBJ_NOEVENT;
}

void CSpellSeller::Late_Update(float _fdTime)
{
	if (m_eEmoState != CEmoji::ES_END)
	{
		CObj* pEmoji = CAbstractFactory<CEmoji>::Create(m_tInfo.fX, m_tHitRect.top - 38.f, "Imoji");
		static_cast<CEmoji*>(pEmoji)->Set_Emoji_State(m_eEmoState);
		CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pEmoji);
		m_eEmoState = CEmoji::ES_END;
	}
}

void CSpellSeller::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);
	HDC hStrDC = CBmpMgr::Get_Instance()->Find_Image("stretch_160_180");

	if (m_fAngle > 90.f && m_fAngle <= 270.f)
	{
		StretchBlt(hStrDC,
			0, 0, m_tInfo.iCX, m_tInfo.iCY, hMemDC,
			m_tInfo.iCX, 0,
			-m_tInfo.iCX, m_tInfo.iCY, SRCCOPY);

		GdiTransparentBlt(_DC,
			(int)(m_tRect.left + _fScrollX),
			(int)(m_tRect.top + _fScrollY),
			m_tInfo.iCX, m_tInfo.iCY,
			hStrDC,
			0,
			0,
			m_tInfo.iCX, m_tInfo.iCY,
			MAGENTA_COLOR);
	}
	else {
		GdiTransparentBlt(_DC,
			(int)(m_tRect.left + _fScrollX),
			(int)(m_tRect.top + _fScrollY),
			m_tInfo.iCX, m_tInfo.iCY,
			hMemDC,
			0,
			0,
			m_tInfo.iCX, m_tInfo.iCY,
			MAGENTA_COLOR);
	}
}

void CSpellSeller::Release()
{
	CBmpMgr::Get_Instance()->Delete_Bmp("SpellSeller");
	CBmpMgr::Get_Instance()->Delete_Bmp("stretch_160_180");
}

void CSpellSeller::Do_FButton_Action(float _fdTime)
{
}
